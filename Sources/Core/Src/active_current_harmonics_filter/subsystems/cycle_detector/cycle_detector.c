/**
 * @file cycle_detector.c
 * @author Nicolás Almaraz
 * @brief Zero-cross detection handling
 */

//======================================
// Dependencies
//======================================
#include "timer_driver.h"

#include "cycle_detector.h"

#include "inject_simulator.h"
#include "current_sensor.h"
#include "timer_driver.h"

//======================================
// Private Defines
//======================================
#define TIME_CYCLE_DETECTION_US 19000 	//!< Debouncing time for cycle detection [us].
#define LEN_MED_MOV 100 				//!< Number of cycles used to estimate the period.


//======================================
// Private Data Structures and Types
//======================================

//======================================
// Private Variables
//======================================
static uint32_t m_samples[LEN_MED_MOV];	//!< Array with durations of each period [us].
static uint32_t m_index = 0; 			//!< Iterator for the array.

//======================================
// Private Function Declarations
//======================================

/**
 * @brief Appends a new period
 */
static void m_append_period(uint32_t period);

//======================================
// Private Function Implementations
//======================================

void m_append_period(uint32_t period) {
	m_samples[m_index] = period;
	m_index++;
	m_index %= LEN_MED_MOV;
}



//======================================
// Public Function Implementations
//======================================

void cycle_detector_init() {
	for(uint32_t i=0; i<LEN_MED_MOV ; i++) {
		m_samples[i]=0;
	}
}

uint32_t cycle_detector_get_period() {
	uint32_t suma = 0;
	for(uint32_t i=0; i<LEN_MED_MOV ; i++) {
		suma += m_samples[i];
	}
	return (uint32_t) suma / LEN_MED_MOV;
}

void cycle_detector_GPIO_IRQHandler() {
	static uint32_t time_mem = 0;
	static bool f_first_time = true;
	if(f_first_time) {
		f_first_time = false;
		time_mem = timer_driver_get_ticks();
	}

	//Check debounce timeout
	status_timer_t timeout = timer_driver_check_timer(TIMER_DEBOUNCE_CYCLES_DETECTION);

	//If timer was running, it means a bounce was detected
	if(timeout == TIMER_RUNNING)
		return;

	timer_driver_start(TIMER_DEBOUNCE_CYCLES_DETECTION, TIME_CYCLE_DETECTION_US);

	//Current Sensor API Notification
	current_sensor_api_set_new_cycle();

	//Inject Simulator API Notification
	inject_simulator_cycle_IRQHandler();

	//Calculate Line Period
	uint32_t current_time = timer_driver_get_ticks();
	uint32_t period = current_time - time_mem;
	time_mem = current_time;
	m_append_period(period);
}
