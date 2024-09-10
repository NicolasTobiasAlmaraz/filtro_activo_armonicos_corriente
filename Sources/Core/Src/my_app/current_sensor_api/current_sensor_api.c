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
#include "common_apis/timer_api/timer_api.h"

//======================================
// Private Defines
//======================================
/** Maximum value of a 16-bit unsigned integer */
#define MAX_UINT16_t 				65535

/** Calibration tolerance in milliamps */
#define CALIBRATION_TOLERANCE_MA	1

/** Current slope in milliamps per count */
#define CURRENT_SLOPE 		(float) 4.356004 // [mA/counts]

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

static state_current_sensor_t g_state = STATE_START; 	//!< Current state of the sensor's state machine
static cycle_t g_current_cycles[CYCLES];				//!< Array of current samples for multiple cycles
static uint32_t g_cycles_index = 0;						//!< Index for tracking the current cycle in the array
static uint16_t g_offset = MAX_UINT16_t / 2;			//!< Offset applied to the ADC readings for calibration
static bool g_f_new_cycle = false; 						//!< Flag indicating if a new cycle in the line voltage has occurred
static status_sampling_t g_status = SAMPLING_COMPLETED;	//!< Status of the current sampling


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
	HAL_ADC_PollForConversion(&hadc1, 1);
	uint32_t sample = HAL_ADC_GetValue(&hadc1);
	return (uint16_t)sample;
}

//======================================
// Public Function Implementations
//======================================

void current_sensor_api_init() {
	HAL_ADC_Init(&hadc1);
	HAL_ADC_Start(&hadc1);
	current_sensor_api_clean_samples();
}

//todo borrar
extern TIM_HandleTypeDef htim2;

status_calibration_t current_sensor_api_calibrate() {
	uint16_t sample_max = 0;
	uint16_t sample_min = MAX_UINT16_t;
	uint32_t sum = 0;

	for(uint32_t i = 0; i < 10000; i++) {
		// Take a sample
		uint32_t t1 = __HAL_TIM_GET_COUNTER(&htim2);
		uint16_t sample = current_sensor_api_get_sample_ADC();
		uint32_t t2 = __HAL_TIM_GET_COUNTER(&htim2);
		uint32_t tiempo_sample = t2-t1;

		// Update max and min
		if(sample > sample_max)
			sample_max = sample;

		if(sample < sample_min)
			sample_min = sample;

		sum += sample;

		// Wait 1ms
		t1 = __HAL_TIM_GET_COUNTER(&htim2);
		HAL_Delay(1);
		t2 = __HAL_TIM_GET_COUNTER(&htim2);
		uint32_t tiempo_delay = t2-t1;
	}

	// Calculate max deviation
	float variability = (sample_max - sample_min) * CURRENT_SLOPE;
	if(variability > CALIBRATION_TOLERANCE_MA)
		return CALIBRATE_ERROR;

	// Calculate offset
	g_offset = (uint16_t) (sum / 100);
	return CALIBRATE_OK;
}

void current_sensor_api_loop() {
	switch(g_state) {
		case STATE_START:
			if(g_f_new_cycle) {
				g_state = STATE_SAMPLING;
				g_status = SAMPLING_IN_PROGRESS;
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
					g_status = SAMPLING_COMPLETED;
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

			// 1 - Timeout fs --> Time to take another sample
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
			//Do nothing
			break;
	}
}

status_sampling_t current_sensor_api_get_status() {
	return g_status;
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
