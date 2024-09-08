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

#include "common_apis/cycle_detector_api/cycle_detector_api.h"
#include "common_apis/timer_api/timer_api.h"

#include "current_sensor_api/current_sensor_api.h"
#include "inject_simulator_api/inject_simulator_api.h"
#include "data_processing_api.h"

#include "signal_analyzer_api/signal_analyzer_api.h"
#include "display_api/display_api.h"

//======================================
// Private Defines
//======================================
#define SETTING_TIME_US 2000000 //2 seg

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
static cycle_t g_cycles_array[CYCLES];
static uint16_t g_zero_offset;

//======================================
// Private Function Declarations
//======================================

/**
 * @brief "reset" state of the state machine
 */
static void data_processing_api_state_reset();

/**
 * @brief "calibrating" state of the state machine
 */
static void data_processing_api_state_calibrating();

/**
 * @brief "stand-by" state of the state machine
 */
static void data_processing_api_state_stand_by();

/**
 * @brief "sampling" state of the state machine
 */
static void data_processing_api_state_sampling();

/**
 * @brief "processing" state of the state machine
 */
static void data_processing_api_state_processing();

/**
 * @brief "waiting setting time" state of the state machine
 */
static void data_processing_api_state_waiting_setting_time();

/**
 * @brief Checks if the user pressed the stop button
 */
static void data_processing_api_check_stop_button();

//======================================
// Private Function Implementations
//======================================
void data_processing_api_state_reset() {
	if(g_f_edge_button) {
		g_f_edge_button = false;
		g_state = STATE_CALIBRATING;
		display_api_set_msg_calibrating();
	}
}

void data_processing_api_state_calibrating() {
	bool status = current_sensor_api_calibrate();
	if(status) {
		g_state = STATE_STAND_BY;
		g_zero_offset = current_sensor_api_get_offset();
		display_api_set_msg_start();
	}
	else {
		g_state = STATE_RESET;
		display_api_set_msg_calibration_error();
	}
}

void data_processing_api_state_stand_by() {
	if(g_f_edge_button) {
		g_f_edge_button = false;
		g_state = STATE_SAMPLING;
		current_sensor_api_clean_samples();
		display_api_set_msg_THD();
	}
}

void data_processing_api_state_sampling() {
	//Takes samples
	status_sampling_t status_s = current_sensor_api_get_status();

	//End Of Conversion
	if(status_s == SAMPLING_COMPLETED) {
		g_state = STATE_PROCESSING;
		current_sensor_api_get_samples(g_cycles_array);
	}

	//Stop button
	data_processing_api_check_stop_button();
}

void data_processing_api_state_processing() {

	//Process samples
	status_processing_t p_status;
	p_status = signal_analyzer_api_analyze_loop(g_cycles_array, CYCLES, g_zero_offset);

	//Stop Button
	data_processing_api_check_stop_button();

	//End of processing
	if(p_status == PROCESSING_COMPLETED) {
		//Change state
		g_state = STATE_WAITING_SETTING_TIME;

		//Start the timer
		timer_api_set_count(TIMER_SETTING, SETTING_TIME_US);

		//Update display
		uint8_t thd = signal_analyzer_api_get_thd();
		display_api_update_THD(thd);

		//Update signal to inject
		cycle_t cycle = signal_analyzer_api_get_cycle_to_inject();
		inject_simulator_api_set_current_waveform(cycle.cycle, cycle.len);

		//Enables Injection Simulation
		inject_simulator_api_set_enable(true);
	}
}

void data_processing_api_state_waiting_setting_time() {
	//Checks timer
	status_timer_t status_t = timer_api_check_timer(TIMER_SETTING);

	//Timeout
	if(status_t) {
		//Restart process
		g_state = STATE_SAMPLING;
		current_sensor_api_clean_samples();
	}

	//Stop Button
	data_processing_api_check_stop_button();
}

static void data_processing_api_check_stop_button() {
	if(g_f_edge_button) {
		g_f_edge_button = false;
		g_state = STATE_STAND_BY;
		inject_simulator_api_set_enable(false);
	}
}

//======================================
// Public Function Implementations
//======================================

void data_processing_api_init() {
	display_api_set_msg_calibrate();
}

void data_processing_api_loop() {
	switch (g_state) {
		default:
			g_state = RESET;
			break;

		case STATE_RESET:
			data_processing_api_state_reset();
			break;

		case STATE_CALIBRATING:
			data_processing_api_state_calibrating();
			break;

		case STATE_STAND_BY:
			data_processing_api_state_stand_by();
			break;

		case STATE_SAMPLING:
			data_processing_api_state_sampling();
			break;

		case STATE_PROCESSING:
			data_processing_api_state_processing();
			break;

		case STATE_WAITING_SETTING_TIME:
			data_processing_api_state_waiting_setting_time();
			break;
	}
}

void data_processing_api_set_edge_button() {
	g_f_edge_button = true;
}
