/**
 * @file current_sensor.h
 * @author Nicolás Almaraz
 * @brief Implements the functionalities for current sensor reading
 */

#ifndef CURRENT_SENSOR_H_
#define CURRENT_SENSOR_H_

//======================================
// Dependencies
//======================================
#include <stdbool.h>
#include <stdint.h>

#include "active_current_harmonics_filter.h"

//======================================
// Public Defines
//======================================

//======================================
// Public Data Structures and Types
//======================================

/**
 * @brief Status code of the calibration process.
 */
typedef enum {
	CALIBRATE_OK,			//!< Calibration successful.
	CALIBRATE_ERROR,		//!< Calibration error.
}status_calibration_t;

/**
 * @brief Status code of the sampling process.
 */
typedef enum {
	SAMPLING_IN_PROGRESS,	//!< Sampling in progress
	SAMPLING_COMPLETED,		//!< Samples ready for processing
}status_sampling_t;

//======================================
// Public Function Declarations
//======================================

/**
 * @brief Initializes the current measurements API.
 */
void current_sensor_init();

/**
 * @brief Starts a new ADC conversion using the DMA Controller every 50 microseconds.
 * @note Check the end of conversion with the `current_sensor_get_status()` function.
 */
void current_sensor_start_sampling();

/**
 * @brief Returns the status of the sampling (in progress or completed)
 * @retval status Status of the sampling process.
 */
status_sampling_t current_sensor_get_sampling_status();

/**
 * @brief Evaluates a correct zero offset value in base of the last DMA Transfer samples.
 * @retval status Status of the calibration process.
 */
status_calibration_t current_sensor_get_calibration_status();

/**
 * @brief Returns the calibration offset for the sensor.
 * @retval offset Calibration offset value.
 */
uint16_t current_sensor_get_offset();

/**
 * @brief Returns the average cycle in base of the last DMA Transfer samples.
 * @retval cycle Average cycle.
 */
void current_sensor_get_average_cycle(cycle_t *buffer);

/**
 * @brief This function must be called on Timer Interrupt Callback.
 */
void current_sensor_Timer_IRQHandler();

/**
 * @brief This function must be called on ADC DMA Conversion Complete Callback
 */
void current_sensor_ADC_DMA_IRQHandler();

/**
 * @brief This function must be called when a new Cycle starts
 */
void current_sensor_api_set_new_cycle();

#endif
