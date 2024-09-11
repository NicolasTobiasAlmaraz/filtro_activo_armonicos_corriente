/**
 * @file data_processing_api.c
 * @author Nicolás Almaraz
 * @brief This API resolves the data processing
 */

//======================================
// Dependencies
//======================================
#include <app_inject_simulator/inject_simulator_api.h>
#include <app_processing/app_processing.h>
#include <app_processing/current_sensor_api/current_sensor_api.h>
#include <app_processing/display_api/display_api.h>
#include <app_processing/signal_analyzer_api/signal_analyzer_api.h>
#include <stdbool.h>

#include "main.h"
#include "my_app.h"

#include "common_apis/cycle_detector_api/cycle_detector_api.h"
#include "common_apis/timer_api/timer_api.h"

#include "signal_analyzer_api/signal_analyzer_api.h"

//======================================
// Private Defines
//======================================

//======================================
// Private Data Structures and Types
//======================================
typedef enum {
	STATE_RESET,
	STATE_CALIBRATING,
	STATE_STAND_BY,
	STATE_SAMPLING,
	STATE_PROCESSING,
	STATE_WAITING_SETTING_TIME,
}app_state_t;

//======================================
// Private Variables
//======================================
static app_state_t g_state = STATE_RESET;
static bool g_f_edge_button = false;
static uint16_t g_zero_offset;

//======================================
// Private Function Declarations
//======================================

/**
 * @brief Checks if the user pressed the stop button
 */
static void app_processing_check_stop_button();

//======================================
// Private Function Implementations
//======================================

void app_processing_check_stop_button() {
	if(g_f_edge_button) {
		g_f_edge_button = false;
		g_state = STATE_STAND_BY;

		//Disable Injection
		inject_simulator_api_set_enable(false);

		//Set display "Stand By Mode"
		display_api_set_msg_stand_by();
	}
}

//======================================
// Public Function Implementations
//======================================

void app_processing_init() {
	//Display Init
	display_api_init();

	//Display with inital msg
	display_api_set_msg_start_calibration();

	//Disable current injection until the user configures the system
	inject_simulator_api_set_enable(false);
}

void app_processing_loop() {
	switch (g_state) {
		default:
			g_state = RESET;
			break;

		case STATE_RESET:
			if(g_f_edge_button) {
				g_f_edge_button = false;
				g_state = STATE_CALIBRATING;

				//Update Display -- "Calibrating ..."
				display_api_set_msg_calibrating();

				//Start ADC conversion
				current_sensor_api_start_sampling();
			}
			break;

		case STATE_CALIBRATING:
			//Checks EOC ADC
			if( current_sensor_api_get_status()!=SAMPLING_COMPLETED )
				break;

			//Get Calibration
			status_calibration_t status = current_sensor_api_get_calibration();

			status = CALIBRATE_OK; //todo Arreglar ruido eléctrico del sensor

			if(status == CALIBRATE_OK) {
				//Save offset, inform user by display to continue with the process
				g_state = STATE_STAND_BY;
				g_zero_offset = current_sensor_api_get_offset();
				display_api_set_msg_calibration_ok();
			}
			else {
				//Come back to rest state and warn user by display "Calibration error, try again"
				g_state = STATE_RESET;
				display_api_set_msg_calibration_error();
			}
			break;

		case STATE_STAND_BY:
			if(g_f_edge_button) {
				g_f_edge_button = false;
				g_state = STATE_SAMPLING;
				display_api_set_msg_THD();
				current_sensor_api_start_sampling();
			}
			break;

		case STATE_SAMPLING:
			//Check stop Button
			app_processing_check_stop_button();

			//Wait conversion time
			if(current_sensor_api_get_status()==SAMPLING_COMPLETED) {
				//Process the signal
				g_state = STATE_PROCESSING;
				cycle_t average_cycle;
				current_sensor_api_get_average_cycle(&average_cycle);
				signal_analyzer_api_set_signal_to_analyze(average_cycle, g_zero_offset); //Llega mal el avg cycle
			}
			break;

		case STATE_PROCESSING:
			//Check stop Button
			app_processing_check_stop_button();

			//Processing
			status_processing_t status_proc = signal_analyzer_api_analyze_loop();
			if(status_proc!=PROCESSING_COMPLETED)
				break;

			//Update display
			uint8_t thd = signal_analyzer_api_get_thd();
			display_api_update_THD(thd);

			//Update DAC signal
			cycle_t sig_to_inject = signal_analyzer_api_get_cycle_to_inject();
			inject_simulator_api_set_current_waveform(sig_to_inject);

			//Start timer to wait setting time
			g_state = STATE_WAITING_SETTING_TIME;
			timer_api_set_count(TIMER_SETTING, SETTING_TIME_US);
			break;

		case STATE_WAITING_SETTING_TIME:
			//Check stop Button
			app_processing_check_stop_button();

			//Check setting time
			if( timer_api_check_timer(TIMER_SETTING)!= TIMER_FINISHED)
				return;

			//Repeat the process
			current_sensor_api_start_sampling();
			g_state = STATE_SAMPLING;
			break;
	}
}

void data_processing_api_set_edge_button() {
	g_f_edge_button = true;
}
