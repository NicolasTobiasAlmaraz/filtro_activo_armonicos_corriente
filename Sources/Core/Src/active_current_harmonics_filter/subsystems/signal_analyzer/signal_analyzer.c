/**
 * @file signal_analyzer.c
 * @author Nicolás Almaraz
 * @brief Header file for main signal processing to obtain THD and correction waveform.
 *
 * This API processes a `cycle_t` signal to obtain the Total Harmonic Distortion (THD)
 * and the correction waveform needed to compensate for harmonics.
 */

//======================================
// Dependencies
//======================================
#include <stdio.h>
#include <stdlib.h>

#include "arm_math.h"

#include "active_current_harmonics_filter.h"
#include "signal_analyzer.h"


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
    float32_t phase_us;		//!< Phase [us]
} sine_t;

/**
 * @brief States for processing
 */
typedef enum {
	STATE_FFT,					//!< Calculating FFT for obtain fundamental and THD
	STATE_SIGNAL_TO_INJECT,		//!< Calculating signal to inject
	STATE_PROCESSING_COMPLETED,	//!< Processing completed
} state_processing_t;

//======================================
// Private Variables
//======================================
static cycle_t m_ave_cycle; 					//!< Signal to analyze (one period)
static uint16_t m_zero_offset; 					//!< Calibration offset
static cycle_t m_inject_cycle; 					//!< Cycle to inject for harmonic compensation
static uint16_t m_thd; 							//!< THD of the average cycle
static state_processing_t m_state = STATE_FFT; 	//!< State


//======================================
// Private Function Declarations
//======================================

/**
 * @brief Calculate Fundamental Harmonic and THD with FFT method
 * @param cycle This is a cycle of the signal
 * @param zero_offset This is the offset
 * @retval Returns the characteristic values of the fundamental harmonic (amplitude, frequency, and phase)
 */
static sine_t m_calculate_fft(cycle_t cycle, uint16_t zero_offset);

/**
 * @brief Calculate signal to inject
 * @param fundamental The desired current signal in the line
 * @param ave_cycle One cycle of the current waveform
 * @param zero_offset Offset
 * @retval The signal to inject at each "start of cycle" interrupt
 *
 * inject_cycle = fundamental - distorted_cycle
 *
 */
static cycle_t m_calculate_injection(sine_t fundamental, cycle_t ave_cycle, uint16_t zero_offset);

/**
 * @brief Applies Flat Top Window to the vector data
 * @param data Pointer to the data
 * @note The data will be modified
 * @param len Length of the vector
 * @retval norm Normalization coefficient
 */
float32_t m_flat_top_window(float32_t *data, uint32_t len);

//======================================
// Private Function Implementations
//======================================

float32_t m_flat_top_window(float32_t *data, uint32_t len) {
	float32_t correction_factor = 0;
	for (uint32_t n = 0; n < len; n++) {
        float32_t w = 1.0f - 1.93f * cosf(2.0f * M_PI * n / (len - 1)) +
                      1.29f * cosf(4.0f * M_PI * n / (len - 1)) -
                      0.388f * cosf(6.0f * M_PI * n / (len - 1)) +
                      0.032f * cosf(8.0f * M_PI * n / (len - 1));
        data[n] *= w;
        correction_factor += w;
    }
	return correction_factor / len;
}

/*
 * todo:
 * Dividir esta funcion en varias
 * asi no ocupo el proc por tanto tiempo
 * sin atender el resto de cosas
 */
sine_t m_calculate_fft(cycle_t cycle, uint16_t zero_offset) {
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
        //HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, true); //debug
        return error_result;
	}

    //Apply a flat top window for better accuracy in amplitude
    float32_t correction = m_flat_top_window(fft_input, padded_len);

    // Calculate FFT
    uint8_t ifftFlag = 0;
    arm_rfft_fast_f32(&fft, fft_input, fft_output, ifftFlag);
    arm_cmplx_mag_f32(fft_output, magnitude, padded_len/2);

    // Calculate the fundamental harmonic
    float32_t   fund_power;
	uint32_t    fund_index;
    arm_max_f32(magnitude, padded_len/2, &fund_power, &fund_index);

    // Fill the return structure
    sine_t fundamental;
    fundamental.amplitude = sqrt(fund_power/correction)/2;			// P = maxValue = A**2/2 --> A = sqrt(maxValue*2)
    fundamental.period_us = len * SAMPLING_PERIOD_US;   // Period in microseconds
    fundamental.phase_us = 0.0f;                        // Assuming no phase shift

    float64_t harm_power = 0;
    for(uint32_t i=fund_index*2; i<padded_len/2; i++) {
    	//Add Harm k*fundamental_freq
    	if(i%fund_index==0)
    		harm_power += (magnitude[i]/correction)*(magnitude[i]/correction);
    }

    // Calculate THD in base of the harmonics (k*F0)
    m_thd = 100*(harm_power / fund_power);

    // Free memory
    free(fft_output);
    free(fft_input);
    free(magnitude);

    return fundamental;
}

cycle_t m_calculate_injection(sine_t fundamental, cycle_t ave_cycle, uint16_t zero_offset) {
    // Save the cycle length to inject
    uint32_t len = ave_cycle.len;

    // Generate array with the fundamental sine
    float32_t *ptr_fund = (float32_t*) calloc (len, sizeof(float32_t));
    for (int i = 0; i < len; i++) {
        ptr_fund[i] = fundamental.amplitude * arm_sin_f32(2 * PI * i / len) + zero_offset*2;
    }

    // Calculate cycle to inject
    // Remember...
    // inject_cycle = fundamental_cycle - distorted_cycle
    cycle_t cycle_to_inject;
    for(uint32_t i = 0; i < LEN_MAX; i++) {
    	if(i >= len) {
    		cycle_to_inject.buffer[i] = m_zero_offset;
    		continue;
    	}
        cycle_to_inject.buffer[i] = (uint16_t) (ptr_fund[i] - ave_cycle.buffer[i]);
    }

    free(ptr_fund);

    // This signal must be injected at each zero cross
    return cycle_to_inject;
}


//======================================
// Public Function Implementations
//======================================

void signal_analyzer_set_signal_to_analyze(cycle_t ave_cycle, uint16_t zero_offset) {
	m_ave_cycle = ave_cycle;
	m_zero_offset = zero_offset;
	m_state = STATE_FFT;
}

status_processing_t signal_analyzer_state_machine() {
    static sine_t fundamental;
    /**
     * Some operations take too much time,
     * So I divide them into a state machine to proceed in stages
     * and refresh the rest of the functionalities within the main loop
     */
    switch(m_state) {
		default:
		case STATE_FFT:
			//Calculate fundamental component
			//Calculate THD
    		fundamental = m_calculate_fft(m_ave_cycle, m_zero_offset);
    		m_state = STATE_SIGNAL_TO_INJECT;
    		break;

    	case STATE_SIGNAL_TO_INJECT:
    		//Calculate signal to inject
    		m_inject_cycle = m_calculate_injection(fundamental, m_ave_cycle, m_zero_offset);
    		m_state = STATE_PROCESSING_COMPLETED;
    		break;

    	case STATE_PROCESSING_COMPLETED:
    		break;
    }
    if(m_state != STATE_PROCESSING_COMPLETED)
    	return PROCESSING_IN_PROGRESS;
    else
    	return PROCESSING_COMPLETED;
}

cycle_t signal_analyzer_get_cycle_to_inject(void) {
    return m_inject_cycle;
}

uint16_t signal_analyzer_get_thd(void) {
    return m_thd;
}

