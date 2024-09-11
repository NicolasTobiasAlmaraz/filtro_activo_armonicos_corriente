/**
 * @file timer_api.c
 * @author Nicolás Almaraz
 * @brief Timer functions and time management
 */

//======================================
// Dependencies
//======================================
#include "common_apis/timer_api/timer_api.h"
#include "main.h"

//======================================
// Private Defines
//======================================
#define TICK_TIME 	1
#define NUM_TIMERS 	5

//======================================
// Private Data Structures and Types
//======================================
typedef struct {
    uint32_t end_count;
    status_timer_t state;
} polling_timer_t;

//======================================
// STM32 Handlers
//======================================
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

//======================================
// Private Variables
//======================================
static polling_timer_t g_timers[NUM_TIMERS];
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

void timer_api_init() {
	//Flag init
	f_init = true;

	// Initialize timers
    for(uint8_t i = 0; i < NUM_TIMERS; i++)
        g_timers[i].state = TIMER_NOT_CONFIGURED;

    // Start timer
    HAL_TIM_Base_Start(&htim2);
    HAL_TIM_Base_Start_IT(&htim3);

}

status_timer_t timer_api_check_timer(timer_id_t id) {
	//Init check
	if(!f_init)
		return TIMER_NOT_CONFIGURED;

	// Verify if the timer was configured
    if(g_timers[id].state == TIMER_NOT_CONFIGURED)
        return TIMER_NOT_CONFIGURED;

    // Get current count and end time
    uint32_t tim_count = __HAL_TIM_GET_COUNTER(&htim2);
    uint32_t tim_end = g_timers[id].end_count;

    // Check for timeout
    if(tim_count >= tim_end) {
        g_timers[id].state = TIMER_FINISHED;
        g_timers[id].state = TIMER_NOT_CONFIGURED;
        return TIMER_FINISHED;
    } else {
        return TIMER_RUNNING;
    }
}

void timer_api_set_count(timer_id_t id, uint32_t time_us) {
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
    g_timers[id] = timer;
}

uint32_t timer_api_get_ticks() {
	return __HAL_TIM_GET_COUNTER(&htim2);
}

void timer_api_enable_interrupts() {
	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
}


void timer_api_disable_interrupts(){
	__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE);
}
