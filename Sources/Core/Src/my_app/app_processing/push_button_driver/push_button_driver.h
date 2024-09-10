/**
 * @file push_button.h
 * @author Nicolás Almaraz
 * @brief User Push Button Processing
 */

#ifndef SRC_MY_APP_PUSH_BUTTON_PUSH_BUTTON_DRIVER_H_
#define SRC_MY_APP_PUSH_BUTTON_PUSH_BUTTON_DRIVER_H_
//======================================
// Dependencies
//======================================

//======================================
// Public Defines
//======================================

//======================================
// Public Structures and Data Types
//======================================

//======================================
// Global Variables
//======================================

//======================================
// Public Function Declarations
//======================================

/**
 * @biref Init driver
 */
void push_button_driver_init();

/**
 * @biref Function Callback
 * This callback must be called in the button interrupt
 */
void push_button_driver_callback();

#endif /* SRC_MY_APP_PUSH_BUTTON_PUSH_BUTTON_DRIVER_H_ */
