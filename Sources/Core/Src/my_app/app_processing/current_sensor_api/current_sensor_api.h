/**
 * @file current_sensor.h
 * @author Nicolás Almaraz
 * @brief Implements the functionalities for current sensor reading
 */

#ifndef SRC_MY_APP_CURRENT_SENSOR_CURRENT_SENSOR_H_
#define SRC_MY_APP_CURRENT_SENSOR_CURRENT_SENSOR_H_

//======================================
// Dependencies
//======================================
#include <stdbool.h>
#include <stdint.h>

//======================================
// Public Defines
//======================================
#define LEN_MAX             450

//======================================
// Public Data Structures and Types
//======================================

/**
 * @brief This structure contains current samples for a 50Hz cycle
 */
typedef struct {
    uint16_t cycle[LEN_MAX];	//!< Samples
    uint32_t len;				//!< Number of Samples
} cycle_t;

/**
 * @brief Status code of the calibration process
 */
typedef enum {
	CALIBRATE_OK,			//!<Calibration successful
	CALIBRATE_ERROR,		//!<Calibration error
}status_calibration_t;

/**
 * @brief Status code of the sampling process
 */
typedef enum {
	SAMPLING_IN_PROGRESS,	//!< Sampling in progress
	SAMPLING_COMPLETED,		//!< Samples ready for processing
}status_sampling_t;

//======================================
// Public Function Declarations
//======================================

/**
 * @brief Initializes the Measurement API
 */
void current_sensor_api_init();

/**
 * @brief Starts a new ADC conversion using DMA Controller
 */
void current_sensor_api_start_sampling();

/**
 * @brief Returns the status of the sampling
 * @retval status
 */
status_sampling_t current_sensor_api_get_status();

/**
 * @brief Evaluates a correct zero offset value in base of the last DMA Transfer samples
 * @retval status
 */
status_calibration_t current_sensor_api_get_calibration();

/**
 * @brief Returns the calibration offset for the sensor
 * @retval Calibration offset
 */
uint16_t current_sensor_api_get_offset();

/**
 * @brief Set the 220V period time (theoretically 50Hz -> 20000 us)
 * @param period_us period in microseconds
 */
void current_sensor_api_set_period_220(uint32_t period_us);

/**
 * @brief Returns the average cycle in base of the last DMA Transfer samples
 * @retval average cycle
 */
cycle_t current_sensor_api_get_average_cycle();

/**
 * @brief This function must be called on the DMA Complete Callback
 */
void current_sensor_api_DMA_cplt_callback();

#endif /* SRC_MY_APP_CURRENT_SENSOR_CURRENT_SENSOR_H_ */
