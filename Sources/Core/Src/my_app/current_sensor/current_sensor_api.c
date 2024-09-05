/**
 * @file current_sensor.c
 * @author Nicolás Almaraz
 * @brief Implements functionality for reading the current sensor
 *
 * Current Sensor Calculations:
 * 		Sensitivity:
 * 		S = 185mV/A
 * 		|--> 1mV = 1.2409 counts
 * 		|--> S = 229.5681 counts / A
 *
 * 		counts = S * current
 * 		|--> current = counts * 1/S
 * 		|--> 1/S = 0.004356004 A / counts
 * 		|--> 1/S = 4.356004 mA / counts
 */

//======================================
// Dependencies
//======================================
#include <string.h>

#include "main.h"
#include "current_sensor_api.h"
#include "timer_api/timer_api.h"

//======================================
// Private Defines
//======================================
/** Maximum value of a 16-bit unsigned integer */
#define MAX_UINT16_t 				65535

/** Calibration tolerance in milliamps */
#define CALIBRATION_TOLERANCE_MA	1

/** Current slope in milliamps per count */
#define CURRENT_SLOPE 				(float) 4.356004 // [mA/counts]

//======================================
// Private Data Structures and Types
//======================================

/** State machine for the current sensor */
typedef enum {
	STATE_START,		/**< Initial state */
	STATE_SAMPLING,		/**< Sampling in progress */
	STATE_WAITING,		/**< Waiting for timer or cycle */
	STATE_EOC,			/**< End of cycle (EOC) */
} state_current_sensor_t;

//======================================
// STM32 Handlers
//======================================
/** ADC handle for the current sensor (defined in main.c) */
extern ADC_HandleTypeDef hadc1;

//======================================
// Private Variables
//======================================

/** Current state of the sensor's state machine */
static state_current_sensor_t g_state = STATE_START;

/** Array of current samples for multiple cycles*/
static cycle_t g_current_cycles[CYCLES];

/** Index for tracking the current cycle in the array*/
static uint32_t g_cycles_index = 0;

/** Offset applied to the ADC readings for calibration*/
static uint16_t g_offset = MAX_UINT16_t / 2;

/** Flag indicating if a new cycle in the line voltage has occurred*/
static bool g_f_new_cycle = false;

//======================================
// Private Function Declarations
//======================================

/**
 * @brief Returns the value read by the ADC
 * @retval 16-bit ADC sample
 */
uint16_t current_sensor_api_get_sample_ADC();

//======================================
// Private Function Implementations
//======================================

uint16_t current_sensor_api_get_sample_ADC() {
	uint32_t sample = HAL_ADC_GetValue(&hadc1);
	return (uint16_t)sample;
}

//======================================
// Public Function Implementations
//======================================

void current_sensor_api_init() {
	current_sensor_api_clean_samples();
}

bool current_sensor_api_calibrate() {
	uint16_t sample_max = 0;
	uint16_t sample_min = MAX_UINT16_t;
	uint32_t sum = 0;

	for(uint32_t i = 0; i < 100; i++) {
		// Take a sample
		uint16_t sample = current_sensor_api_get_sample_ADC();

		// Update max and min
		if(sample > sample_max)
			sample_max = sample;

		if(sample < sample_min)
			sample_min = sample;

		sum += sample;

		// Wait 10ms
		HAL_Delay(10);
	}

	// Calculate max deviation
	float variability = (sample_max - sample_min) * CURRENT_SLOPE;
	if(variability > CALIBRATION_TOLERANCE_MA)
		return CALIBRATE_ERROR;

	// Calculate offset
	g_offset = (uint16_t) (sum / 100);
	return CALIBRATE_OK;
}

bool current_sensor_api_sampling_loop() {
	bool f_eoc = false;

	switch(g_state) {
		case STATE_START:
			if(g_f_new_cycle) {
				g_state = STATE_SAMPLING;
			}
			break;

		case STATE_SAMPLING:
			// Take a new sample
			uint16_t sample = current_sensor_api_get_sample_ADC();

			// Start timer
			timer_api_set_count(TIMER_SAMPLING, 200);

			// If a new cycle notification is received, change the cycle
			if(g_f_new_cycle) {
				g_f_new_cycle = false;
				g_cycles_index++;

				// If all CYCLES are completed, return EOC
				if(g_cycles_index == CYCLES) {
					g_state = STATE_EOC;
					f_eoc = true;
					break;
				}
			}

			// Save the sample
			uint32_t len = g_current_cycles[g_cycles_index].len;
			g_current_cycles[g_cycles_index].cycle[len] = sample;
			len++;
			g_current_cycles[g_cycles_index].len = len;
			break;

		case STATE_WAITING:
			// Return to sampling state if...

			// 1 - Timeout
			if(timer_api_check_timer(TIMER_SAMPLING) == TIMER_FINISHED) {
				g_state = STATE_SAMPLING;
			}

			// 2 - A new cycle starts
			// The cycles must be properly aligned for processing
			if(g_f_new_cycle) {
				g_state = STATE_SAMPLING;
			}
			break;

		case STATE_EOC:
			f_eoc = true;
			break;
	}

	return f_eoc;
}

void current_sensor_api_clean_samples() {
	g_state = STATE_START;
	g_cycles_index = 0;
	for(uint32_t i = 0; i < CYCLES; i++) {
		g_current_cycles[i].len = 0;
	}
}

void current_sensor_api_get_samples(cycle_t *ptr_out) {
    memcpy(ptr_out, g_current_cycles, sizeof(g_current_cycles));
}

void current_sensor_api_set_new_cycle() {
	g_f_new_cycle = true;
}

uint16_t current_sensor_api_get_offset() {
	return g_offset;
}
