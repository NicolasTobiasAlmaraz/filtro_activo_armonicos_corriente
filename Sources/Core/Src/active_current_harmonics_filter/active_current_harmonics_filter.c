/**
 * @file data_processing_api.c
 * @author Nicolás Almaraz
 * @brief This API resolves the data processing
 */

//======================================
// Dependencies
//======================================
#include <stdbool.h>

#include "main.h"

#include "inject_simulator.h"
#include "current_sensor.h"
#include "display.h"
#include "signal_analyzer.h"
#include "cycle_detector.h"
#include "timer_driver.h"

//======================================
// Private Defines
//======================================

//======================================
// Private Data Structures and Types
//======================================

/**
 * @brief Enumeration representing the different states of the application.
 */
typedef enum {
    STATE_RESET,               //!< Reset state.
    STATE_CALIBRATING,         //!< Calibrating state.
    STATE_STAND_BY,            //!< Stand-by mode.
    STATE_SAMPLING,            //!< Sampling data (Working mode).
    STATE_PROCESSING,          //!< Processing the collected data (Working mode).
    STATE_WAITING_SETTING_TIME //!< Waiting for the setting (Working mode).
} app_state_t;


//======================================
// Private Variables
//======================================
static app_state_t m_state = STATE_RESET;
static bool m_f_edge_button = false;
static uint16_t m_zero_offset;

//======================================
// Private Function Declarations
//======================================

/**
 * @brief Checks if the user pressed the stop button
 */
static void m_check_stop_button();

//======================================
// Private Function Implementations
//======================================

void m_check_stop_button() {
	if(m_f_edge_button) {
		m_f_edge_button = false;
		m_state = STATE_STAND_BY;

		//Disable Injection
		inject_simulator_set_enable(false);

		//Set display "Stand By Mode"
		display_set_msg_stand_by();
	}
}

//======================================
// Public Function Implementations
//======================================

void my_system_init() {
	//Init Timer
	timer_api_init();

	//Init Cycle detector
	cycle_detector_init();

	//Init current sensor
	current_sensor_init();

	//Init inject simulator
	inject_simulator_init();
	inject_simulator_set_enable(false); //Force disabled

	//Display Init
	display_init();
	display_set_msg_start_calibration();
}

void my_system_state_machine() {
	switch (m_state) {
		default:
			m_state = RESET;
			break;

		case STATE_RESET:
			if(m_f_edge_button) {
				m_f_edge_button = false;
				m_state = STATE_CALIBRATING;

				//Update Display -- "Calibrating ..."
				display_set_msg_calibrating();

				//Start ADC conversion
				current_sensor_start_sampling();
			}
			break;

		case STATE_CALIBRATING:
			//Checks EOC ADC
			if( current_sensor_get_sampling_status()!=SAMPLING_COMPLETED )
				break;

			//Get Calibration
			status_calibration_t status = current_sensor_get_calibration_status();

			status = CALIBRATE_OK; //todo Arreglar ruido eléctrico del sensor

			if(status == CALIBRATE_OK) {
				//Save offset, inform user by display to continue with the process
				m_state = STATE_STAND_BY;
				m_zero_offset = current_sensor_get_offset();
				display_set_msg_calibration_ok();
			}
			else {
				//Come back to rest state and warn user by display "Calibration error, try again"
				m_state = STATE_RESET;
				display_set_msg_calibration_error();
			}
			break;

		case STATE_STAND_BY:
			if(m_f_edge_button) {
				m_f_edge_button = false;
				m_state = STATE_SAMPLING;
				display_set_template_working();
				current_sensor_start_sampling();
				inject_simulator_set_enable(true);
			}
			break;

		case STATE_SAMPLING:
			//Check stop Button
			m_check_stop_button();

			//Wait conversion time
			if(current_sensor_get_sampling_status()==SAMPLING_COMPLETED) {
				//Process the signal
				m_state = STATE_PROCESSING;
				cycle_t average_cycle;
				current_sensor_get_average_cycle(&average_cycle);
				signal_analyzer_set_signal_to_analyze(average_cycle, m_zero_offset); //Llega mal el avg cycle
			}
			break;

		case STATE_PROCESSING:
			//Check stop Button
			m_check_stop_button();

			//Processing
			status_processing_t status_proc = signal_analyzer_state_machine();
			if(status_proc!=PROCESSING_COMPLETED)
				break;

			//Update display
			uint16_t thd = signal_analyzer_get_thd();
			display_update_THD(thd);

			//Update DAC signal
			cycle_t sig_to_inject = signal_analyzer_get_cycle_to_inject();
			inject_simulator_set_waveform(sig_to_inject);

			//Start timer to wait setting time
			m_state = STATE_WAITING_SETTING_TIME;
			timer_api_set_count(TIMER_SETTING, SETTING_TIME_US);
			break;

		case STATE_WAITING_SETTING_TIME:
			//Check stop Button
			m_check_stop_button();

			//Check setting time
			if( timer_api_check_timer(TIMER_SETTING)!= TIMER_FINISHED)
				return;

			//Repeat the process
			current_sensor_start_sampling();
			m_state = STATE_SAMPLING;
			break;
	}
}

void my_system_set_user_button() {
	m_f_edge_button = true;
}
