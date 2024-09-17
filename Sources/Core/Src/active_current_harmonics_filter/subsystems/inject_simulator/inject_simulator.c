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
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "main.h"

#include "inject_simulator.h"

//======================================
// Private Defines
//======================================

//======================================
// Private Structures and Data Types
//======================================

/**
 * @brief Enumeration of current injection states.
 */
typedef enum {
    STATE_INJECTION_DISABLED,    	//!< Injection is disabled
    STATE_INJECTION_RUNNING,     	//!< Injection is running and actively updating the DAC
    STATE_INJECTION_WAITING_CYCLE, 	//!< Waiting for a new cycle to start injection
} inject_state_t;

//======================================
// Private Variables
//======================================
static bool m_f_new_cycle = false;							//!< Flag to indicate the detection of a new cycle.
static bool m_f_enable = false;								//!< Flag to indicate that the value must be updated.
static cycle_t m_cycle;										//!< Cycle to inject
static uint16_t m_cycle_index = 0;							//!< Index
static inject_state_t m_state = STATE_INJECTION_DISABLED;	//!< State

//======================================
// STM32 Handlers
//======================================
extern DAC_HandleTypeDef hdac;								//!< ADC handle for the DAC output

//======================================
// Private Function Declarations
//======================================
static void m_dac_update(uint16_t value);

//======================================
// Private Function Implementations
//======================================
void m_dac_update(uint16_t value) {
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
}

//======================================
// Public Function Implementations
//======================================
void inject_simulator_init() {
	//Initialize Cycle in 0
	for(uint32_t i=0; i<LEN_MAX ; i++) {
		m_cycle.buffer[i]=0;
	}

	//Dac start
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

	//Init 0 value
	m_dac_update(0);
}

void app_inject_simulator_state_machine() {
    // For now, this function is empty because there is no power stage yet.
    // Once added, the PWM signal duty cycle values should be updated here.
}

void inject_simulator_set_waveform(cycle_t cycle) {
	m_cycle = cycle;
}

void inject_simulator_set_enable(bool enable) {
	m_f_enable = enable;
}

void inject_simulator_cycle_IRQHandler() {
	// Notify the system that a new cycle has started
	m_f_new_cycle = true;
}

void inject_simulator_timer_IRQHandler() {
	switch(m_state) {
		default:
		case STATE_INJECTION_DISABLED:
			if(m_f_enable) {
				m_cycle_index = 0;
				m_state = STATE_INJECTION_WAITING_CYCLE;
			}
			break;

		case STATE_INJECTION_RUNNING:
			if(!m_f_enable) {
				m_state = STATE_INJECTION_DISABLED;
				m_dac_update(0);
				break;
			}
			if(m_f_new_cycle) {
				m_cycle_index=0;
				m_f_new_cycle = 0;
			}
			uint16_t value = m_cycle.buffer[m_cycle_index];
			m_dac_update(value);
			m_cycle_index++;
			if(m_cycle_index >= m_cycle.len-1) {
				m_state = STATE_INJECTION_WAITING_CYCLE;
			}

		case STATE_INJECTION_WAITING_CYCLE:
			if(!m_f_enable) {
				m_state = STATE_INJECTION_DISABLED;
				m_dac_update(0);
				break;
			}

			if(m_f_new_cycle) {
				uint16_t value = m_cycle.buffer[0];
				m_dac_update(value);
				m_f_new_cycle = 0;
				m_cycle_index++;
				m_state = STATE_INJECTION_RUNNING;
			}
			break;
	}
}
