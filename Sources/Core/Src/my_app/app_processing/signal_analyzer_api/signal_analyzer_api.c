/**
 * @file current_sensor.h
 * @author Nicolás Almaraz
 * @brief Main signal processing to obtain THD and current to inject
 */

//======================================
// Dependencies
//======================================
#include <app_processing/current_sensor_api/current_sensor_api.h>
#include <app_processing/signal_analyzer_api/CMSIS_DSP/Include/arm_math.h>
#include <app_processing/signal_analyzer_api/signal_analyzer_api.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_app.h"


//======================================
// Private Defines
//======================================
#define FFT_MAX_LEN 2048 //!< Length for FFT

//======================================
// Private Structures and Data Types
//======================================

/**
 * @brief Characteristics values of a sine waveform
 */
typedef struct {
    float32_t amplitude;	//!< Amplitude [mA]
    float32_t period_us;	//!< Frequency [Hz]
    float32_t phase_us;		//!< Phase [rad]
} sine_t;

typedef enum {
	STATE_AVERAGE_POWER,
	STATE_FUNDAMENTAL,
	STATE_FUND_POWER,
	STATE_THD,
	STATE_SIGNAL_TO_INJECT,
	STATE_PROCESSING_COMPLETED,
} state_processing_t;

//======================================
// Private Variables
//======================================
static cycle_t g_ave_cycle;
static uint16_t g_zero_offset;
static cycle_t g_inject_cycle;
static int8_t g_thd;
static state_processing_t g_state = STATE_AVERAGE_POWER;

//======================================
// Private Function Declarations
//======================================

/**
 * @brief Calculate signal power
 * @param cycle One cycle of the signal
 * @param offset Offset of the signal
 * @retval power
 *
 * Energy = Σ {first_sample, last_sample} {sample**2}
 * Power = Energy / samples
 */
static float32_t signal_analyzer_api_calculate_sig_power(cycle_t cycle, uint16_t offset);

/**
 * @brief Calculate Fundamental Harmonic
 * @param cycle This is a cycle of the signal
 * @param zero_offset This is the offset
 * @retval Returns the characteristic values of the fundamental harmonic (amplitude, frequency, and phase)
 */
static sine_t signal_analyzer_api_calculate_fundamental(cycle_t cycle, uint16_t zero_offset);

/**
 * @brief Calculate the power of a sine
 *
 * power = amplitude **2 / 2
 *
 * @param sine
 * @retval power of the sine
 */
static float32_t signal_analyzer_api_calculate_sine_power(sine_t sine);

/**
 * @brief Calculate THD
 * @param fund_power Power of the fundamental
 * @param signal_power Power of the signal
 *
 * THD% = 100% * power_harmonics / power_fundamental
 */
static uint8_t signal_analyzer_api_calculate_thd(float32_t fund_power, float32_t signal_power);

/**
 * @brief Calculate signal to inject
 * @param fundamental The desired current signal in the line
 * @param ave_cycle One cycle of the current waveform
 * @param zero_offset Offset
 *
 * @retval The signal to inject at each "start of cycle" interrupt
 *
 * inject_cycle = fundamental - distorted_cycle
 *
 */
static cycle_t signal_analyzer_api_calculate_injection(sine_t fundamental, cycle_t ave_cycle, uint16_t zero_offset);

/**
 * @brief Apply Flat Top Window to the vector data
 * @param data ptr to the data
 * @note data will be modified
 * @param len length of the vector
 */
void signal_analyzer_api_apply_flat_top_window(float32_t *data, uint32_t len);

//======================================
// Private Function Implementations
//======================================

float32_t signal_analyzer_api_calculate_sig_power(cycle_t cycle, uint16_t offset) {
    float64_t energy = 0;
    for(uint32_t i = 0; i < cycle.len; i++) {
    	int32_t value = cycle.buffer[i] - offset;
        energy += (value * value);
    }
    energy /= cycle.len;
    float64_t power = (float32_t)energy;
    return power;
}

void signal_analyzer_api_apply_flat_top_window(float32_t *data, uint32_t len) {
    for (uint32_t n = 0; n < len; n++) {
        float32_t w = 1.0f - 1.93f * cosf(2.0f * M_PI * n / (len - 1)) +
                      1.29f * cosf(4.0f * M_PI * n / (len - 1)) -
                      0.388f * cosf(6.0f * M_PI * n / (len - 1)) +
                      0.032f * cosf(8.0f * M_PI * n / (len - 1));
        data[n] *= w;
    }
}

sine_t signal_analyzer_api_calculate_fundamental(cycle_t cycle, uint16_t zero_offset) {
    float32_t *fft_output = NULL;
    float32_t *fft_input = NULL;
    float32_t *magnitude = NULL;
    uint32_t len = cycle.len;
    uint32_t padded_len = FFT_MAX_LEN;

    // Allocate memory for buffers in and out
    fft_input = (float32_t*) calloc(padded_len, sizeof(float32_t));
    fft_output = (float32_t*) calloc(padded_len, sizeof(float32_t));
    magnitude = (float32_t*) calloc(padded_len / 2 + 1, sizeof(float32_t));

    // Check allocation
    if (fft_input == NULL || fft_output == NULL) {
        sine_t error_result = { .amplitude = 0.0f, .period_us = 0.0f, .phase_us = 0.0f };
        if (fft_input)
        	free(fft_input);
        if (fft_output)
        	free(fft_output);
        if (magnitude)
            free(magnitude);
        return error_result;
    }

    // Repeat the cycle for vector
    uint32_t repetitions = padded_len / len;
    uint32_t remainder = padded_len % len;

    uint32_t index = 0;

    // Entire quantity
    for (uint32_t rep = 0; rep < repetitions; rep++) {
        for (uint32_t i = 0; i < len; i++) {
            fft_input[index++] = cycle.buffer[i] - zero_offset;
        }
    }

    // Rest of samples
    for (uint32_t i = 0; i < remainder; i++) {
        fft_input[index++] = cycle.buffer[i] - zero_offset;
    }

    // --- Calculate the FFT ---

    // Initialize FFT instance
    arm_rfft_fast_instance_f32 fft;
    arm_status status = arm_rfft_fast_init_f32(&fft, padded_len);
    if (status != ARM_MATH_SUCCESS) {
        sine_t error_result = { .amplitude = 0.0f, .period_us = 0.0f, .phase_us = 0.0f };
        if (fft_input)
        	free(fft_input);
        if (fft_output)
        	free(fft_output);
        if (magnitude)
        	free(magnitude);
        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, true); //debug
        return error_result;
	}

    //Apply a flat top window for better accuracy in amplitude
    signal_analyzer_api_apply_flat_top_window(fft_input, padded_len);

    // Calculate FFT
    uint8_t ifftFlag = 0;
    arm_rfft_fast_f32(&fft, fft_input, fft_output, ifftFlag);
    arm_cmplx_mag_f32(fft_output, magnitude, padded_len/2);

    // Calculate the fundamental harmonic
    float32_t   maxValue;
	uint32_t    maxIndex;
    arm_max_f32(magnitude, padded_len/2, &maxValue, &maxIndex);

    // Fill the return structure
    sine_t fundamental;
    fundamental.amplitude = sqrt(maxValue);
    fundamental.period_us = len * SAMPLING_PERIOD_US;   // Period in microseconds
    fundamental.phase_us = 0.0f;                        // Assuming no phase shift

    // Free memory
    free(fft_output);
    free(fft_input);
    free(magnitude);

    return fundamental;
}

float32_t signal_analyzer_api_calculate_sine_power(sine_t sine) {
    float32_t power = sine.amplitude * sine.amplitude / 2;
    return power;
}

uint8_t signal_analyzer_api_calculate_thd(float32_t fund_power, float32_t signal_power) {
    uint32_t thd = (signal_power - fund_power) * 100 / fund_power;
    return (uint8_t)thd;
}

cycle_t signal_analyzer_api_calculate_injection(sine_t fundamental, cycle_t ave_cycle, uint16_t zero_offset) {
    // Save the cycle length to inject
    uint32_t len = ave_cycle.len;

    // Generate array with the fundamental sine
    float32_t *ptr_fund = (float32_t*) calloc (len, sizeof(float32_t));
    for (int i = 0; i < len; i++) {
        ptr_fund[i] = fundamental.amplitude * arm_sin_f32(2 * PI * i / len);
    }

    // Calculate cycle to inject
    // Remember...
    // inject_cycle = fundamental_cycle - distorted_cycle
    cycle_t cycle_to_inject;
    for(uint32_t i = 0; i < len; i++) {
        cycle_to_inject.buffer[i] = (uint16_t) (ptr_fund[i] - ave_cycle.buffer[i] + zero_offset);
    }

    free(ptr_fund);

    // This signal must be injected at each zero cross
    return cycle_to_inject;
}


//======================================
// Public Function Implementations
//======================================

void signal_analyzer_api_set_signal_to_analyze(cycle_t ave_cycle, uint16_t zero_offset) {
	g_ave_cycle = ave_cycle;
	g_zero_offset = zero_offset;
	g_state = STATE_AVERAGE_POWER;
}

status_processing_t signal_analyzer_api_analyze_loop() {
    static float32_t current_power;
    static sine_t fundamental;
    static float32_t fundamental_power;

    /**
     * Some operations take too much time,
     * So I divide them into a state machine to proceed in stages
     * and refresh the rest of the functionalities within the main loop
     */
    switch(g_state) {
    	case STATE_AVERAGE_POWER:
    		current_power = signal_analyzer_api_calculate_sig_power(g_ave_cycle, g_zero_offset);
    		g_state = STATE_FUNDAMENTAL;
    		break;

    	case STATE_FUNDAMENTAL:
    		fundamental = signal_analyzer_api_calculate_fundamental(g_ave_cycle, g_zero_offset);
    		g_state = STATE_FUND_POWER;
    		break;

    	case STATE_FUND_POWER:
    		fundamental_power = signal_analyzer_api_calculate_sine_power(fundamental);
    		g_state = STATE_THD;
    		break;

    	case STATE_THD:
    		g_thd = signal_analyzer_api_calculate_thd(fundamental_power, current_power);
    		g_state = STATE_SIGNAL_TO_INJECT;
    		break;

    	case STATE_SIGNAL_TO_INJECT:
    		g_inject_cycle = signal_analyzer_api_calculate_injection(fundamental, g_ave_cycle, g_zero_offset);
    		g_state = STATE_PROCESSING_COMPLETED;
    		break;

    	case STATE_PROCESSING_COMPLETED:
    		break;
    }
    if(g_state != STATE_PROCESSING_COMPLETED)
    	return PROCESSING_IN_PROGRESS;
    else
    	return PROCESSING_COMPLETED;
}

cycle_t signal_analyzer_api_get_cycle_to_inject(void) {
    return g_inject_cycle;
}

uint8_t signal_analyzer_api_get_thd(void) {
    return g_thd;
}
