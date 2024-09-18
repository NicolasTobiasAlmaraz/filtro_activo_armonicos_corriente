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
#include <string.h>

#include "main.h"

#include "current_sensor.h"
#include "cycle_detector.h"
#include "timer_driver.h"

//======================================
// Private Defines
//======================================
#define MAX_UINT16_t 				65535 				//!< Maximum value of a 16-bit unsigned integer
#define CALIBRATION_TOLERANCE_MA	1					//!< Calibration tolerance in milliamps
#define CURRENT_SENSIBILITY 		(float) 4.356004 	//!< Current slope in [mA/counts]
#define ADC_BUF_LEN             	40000				//!< ADC Buffer Len DMA
#define SAMPLING_PERIOD_US			50					//!< ADC SAMPLING PERIOD (Ts)
#define MIN_SAMPLES_CYCLE 375							//!< Minimum validate samples in one cycle
#define MAX_SAMPLES_CYCLE 415							//!< Maximum validate samples in one cycle

//======================================
// Private Data Structures and Types
//======================================

/**
 * @brief State machine for the current sensor
 */
typedef enum {
	STATE_RESET,		//!< Sampling completed.
	STATE_SAMPLING,		//!< Sampling in progress.
} state_current_sensor_t;

/**
 * @brief Structure to save the limits of each cycle inside the ADC buffer
 */
typedef struct {
	uint32_t index_init;	//!< First sample of the cycle
	uint32_t index_end;		//!< Last sample of the cycle
} cycle_limits_t;

/**
 * @brief Structure to save a conjunct of cycles per sampling process
 */
typedef struct {
	cycle_limits_t limits[150];	//!< Cycles array.
	uint32_t cycles_num;		//!< Number of cycles.
} handler_cycle_lmits_t;


//======================================
// STM32 Handlers
//======================================
extern ADC_HandleTypeDef hadc1;	//!< ADC handle for the current sensor (defined in main.c)

//======================================
// Private Variables
//======================================

static uint16_t m_ADC_buffer[ADC_BUF_LEN];					//!< ADC Buffer
static uint32_t m_index_ADC_buffer = 0;						//!< Index for iterate g_ADC_buffer
static uint16_t m_offset = 4096 / 2;						//!< Offset applied to the ADC readings for calibration
static uint32_t m_period_AC_sig_us = 20000; 				//!< AC period signal (In theory 50Hz --> 20000 us)
static status_sampling_t m_status_sam = SAMPLING_COMPLETED;	//!< Status of the sampling
static bool m_f_eoc_dma = false;							//!< Status of DMA Conv
static handler_cycle_lmits_t m_handler_lim;					//!< Handler to register limits of each cycle inside the complete array (up to 100 cycles)

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
static void m_start_DMA_ADC_sampling(uint16_t *ptr);

//======================================
// Private Function Implementations
//======================================
void m_start_DMA_ADC_sampling(uint16_t *ptr) {
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

void current_sensor_init() {
	//Init ADC
	HAL_ADC_Init(&hadc1);
}

void current_sensor_start_sampling() {
	m_status_sam = SAMPLING_IN_PROGRESS;
	//timer_api_enable_interrupts();
}

status_sampling_t current_sensor_get_sampling_status() {
	return m_status_sam;
}

status_calibration_t current_sensor_get_calibration_status() {
	status_calibration_t retval;
	uint64_t sum=0;
	uint16_t max_value = 0;
	uint16_t min_value = MAX_UINT16_t;

	//Process buff
	for(uint32_t i=0 ; i<ADC_BUF_LEN ; i++) {
		uint16_t raw = m_ADC_buffer[i];
		sum += raw;
		if(raw > max_value)
			max_value = raw;
		if(raw < min_value)
			min_value = raw;
	}

	//Calculate offset
	float offset = (float) sum / ADC_BUF_LEN;
	m_offset = (uint16_t)offset;

	//Calculate max variation
	float slope_ma = (max_value - min_value) * CURRENT_SENSIBILITY;
	retval = CALIBRATE_OK;
	if(slope_ma > CALIBRATION_TOLERANCE_MA)
		retval = CALIBRATE_ERROR;

	return retval;
}

uint16_t current_sensor_get_offset() {
	return m_offset;
}

void current_sensor_api_set_period_220(uint32_t period_us) {
	m_period_AC_sig_us = period_us;
}

void current_sensor_ADC_DMA_IRQHandler() {
	m_f_eoc_dma = true;
}

void current_sensor_Timer_IRQHandler() {
	static state_current_sensor_t state;

	switch(state) {
		case STATE_RESET:
			//Checks if sampling start
			if(m_status_sam==SAMPLING_IN_PROGRESS) {
				//Reset cycles handler
				m_handler_lim.cycles_num = 0;
				m_handler_lim.limits[0].index_init = 0;

				//Reset index buffer
				m_index_ADC_buffer = 0;

				//Start Sampling
				m_start_DMA_ADC_sampling(&m_ADC_buffer[m_index_ADC_buffer]);
				m_index_ADC_buffer++;
				state = STATE_SAMPLING;
			}
			break;

		case STATE_SAMPLING:
			//Wait DMA notifies "Conversion Completed"
			if(m_f_eoc_dma) {
				m_f_eoc_dma = false;

				//Checks if buffer was completed
				if(m_index_ADC_buffer >= ADC_BUF_LEN) {
					state = STATE_RESET;
					m_status_sam = SAMPLING_COMPLETED;
					state = STATE_RESET;
					//timer_api_disable_interrupts();
					return;
				}

				//Start DMA again
				m_start_DMA_ADC_sampling(&m_ADC_buffer[m_index_ADC_buffer]);
				m_index_ADC_buffer++;
			}

			break;
	}
}

void current_sensor_get_average_cycle(cycle_t *buffer) {
	//If the cycle detector don't working -> Error
	uint32_t cycles_num = m_handler_lim.cycles_num-1;
	if(cycles_num==0) {
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, true); //debug
		return;
	}

	//Structure data by cycles
	//cycle_array = [cycle1, cycle2, ... cycleN]
	cycle_t *cycle_array;
	uint16_t min_lenght = MAX_UINT16_t;
	uint16_t validated_cycles = 0;

	cycle_array = (cycle_t *)calloc(cycles_num, sizeof(cycle_t));


	//Complete cycles array
	for(uint8_t cycle=0; cycle<cycles_num ; cycle++) {

		//Calculate length and verify minimum length
		uint16_t len_cycle = m_handler_lim.limits[cycle].index_end - m_handler_lim.limits[cycle].index_init;

		//Discard if the cycle is too short or too long
		if(len_cycle<MIN_SAMPLES_CYCLE || len_cycle>MAX_SAMPLES_CYCLE) {
			continue;
		}

		if(len_cycle<min_lenght)
			min_lenght = len_cycle;

		//Complete cycle
		cycle_array[validated_cycles].len = len_cycle;
		uint32_t index_init = m_handler_lim.limits[cycle].index_init;
		for(uint32_t i=0; i<len_cycle; i++) {
			cycle_array[validated_cycles].buffer[i] = m_ADC_buffer[index_init+i];
		}
		validated_cycles++;
	}

	//Calculate the average cycle
	cycle_t average_cycle;
	average_cycle.len = min_lenght;
	for(uint32_t i=0; i<min_lenght; i++) {
		uint64_t sum = 0;
		for(uint8_t cycle=0; cycle<validated_cycles ; cycle++) {
			sum += cycle_array[cycle].buffer[i];
		}
		sum /= validated_cycles;
		average_cycle.buffer[i] = (uint16_t) sum;
	}

	//Copy on ptr_out
	memcpy(buffer, &average_cycle, sizeof(cycle_t));

	//Free memory
	free(cycle_array);
}

void current_sensor_api_set_new_cycle() {
	//Check if corresponds to analyze
	if(m_status_sam == SAMPLING_COMPLETED) {
		return;
	}

	// Read current cycle
	uint32_t current_cycle = m_handler_lim.cycles_num;

	// Register the end of the current cycle and the start of the next
	m_handler_lim.limits[current_cycle].index_end = m_index_ADC_buffer-1;
	m_handler_lim.limits[current_cycle+1].index_init = m_index_ADC_buffer;

	//Increment one cycle
	m_handler_lim.cycles_num++;
}
