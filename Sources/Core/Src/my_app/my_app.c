/**
 * @file my_app.c
 * @author Nicolás Almaraz
 * @brief This is API starts the app
 */

//======================================
// Dependencies
//======================================
#include "my_app.h"

//Input
#include "current_sensor_api/current_sensor_api.h"

//Proccessing
#include "data_processing_api/data_processing_api.h"

//Output
#include "inject_simulator_api/inject_simulator_api.h"

//Common
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
	data_processing_api_init();
	inject_simulator_api_init();
}

void my_app_loop() {
	//Current sensor state machine
	current_sensor_api_loop();

	//Processing state machine
	data_processing_api_loop();

	//Inject state machine
	inject_simulator_api_loop();
}
