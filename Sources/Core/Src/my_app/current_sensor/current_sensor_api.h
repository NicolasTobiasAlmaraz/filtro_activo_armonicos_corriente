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
#define CYCLES              50
#define LEN_MAX             450
#define CALIBRATE_OK        0
#define CALIBRATE_ERROR     1

//======================================
// Public Data Structures and Types
//======================================
typedef struct {
    uint16_t cycle[LEN_MAX];
    uint32_t len;
} cycle_t;

//======================================
// Public Function Declarations
//======================================

/**
 * @brief Initializes the Measurement API
 */
void current_sensor_api_init();

/**
 * @brief Measures the offset of the signal when there is no current and saves the value for future conversions
 *
 * Measures the offset of the signal when there is no current.
 * It is considered that there is current consumption when there is a variation greater than TOLERANCE_CALIBRATION_MA mA.
 * Internally, it saves the value in the API to calibrate future measurements.
 *
 * Returns:
 *  CALIBRATE_OK: Successful calibration, the offset was saved in the pointer.
 *  CALIBRATE_ERROR: Current consumption was detected during calibration.
 * @retval status
 */
bool current_sensor_api_calibrate();

/**
 * @brief Main sampling loop for the current sensor
 * @retval true if an end of cycle (EOC) is reached, false otherwise
 */
bool current_sensor_api_sampling_loop();

/**
 * @brief Clears all current samples and resets the state machine
 */
void current_sensor_api_clean_samples();

/**
 * @brief Copies the collected samples to an output buffer
 * @param ptr_out Pointer to the output buffer
 */
void current_sensor_api_get_samples(cycle_t *ptr_out);

/**
 * @brief Indicates to the API that a new line cycle has started.
 *
 * This function notifies the API that a new line cycle has started.
 * It is recommended to use this function in the GPIO cycle detection interrupt callback.
 */
void current_sensor_api_set_new_cycle();

/**
 * @brief Returns the calibration offset for the sensor
 * @retval Calibration offset
 */
uint16_t current_sensor_api_get_offset();

#endif /* SRC_MY_APP_CURRENT_SENSOR_CURRENT_SENSOR_H_ */
