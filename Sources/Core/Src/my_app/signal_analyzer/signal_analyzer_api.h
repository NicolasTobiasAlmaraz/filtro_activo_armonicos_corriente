/**
 * @file signal_analyzer.h
 * @author Nicolás Almaraz
 * @brief Main signal processing to obtain THD and ic(t)
 */


#ifndef SRC_MY_APP_SIGNAL_ANALYZER_SIGNAL_ANALYZER_H_
#define SRC_MY_APP_SIGNAL_ANALYZER_SIGNAL_ANALYZER_H_

//======================================
// Dependencies
//======================================
#include "main.h"
#include "current_sensor/current_sensor_api.h"

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
void signal_analyzer_api_init();

/**
 * @brief Analyzes the signal
 * @param cycles Samples of each cycle
 * @param len Number of cycles
 * @param zero_offset Zero crossing offset value
 */
void signal_analyzer_api_start_new_analyze(cycle_t *cycles, uint32_t len, uint16_t zero_offset);

/**
 * @brief Retrieves the Total Harmonic Distortion (THD)
 */
uint8_t signal_analyzer_api_get_thd();

/**
 * @brief Retrieves the injected current
 */
cycle_t signal_analyzer_api_get_inject_current();

/**
 * @brief Sets the sampling frequency (fs)
 * @param fs Sampling frequency in Hz
 */
void signal_analyzer_api_set_fs(uint32_t fs);

#endif /* SRC_MY_APP_SIGNAL_ANALYZER_SIGNAL_ANALYZER_H_ */
