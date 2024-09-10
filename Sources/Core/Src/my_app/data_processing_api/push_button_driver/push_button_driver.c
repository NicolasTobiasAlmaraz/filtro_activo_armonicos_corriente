/**
 * @file push_button_driver.c
 * @author Nicolás Almaraz
 * @brief User Push Button Processing
 */

//======================================
// Dependencies
//======================================
#include "common_apis/timer_api/timer_api.h"
#include "data_processing_api/data_processing_api.h"

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

void push_button_driver_init() {

}

void push_button_driver_callback() {
	//Check debounce timeout
	status_timer_t timeout = timer_api_check_timer(TIMER_DEBOUNCE_BUTTON);

	//If timer was running, it means a bounce was detected
	if(timeout == TIMER_RUNNING)
		return;

	timer_api_set_count(TIMER_DEBOUNCE_BUTTON, TIME_DEBOUNCE);
	data_processing_api_set_edge_button();
}
