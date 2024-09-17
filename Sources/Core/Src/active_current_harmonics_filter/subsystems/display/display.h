/**
 * @file display.h
 * @author Nicolás Almaraz
 * @brief Display handler functions.
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

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
 * @brief Initializes the display (peripherals and initial messages).
 */
void display_init();

/**
 * @brief Show instructions about calibration.
 */
void display_set_msg_start_calibration();

/**
 * @brief Show: "Calibrating...".
 */
void display_set_msg_calibrating();

/**
 * @brief Show instructions about start the process.
 */
void display_set_msg_calibration_ok();

/**
 * @brief Show instructions about calibration error.
 */
void display_set_msg_calibration_error();

/**
 * @brief Sets the working mode template.
 */
void display_set_template_working();

/**
 * @brief Updates THD value in working template.
 */
void display_update_THD(uint16_t thd);

/**
 * @brief Sets stand by mode messages.
 */
void display_set_msg_stand_by();

#endif /* SRC_SUBSYSTEMS_DISPLAY_LOGS_H_ */
