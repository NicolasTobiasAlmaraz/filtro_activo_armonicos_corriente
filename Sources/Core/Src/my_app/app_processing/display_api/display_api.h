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
 * @brief Initializes the display (peripherals and static messages)
 */
void display_api_init();

/**
 * @brief Sets information about THD
 */
void display_api_set_msg_THD();

/**
 * @brief Updates only THD value
 */
void display_api_update_THD(uint8_t thd);

/**
 * @brief Show instructions about calibration
 */
void display_api_set_msg_calibrate();

/**
 * @brief Show instructions about start the working
 */
void display_api_set_msg_start();

/**
 * @brief Show: Calibrating...
 */
void display_api_set_msg_calibrating();

/**
 * @brief Show: "Calibration Error! Try again"
 */
void display_api_set_msg_calibration_error();

#endif /* SRC_SUBSYSTEMS_DISPLAY_LOGS_H_ */
