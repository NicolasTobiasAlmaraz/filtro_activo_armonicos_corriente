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
	STATE_INJECTION_WAITING,   //!< Waiting for a new cycle to start the injection
	STATE_INJECTION_RUNNING,   //!< Injection is running and actively updating the DAC
} inject_state_t;

//======================================
// Private Variables
//======================================
static uint16_t* g_current_buffer = NULL;					//!< Pointer to the current waveform buffer to be injected via DAC.
static uint32_t g_len_current = 0;							//!< Length of the current waveform buffer.
static inject_state_t g_state = STATE_INJECTION_DISABLED;	//!< Current state of the injection (disabled, waiting, or running).
static bool g_f_new_cycle = false;							//!< Flag to indicate the detection of a new cycle.

//======================================
// STM32 Handlers
//======================================
extern DAC_HandleTypeDef hdac;								//!< ADC handle for the DAC output

//======================================
// Private Function Declarations
//======================================

//======================================
// Private Function Implementations
//======================================

//======================================
// Public Function Implementations
//======================================
void inject_simulator_api_init() {
	// todo
}

void inject_simulator_api_loop() {

	switch(g_state) {
		case STATE_INJECTION_DISABLED:
			// Do nothing if the injection is disabled
			break;

		case STATE_INJECTION_WAITING:
			// Wait for a new cycle to start the injection
			if(g_f_new_cycle) {
				g_f_new_cycle = false;
				// Start DAC output with the current waveform buffer
				HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)g_current_buffer, g_len_current / 2, DAC_ALIGN_12B_R);
			}
			break;

		case STATE_INJECTION_RUNNING:
			// If a new cycle is detected, restart the DAC output
			if(g_f_new_cycle) {
				g_f_new_cycle = false;
				// Stop and restart DAC output to synchronize with the new cycle
				HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
				HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)g_current_buffer, g_len_current / 2, DAC_ALIGN_12B_R);
			}
			break;
	}
}

void inject_simulator_api_set_current_waveform(uint16_t *buffer, uint32_t len) {
	// Reallocate dynamic memory for the current waveform buffer
	g_current_buffer = realloc((void *) g_current_buffer, len * sizeof(uint16_t));
	if(g_current_buffer == NULL)
		return;

	// Copy the new waveform into the buffer
	memcpy(g_current_buffer, buffer, len * sizeof(uint16_t));

	// Update the current length of the buffer
	g_len_current = len;
}

void inject_simulator_api_set_enable(bool enable) {
	if(enable) {
		// If was enabled return
		if(g_state == STATE_INJECTION_WAITING)
			return;

		// Enable the injection and wait for a new cycle to start
		g_state = STATE_INJECTION_WAITING;
		g_f_new_cycle = false;
	} else {
		// Disable the injection
		g_state = STATE_INJECTION_DISABLED;
	}
}

void inject_simulator_api_set_new_cycle() {
	// Notify the system that a new cycle has started
	g_f_new_cycle = true;
}
