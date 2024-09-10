/**
 * @file display.h
 * @author Nicolás Almaraz
 * @brief Display update with user messages
 */

#ifndef SRC_SUBSYSTEMS_DISPLAY_LOGS_H_
#define SRC_SUBSYSTEMS_DISPLAY_LOGS_H_

//======================================
// Dependencies
//======================================
#include <stdbool.h>
#include "main.h"

//======================================
// Public Defines
//======================================

//======================================
// Public Structures and Data Types
//======================================

//======================================
// Public Function Declarations
//======================================

/**
 * @brief Initializes the display (peripherals and initial messages)
 */
void display_api_init();

/**
 * @brief Show instructions about calibration
 */
void display_api_set_msg_start_calibration();

/**
 * @brief Show: Calibrating...
 */
void display_api_set_msg_calibrating();

/**
 * @brief Show instructions about start the process
 */
void display_api_set_msg_calibration_ok();

/**
 * @brief Show instructions about calibration error
 */
void display_api_set_msg_calibration_error();

/**
 * @brief Sets information about THD and working mode
 */
void display_api_set_msg_THD();

/**
 * @brief Updates only THD value
 */
void display_api_update_THD(uint8_t thd);

/**
 * @brief Set stand by msg
 */
void display_api_set_msg_stand_by();

#endif /* SRC_SUBSYSTEMS_DISPLAY_LOGS_H_ */
