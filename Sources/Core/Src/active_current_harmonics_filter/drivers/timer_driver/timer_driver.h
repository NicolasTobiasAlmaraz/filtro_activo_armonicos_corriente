/**
 * @file timer_driver.h
 * @author Nicolás Almaraz
 * @brief Timer functions and time management
 */

#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

//======================================
// Dependencies
//======================================
#include <stdbool.h>
#include <stdint.h>

//======================================
// Public Defines
//======================================

//======================================
// Public Data Structures and Types
//======================================

/**
 * @brief Timer IDs
 */
typedef enum {
    TIMER_SAMPLING,						//<! Timer for sampling
    TIMER_DISPLAY,						//<! Timer for display
    TIMER_SETTING,						//<! Timer for setting time
    TIMER_DEBOUNCE_BUTTON,				//<! Timer for debounce button
	TIMER_DEBOUNCE_CYCLES_DETECTION,	//<! Timer for debounce cycles
} timer_id_t;

/**
 * @brief Timer states
 */
typedef enum {
    TIMER_RUNNING,			//<! Running
    TIMER_FINISHED,			//<! Stopped
    TIMER_NOT_CONFIGURED,	//<! Not configured
} status_timer_t;

//======================================
// Global Variables
//======================================

//======================================
// Public Function Declarations
//======================================

/**
 * @brief Initialize the Timer API
 */
void timer_driver_init();

/**
 * @brief Check the state of a timer
 * @param id Timer ID
 * @retval Timer state
 */
status_timer_t timer_driver_check_timer(timer_id_t id);

/**
 * @brief Start a timer
 * @param id Timer ID to start
 * @param time_us Time in microseconds
 */
void timer_driver_start(timer_id_t id, uint32_t time_us);

/**
 * @brief Returns the timer ticks (1tick = 1useg)
 */
uint32_t timer_driver_get_ticks();

#endif /* SRC_MY_APP_TIMER_API_TIMER_API_H_ */
