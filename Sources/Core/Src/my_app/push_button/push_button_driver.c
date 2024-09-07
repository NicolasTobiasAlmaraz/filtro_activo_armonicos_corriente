/**
 * @file push_button_driver.c
 * @author Nicolás Almaraz
 * @brief User Push Button Processing
 */

//======================================
// Dependencies
//======================================
#include "main.h"
#include "timer_api/timer_api.h"

//======================================
// Private Defines
//======================================
#define TIME_DEBOUNCE 30000 //30ms

//======================================
// Private Data Structures and Types
//======================================

//======================================
// Private Variables
//======================================

//======================================
// Private Function Declarations
//======================================

//======================================
// Private Function Implementations
//======================================

//======================================
// Public Function Implementations
//======================================

/**
 * @biref Init driver
 */
void push_button_driver_init() {

}

/**
 * @biref Callback
 */
void push_button_driver_callback() {
	//Check debounce timeout
	timer_state_t timeout = timer_api_check_timer(TIMER_DEBOUNCE_BUTTON);
	if(timeout != TIMER_RUNNING) {
		timer_api_set_count(TIMER_DEBOUNCE_BUTTON, TIME_DEBOUNCE);
		my_app_set_edge_button();
	}
}
