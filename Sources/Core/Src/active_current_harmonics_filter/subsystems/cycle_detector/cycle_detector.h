/**
 * @file zero_cross.h
 * @author Nicolás Almaraz
 * @brief Zero-cross detection handling
 */

#ifndef CYCLE_DETECTOR_H_
#define CYCLE_DETECTOR_H_

//======================================
// Dependencies
//======================================

//======================================
// Public Defines
//======================================

//======================================
// Public Data Structures and Types
//======================================

//======================================
// Global Variables
//======================================

//======================================
// Public Function Declarations
//======================================

/**
 * @brief Initializes zero-cross detection
 */
void cycle_detector_api_init();

/**
 * @brief Calculates the average period of 220V 50Hz line
 * retval period_us
 */
uint32_t cycle_detector_api_get_period();

/**
 * @brief This function must be called on cycle detector interrupt
 */
void cycle_detector_GPIO_IRQHandler();

#endif /* SRC_MY_APP_CYCLE_DETECTOR_ZERO_CROSS_H_ */
