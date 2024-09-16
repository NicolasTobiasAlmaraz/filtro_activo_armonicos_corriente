/**
 * @file timer_api.h
 * @author Nicolás Almaraz
 * @brief Timer functions and time management
 */

#ifndef SRC_MY_APP_TIMER_API_TIMER_API_H_
#define SRC_MY_APP_TIMER_API_TIMER_API_H_

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
    TIMER_SAMPLING,
    TIMER_DISPLAY,
    TIMER_SETTING,
    TIMER_DEBOUNCE_BUTTON,
	TIMER_DEBOUNCE_CYCLES_DETECTION,
} timer_id_t;

/**
 * @brief Timer states
 */
typedef enum {
    TIMER_RUNNING,
    TIMER_FINISHED,
    TIMER_NOT_CONFIGURED,
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
void timer_api_init();

/**
 * @brief Check the state of a timer
 * @param id Timer ID
 * @retval Timer state
 */
status_timer_t timer_api_check_timer(timer_id_t id);

/**
 * @brief Start a timer
 * @param id Timer ID to start
 * @param time_us Time in microseconds
 */
void timer_api_set_count(timer_id_t id, uint32_t time_us);

/**
 * @brief Returns the timer ticks (1tick = 1useg)
 */
uint32_t timer_api_get_ticks();

/**
 * @brief Enables timer interrupt
 */
void timer_api_enable_interrupts();

/**
 * @brief Disable timer interrupt
 */
void timer_api_disable_interrupts();

#endif /* SRC_MY_APP_TIMER_API_TIMER_API_H_ */
