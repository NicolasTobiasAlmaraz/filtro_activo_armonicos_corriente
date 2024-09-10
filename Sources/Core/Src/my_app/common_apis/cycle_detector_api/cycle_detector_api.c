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

#include "app_processing/current_sensor_api/current_sensor_api.h"

//======================================
// Private Defines
//======================================
#define TIME_CYCLE_DETECTION_US 15000	//15ms
#define LEN_MED_MOV				100   	//100 cycles to estimate the period

//======================================
// Private Data Structures and Types
//======================================

//======================================
// Private Variables
//======================================
static uint32_t g_samples[LEN_MED_MOV];
static uint32_t g_index=0;


//======================================
// Private Function Declarations
//======================================

/**
 * @brief Appends a new period
 */
static void cycle_detector_api_append_period(uint32_t period);

//======================================
// Private Function Implementations
//======================================

void cycle_detector_api_append_period(uint32_t period) {
	g_samples[g_index] = period;
	g_index++;
	g_index %= LEN_MED_MOV;
}



//======================================
// Public Function Implementations
//======================================

void cycle_detector_api_init() {

}

uint32_t cycle_detector_api_get_period(uint32_t period) {
	uint32_t suma = 0;
	for(uint32_t i=0; i<LEN_MED_MOV ; i++) {
		suma += g_samples[i];
	}
	return (uint32_t) suma / LEN_MED_MOV;
}

void cycle_detector_api_callback() {
	static uint32_t time_mem = 0;
	static bool f_first_time = true;
	if(f_first_time) {
		f_first_time = false;
		time_mem = timer_api_get_ticks();
	}

	//Check debounce timeout
	status_timer_t timeout = timer_api_check_timer(TIMER_DEBOUNCE_CYCLES_DETECTION);

	//If timer was running, it means a bounce was detected
	if(timeout == TIMER_RUNNING)
		return;

	timer_api_set_count(TIMER_DEBOUNCE_CYCLES_DETECTION, TIME_CYCLE_DETECTION_US);

	//Calculate Line Period
	uint32_t current_time = timer_api_get_ticks();
	uint32_t period = current_time - time_mem;
	time_mem = current_time;
	cycle_detector_api_append_period(period);

	//Inject Simulator API
	inject_simulator_api_set_new_cycle();
}
