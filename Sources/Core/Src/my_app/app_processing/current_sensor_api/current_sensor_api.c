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
#include <stdio.h>
#include <stdlib.h>

#include <app_processing/current_sensor_api/current_sensor_api.h>
#include <string.h>

#include "main.h"
#include "common_apis/timer_api/timer_api.h"

//======================================
// Private Defines
//======================================
/** Maximum value of a 16-bit unsigned integer */
#define MAX_UINT16_t 				65535

/** Calibration tolerance in milliamps */
#define CALIBRATION_TOLERANCE_MA	1

/** Current slope in milliamps per count */
#define CURRENT_SENSIBILITY 		(float) 4.356004 // [mA/counts]

/** ADC Buffer Len DMA*/
#define ADC_BUF_LEN             	20000

/** ADC SAMPLING PERIOD (Ts) */
#define SAMPLING_PERIOD_US			50

//======================================
// Private Data Structures and Types
//======================================

/** State machine for the current sensor */
typedef enum {
	STATE_RESET,		/**< Sampling completed */
	STATE_SAMPLING,		/**< Sampling in progress */
} state_current_sensor_t;

//======================================
// STM32 Handlers
//======================================
/** ADC handle for the current sensor (defined in main.c) */
extern ADC_HandleTypeDef hadc1;

//======================================
// Private Variables
//======================================

static uint16_t g_ADC_buffer[ADC_BUF_LEN];					//!< ADC Buffer
static uint16_t g_offset = MAX_UINT16_t / 2;				//!< Offset applied to the ADC readings for calibration
static uint32_t g_period_AC_sig_us = 20000; 				//!< AC period signal (In theory 50Hz --> 20000 us)
static status_sampling_t g_status_sam = SAMPLING_COMPLETED;	//!< Status of the sampling
static bool g_f_eoc_dma = false;							//!< Status of DMA Conv

//======================================
// Private Function Declarations
//======================================

#ifdef DEBUG_NICOLAS
/**
 * @brief Takes one sample by polling.
 * Typical Time Conversion: 3 or 4 useg
 */
static uint16_t current_sensor_api_get_sample_ADC();
#endif

/**
 * @brief Start the ADC using DMA Controller by Single Conversion
 */
static void current_sensor_api_start_ADC_sampling_DMA(uint16_t *ptr);

//======================================
// Private Function Implementations
//======================================
void current_sensor_api_start_ADC_sampling_DMA(uint16_t *ptr) {
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ptr, 1);
}

#ifdef DEBUG_NICOLAS
uint16_t current_sensor_api_get_sample_ADC() {
	uint32_t t1 = timer_api_get_ticks();


	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	uint16_t raw = HAL_ADC_GetValue(&hadc1);

	uint32_t t2 = timer_api_get_ticks();
	uint32_t time_conv = t2-t1;
	if(time_conv>1000)
		return 0;


	return raw;

}
#endif

//======================================
// Public Function Implementations
//======================================

void current_sensor_api_init() {
	//Init ADC
	HAL_ADC_Init(&hadc1);
}

void current_sensor_api_start_sampling() {
	g_status_sam = SAMPLING_IN_PROGRESS;
	timer_api_enable_interrupts();
}

status_sampling_t current_sensor_api_get_status() {
	return g_status_sam;
}

status_calibration_t current_sensor_api_get_calibration() {
	status_calibration_t retval;
	uint64_t sum=0;
	uint16_t max_value = 0;
	uint16_t min_value = MAX_UINT16_t;

	//Process buff
	for(uint32_t i=0 ; i<ADC_BUF_LEN ; i++) {
		uint16_t raw = g_ADC_buffer[i];
		sum += raw;
		if(raw > max_value)
			max_value = raw;
		if(raw < min_value)
			min_value = raw;
	}

	//Calculate max variation
	float slope_ma = (max_value - min_value) * CURRENT_SENSIBILITY;
	retval = CALIBRATE_OK;
	if(slope_ma > CALIBRATION_TOLERANCE_MA)
		retval = CALIBRATE_ERROR;

	//Calculate offset
	g_offset = (uint16_t) sum / ADC_BUF_LEN;

	return retval;
}

uint16_t current_sensor_api_get_offset() {
	return g_offset;
}

void current_sensor_api_set_period_220(uint32_t period_us) {
	g_period_AC_sig_us = period_us;
}

void current_sensor_api_dma_callback() {
	g_f_eoc_dma = true;
}

void current_sensor_api_timer_callback() {
	static state_current_sensor_t state;
	static uint32_t sample = 0;

	switch(state) {
		case STATE_RESET:
			//Checks if sampling start
			if(g_status_sam==SAMPLING_IN_PROGRESS) {
				//Start process
				sample = 0;
				current_sensor_api_start_ADC_sampling_DMA(&g_ADC_buffer[sample]);
				sample++;
				state = STATE_SAMPLING;
			}
			break;

		case STATE_SAMPLING:
			//Wait DMA notifies "Conversion Completed"
			if(g_f_eoc_dma) {
				g_f_eoc_dma = false;

				//Checks if buffer was completed
				if(sample >= ADC_BUF_LEN) {
					state = STATE_RESET;
					g_status_sam = SAMPLING_COMPLETED;
					state = STATE_RESET;
					timer_api_disable_interrupts();
					return;
				}

				//Start DMA again
				current_sensor_api_start_ADC_sampling_DMA(&g_ADC_buffer[sample]);
				sample++;
			}

			break;
	}
}

cycle_t current_sensor_api_get_average_cycle() {
	//Iterator for DMA buffer
	uint32_t dma_i=0;

	//Calculate how many samples are by cycle
	uint32_t len_cycle = g_period_AC_sig_us / SAMPLING_PERIOD_US;

	//Calculate how many cycles are in one ADC sampling process
	uint8_t cycles_num = ADC_BUF_LEN /len_cycle;

	//Structure data by cycles
	//cycle_array = [cycle1, cycle2, ... cycleN]
	cycle_t *cycle_array;
	cycle_array = (cycle_t *)calloc(cycles_num, sizeof(cycle_t));
	for(uint8_t cycle=0; cycle<cycles_num ; cycle++) {
		for(uint32_t i=0; i<len_cycle; i++) {
			cycle_array[cycle].buffer[i] = g_ADC_buffer[dma_i];
			dma_i++;
		}
	}

	//Calculate the average cycle
	cycle_t average_cycle;
	average_cycle.len = len_cycle;
	for(uint32_t i=0; i<len_cycle; i++) {
		float sum = 0;
		for(uint8_t cycle=0; cycle<cycles_num ; cycle++) {
			sum += cycle_array[cycle].buffer[i];
		}
		average_cycle.buffer[i] = (uint16_t) sum / cycles_num;
	}
	return average_cycle;
}
