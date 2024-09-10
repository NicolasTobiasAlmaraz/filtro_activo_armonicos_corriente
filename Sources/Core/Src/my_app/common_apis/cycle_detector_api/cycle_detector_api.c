/**
 * @file zero_cross.c
 * @author Nicolás Almaraz
 * @brief Zero-cross detection handling
 */

//======================================
// Dependencies
//======================================
#include <app_inject_simulator/inject_simulator_api.h>
#include "cycle_detector_api.h"

#include "common_apis/timer_api/timer_api.h"

#include "current_sensor_api/current_sensor_api.h"

//======================================
// Private Defines
//======================================
#define TIME_CYCLE_DETECTION_US 15000 //15ms

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

void cycle_detector_api_init() {

}

void cycle_detector_api_callback() {
	//Check debounce timeout
	status_timer_t timeout = timer_api_check_timer(TIMER_DEBOUNCE_CYCLES_DETECTION);

	//If timer was running, it means a bounce was detected
	if(timeout == TIMER_RUNNING)
		return;

	timer_api_set_count(TIMER_DEBOUNCE_CYCLES_DETECTION, TIME_CYCLE_DETECTION_US);

	//Notify the begin of a new cycle
	//Current Sensor API
	current_sensor_api_set_new_cycle();

	//Inject Simulator API
	inject_simulator_api_set_new_cycle();
}
