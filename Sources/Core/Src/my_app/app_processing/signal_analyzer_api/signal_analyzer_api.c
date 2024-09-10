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

//======================================
// Private Structures and Data Types
//======================================

/**
 * @brief Characteristics values of a sine waveform
 */
typedef struct {
    float32_t amplitude;	//!< Amplitude [mA]
    float32_t frequency;	//!< Frequency [Hz]
    float32_t phase;		//!< Phase [rad]
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
static uint8_t g_thd;
static state_processing_t g_state = STATE_AVERAGE_POWER;

//======================================
// Private Function Declarations
//======================================

/**
 * @brief Calculate signal power
 * @param cycle One cycle of the signal
 * @retval power
 *
 * Energy = Σ {first_sample, last_sample} {sample**2}
 * Power = Energy / samples
 */
static float32_t signal_analyzer_api_calculate_sig_power(cycle_t cycle);

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


//======================================
// Private Function Implementations
//======================================

float32_t signal_analyzer_api_calculate_sig_power(cycle_t cycle) {
    uint32_t energy = 0;
    for(uint32_t i = 0; i < cycle.len; i++) {
        energy += (cycle.buffer[i] * cycle.buffer[i]);
    }

    float32_t power = (float32_t) energy / cycle.len;
    return power;
}

sine_t signal_analyzer_api_calculate_fundamental(cycle_t cycle, uint16_t zero_offset) {
    float32_t *ptr_fft;
    float32_t *ptr_in;
    float32_t *magnitude;
    uint32_t len = cycle.len;

    // Assign memory for input and output arrays
    ptr_in = (float32_t*) calloc (len, sizeof(float32_t));
    ptr_fft = (float32_t*) calloc (len, sizeof(float32_t));
    magnitude = (float32_t*) calloc (len/2, sizeof(float32_t));

    // Filter the DC component of the signal
    for(uint32_t i = 0; i < len; i++) {
        ptr_in[i] = cycle.buffer[i] - zero_offset;
    }

    // --- Calculate the FFT ---

    // Initialize FFT instance
    arm_rfft_fast_instance_f32 fft_handler;
    arm_rfft_fast_init_f32(&fft_handler, len);

    // Calculate FFT
    arm_rfft_fast_f32(&fft_handler, ptr_in, ptr_fft, 0);
    // Discard the negative components
    arm_cmplx_mag_f32(ptr_fft, magnitude, len / 2);

    // Calculate the fundamental harmonic
    uint32_t fund_i;
    float32_t amplitude;
    arm_max_f32(magnitude, len / 2, &amplitude, &fund_i);
    float32_t fund_freq = (SAMPLING_FREQUENCY_HZ * fund_i) / len;

    // Fill the return structure
    sine_t fundamental;
    fundamental.amplitude = amplitude;
    fundamental.frequency = fund_freq;
    fundamental.phase = 0;  // Assuming no phase shift

    // Free memory
    free(ptr_fft);
    free(ptr_in);
    free(magnitude);

    return fundamental;
}

float32_t signal_analyzer_api_calculate_sine_power(sine_t sine) {
    float32_t power = sine.amplitude * sine.amplitude / 2;
    return power;
}

uint8_t signal_analyzer_api_calculate_thd(float32_t fund_power, float32_t signal_power) {
    uint8_t thd = (uint8_t) (signal_power - fund_power) * 100 / fund_power;
    return thd;
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
    		current_power = signal_analyzer_api_calculate_sig_power(g_ave_cycle);
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
    		current_power = signal_analyzer_api_calculate_thd(fundamental_power, current_power);
    		g_state = STATE_FUNDAMENTAL;
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
