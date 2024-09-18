/**
 * @file inject_simulator.h
 * @author Nicolás Almaraz
 * @brief Current injection simulator
 *
 * This API is responsible for simulating what would be a current injection into the power line.
 * Each time it receives the notification that a new cycle has started, it injects the pre-loaded waveform via DAC.
 * This allows the waveform calculated by the microcontroller to be measured with an oscilloscope at the DAC output.
 */

#ifndef INJECT_SIMULATOR_H_
#define INJECT_SIMULATOR_H_

//======================================
// Dependencies
//======================================
#include <stdbool.h>
#include <stdint.h>

#include "active_current_harmonics_filter.h"

//======================================
// Public Defines
//======================================

//======================================
// Public Structures and Data Types
//======================================

//======================================
// Global Variables
//======================================

//======================================
// Public Function Declarations
//======================================

/**
 * @brief Initializes the API
 */
void inject_simulator_init();

/**
 * @brief Manages the start of a new transmission.
 * This function should run in the main loop
 */
void app_inject_simulator_state_machine();

/**
 * @brief Sets the current waveform to inject on each new cycle detection
 * @param buffer with the cycle to inject
 */
void inject_simulator_set_waveform(cycle_t cycle);

/**
 * @brief Enable or disable the injection
 * @param enable (true: Enable) -- (false: Disable)
 */
void inject_simulator_set_enable(bool enable);

/**
 * @brief Notifies the API that a new cycle has started
 * This function should be called whenever a new cycle is detected on the power line
 */
void inject_simulator_cycle_IRQHandler();


/**
 * @brief Notifies the API that the current value must be updated
 * This function should be called in timer interrupt callback
 */
void inject_simulator_timer_IRQHandler();

#endif /* SRC_MY_APP_INJECT_SIMULATOR_INJECT_SIMULATOR_H_ */
