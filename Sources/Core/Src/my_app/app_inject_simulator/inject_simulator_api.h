/**
 * @file inject_simulator.h
 * @author Nicolás Almaraz
 * @brief Current injection simulator
 *
 * This API is responsible for simulating what would be a current injection into the power line.
 * Each time it receives the notification that a new cycle has started, it injects the pre-loaded waveform via DAC.
 * This allows the waveform calculated by the microcontroller to be measured with an oscilloscope at the DAC output.
 */

#ifndef SRC_MY_APP_INJECT_SIMULATOR_INJECT_SIMULATOR_H_
#define SRC_MY_APP_INJECT_SIMULATOR_INJECT_SIMULATOR_H_

//======================================
// Dependencies
//======================================
#include <stdbool.h>
#include <stdint.h>

#include "my_app.h"

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
void inject_simulator_api_init();

/**
 * @brief Manages the start of a new transmission.
 * This function should run in the main loop
 */
void inject_simulator_api_loop();

/**
 * @brief Sets the current waveform to inject on each new cycle detection
 * @param buffer with the cycle to inject
 */
void inject_simulator_api_set_current_waveform(cycle_t cycle);

/**
 * @brief Enable or disable the injection
 * @param enable true: Enable -- false: Disable
 */
void inject_simulator_api_set_enable(bool enable);

/**
 * @brief Notifies the API that a new cycle has started
 * This function should be called whenever a new cycle is detected on the power line
 */
void inject_simulator_api_set_new_cycle();

#endif /* SRC_MY_APP_INJECT_SIMULATOR_INJECT_SIMULATOR_H_ */
