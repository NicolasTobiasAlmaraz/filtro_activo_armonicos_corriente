/**
 * @file zero_cross.c
 * @author Nicolás Almaraz
 * @brief Zero-cross detection handling
 */

//======================================
// Dependencies
//======================================
#include "main.h"

#include <cycle_detector/cycle_detector_api.h>

#include "inject_simulator/inject_simulator_api.h"
#include "current_sensor/current_sensor_api.h"

//======================================
// Private Defines
//======================================

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
	//Notify the begin of a new cycle

	//Current Sensor API
	current_sensor_api_set_new_cycle();

	//Inject Simulator API
	inject_simulator_api_set_new_cycle();
}
