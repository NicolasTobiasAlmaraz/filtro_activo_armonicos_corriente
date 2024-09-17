/**
 * @file push_button.h
 * @author Nicolás Almaraz
 * @brief User Push Button Processing
 */

#ifndef PUSH_BUTTON_H_
#define PUSH_BUTTON_H_
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
 * @biref Function Callback
 * This callback must be called in the button interrupt
 */
void user_button_GPIO_IRQHandler();

#endif /* SRC_MY_APP_PUSH_BUTTON_PUSH_BUTTON_DRIVER_H_ */
