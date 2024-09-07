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
void display_init();

/**
 * @brief Updates the THD value on the display
 */
void display_update_THD(uint8_t thd);


#endif /* SRC_SUBSYSTEMS_DISPLAY_LOGS_H_ */
