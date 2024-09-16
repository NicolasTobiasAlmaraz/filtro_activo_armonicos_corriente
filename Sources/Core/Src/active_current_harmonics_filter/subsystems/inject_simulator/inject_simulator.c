/**
 * @file inject_simulator.c
 * @author Nicolás Almaraz
 * @brief Current injection simulator
 *
 * This API is responsible for simulating what would be a current injection into the power line.
 * Each time it receives the notification that a new cycle has started, it injects the pre-loaded waveform via DAC.
 * This allows the waveform calculated by the microcontroller to be measured with an oscilloscope at the DAC output.
 */

//======================================
// Dependencies
//======================================

#include <app_inject_simulator/inject_simulator_api.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "main.h"

//======================================
// Private Defines
//======================================

//======================================
// Private Structures and Data Types
//======================================

/**
 * @brief Enum to represent the current injection state.
 */
typedef enum {
	STATE_INJECTION_DISABLED,  //!< Injection is disabled
	STATE_INJECTION_RUNNING,   //!< Injection is running and actively updating the DAC
	STATE_INJECTION_WAITING_CYCLE
} inject_state_t;

//======================================
// Private Variables
//======================================
static bool g_f_new_cycle = false;							//!< Flag to indicate the detection of a new cycle.
static bool g_f_enable = false;								//!< Flag to indicate that the value must be updated.
static cycle_t g_cycle;										//!< Cycle to inject
static uint16_t g_cycle_index = 0;							//!< Index
static inject_state_t g_state = STATE_INJECTION_DISABLED;	//!< State

//======================================
// STM32 Handlers
//======================================
extern DAC_HandleTypeDef hdac;								//!< ADC handle for the DAC output

//======================================
// Private Function Declarations
//======================================
static void inject_simulator_api_dac_update(uint16_t value);

//======================================
// Private Function Implementations
//======================================
void inject_simulator_api_dac_update(uint16_t value) {
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
}

//======================================
// Public Function Implementations
//======================================
void inject_simulator_api_init() {
	//Initialize Cycle in 0
	for(uint32_t i=0; i<LEN_MAX ; i++) {
		g_cycle.buffer[i]=0;
	}

	//Dac start
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

	//Init 0 value
	inject_simulator_api_dac_update(0);
}

void app_inject_simulator_loop() {
    // For now, this function is empty because there is no power stage yet.
    // Once added, the PWM signal duty cycle values should be updated here.
}

void inject_simulator_api_set_current_waveform(cycle_t cycle) {
	g_cycle = cycle;
}

void inject_simulator_api_set_enable(bool enable) {
	g_f_enable = enable;
}

void inject_simulator_api_cycle_callback() {
	// Notify the system that a new cycle has started
	g_f_new_cycle = true;
}

void inject_simulator_api_timer_callback() {
	switch(g_state) {
		default:
		case STATE_INJECTION_DISABLED:
			if(g_f_enable) {
				g_cycle_index = 0;
				g_state = STATE_INJECTION_WAITING_CYCLE;
			}
			break;

		case STATE_INJECTION_RUNNING:
			if(!g_f_enable) {
				g_state = STATE_INJECTION_DISABLED;
				inject_simulator_api_dac_update(0);
				break;
			}
			if(g_f_new_cycle) {
				g_cycle_index=0;
				g_f_new_cycle = 0;
			}
			uint16_t value = g_cycle.buffer[g_cycle_index];
			inject_simulator_api_dac_update(value);
			g_cycle_index++;
			if(g_cycle_index >= g_cycle.len-1) {
				g_state = STATE_INJECTION_WAITING_CYCLE;
			}

		case STATE_INJECTION_WAITING_CYCLE:
			if(!g_f_enable) {
				g_state = STATE_INJECTION_DISABLED;
				inject_simulator_api_dac_update(0);
				break;
			}

			if(g_f_new_cycle) {
				uint16_t value = g_cycle.buffer[0];
				inject_simulator_api_dac_update(value);
				g_f_new_cycle = 0;
				g_cycle_index++;
				g_state = STATE_INJECTION_RUNNING;
			}
			break;
	}
}
