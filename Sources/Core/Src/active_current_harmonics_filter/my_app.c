/**
 * @file my_app.c
 * @author Nicolás Almaraz
 * @brief This is API starts the app
 */

//======================================
// Dependencies
//======================================
#include <app_inject_simulator/inject_simulator_api.h>
#include <app_processing/app_processing.h>
#include <app_processing/current_sensor_api/current_sensor_api.h>
#include "main.h"
#include "my_app.h"

//Input
#include "common_apis/timer_api/timer_api.h"
#include "common_apis/cycle_detector_api/cycle_detector_api.h"

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

void my_app_init() {
	timer_api_init();
	cycle_detector_api_init();
	current_sensor_api_init();
	app_processing_init();
	inject_simulator_api_init();
}

void my_app_loop() {
	//Processing state machine
	app_processing_loop();

	//Inject state machine
	// For now, this function is empty because there is no power stage yet.
	// Once added, the PWM signal duty cycle values should be updated here.
	app_inject_simulator_loop();
}
