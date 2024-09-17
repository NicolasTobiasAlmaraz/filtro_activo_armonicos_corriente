/**
 * @file push_button_driver.c
 * @author Nicolás Almaraz
 * @brief User Push Button Processing
 */

//======================================
// Dependencies
//======================================
#include "active_current_harmonics_filter.h"
#include "timer_driver.h"

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
void user_button_GPIO_IRQHandler() {
	//Check debounce timeout
	status_timer_t timeout = timer_api_check_timer(TIMER_DEBOUNCE_BUTTON);

	//If timer was running, it means a bounce was detected
	if(timeout == TIMER_RUNNING)
		return;

	timer_api_set_count(TIMER_DEBOUNCE_BUTTON, TIME_DEBOUNCE);
	my_system_set_user_button();
}
