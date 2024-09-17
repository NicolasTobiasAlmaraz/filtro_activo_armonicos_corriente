/**
 * @file timer_driver.c
 * @author Nicolás Almaraz
 * @brief Timer functions and time management
 */

//======================================
// Dependencies
//======================================
#include "main.h"

#include "timer_driver.h"

//======================================
// Private Defines
//======================================
#define TICK_TIME 	1 //!< Ticks for 1 us
#define NUM_TIMERS 	5 //!< Number of timers

//======================================
// Private Data Structures and Types
//======================================

/**
 * @brief Handler of polling timers
 */
typedef struct {
    uint32_t end_count;		//!< Count for timeout
    status_timer_t state;	//!< State of timer
} polling_timer_t;

//======================================
// STM32 Handlers
//======================================
extern TIM_HandleTypeDef htim2;	//!< Timer 2 Handler (timer by polling)
extern TIM_HandleTypeDef htim3; //!< Timer 3 Handler (timer by interrupt)

//======================================
// Private Variables
//======================================
static polling_timer_t m_timers[NUM_TIMERS];
static bool f_init = false;

//======================================
// Private Function Declarations
//======================================

//======================================
// Private Function Implementations
//======================================

//======================================
// Public Function Implementations
//======================================

void timer_driver_init() {
	//Flag init
	f_init = true;

	// Initialize timers
    for(uint8_t i = 0; i < NUM_TIMERS; i++)
        m_timers[i].state = TIMER_NOT_CONFIGURED;

    // Start timer
    HAL_TIM_Base_Start(&htim2);
    HAL_TIM_Base_Start_IT(&htim3);

}

status_timer_t timer_driver_check_timer(timer_id_t id) {
	//Init check
	if(!f_init)
		return TIMER_NOT_CONFIGURED;

	// Verify if the timer was configured
    if(m_timers[id].state == TIMER_NOT_CONFIGURED)
        return TIMER_NOT_CONFIGURED;

    // Get current count and end time
    uint32_t tim_count = __HAL_TIM_GET_COUNTER(&htim2);
    uint32_t tim_end = m_timers[id].end_count;

    // Check for timeout
    if(tim_count >= tim_end) {
        m_timers[id].state = TIMER_FINISHED;
        m_timers[id].state = TIMER_NOT_CONFIGURED;
        return TIMER_FINISHED;
    } else {
        return TIMER_RUNNING;
    }
}

void timer_driver_start(timer_id_t id, uint32_t time_us) {
	//Init check
	if(!f_init)
		return;

	// Get the current timer count
    uint32_t tim_count = __HAL_TIM_GET_COUNTER(&htim2);

    // Set the end time and status
    polling_timer_t timer;
    timer.end_count = tim_count + time_us / TICK_TIME;
    timer.state = TIMER_RUNNING;

    // Assign the timer to the global array
    m_timers[id] = timer;
}

uint32_t timer_driver_get_ticks() {
	return __HAL_TIM_GET_COUNTER(&htim2);
}
