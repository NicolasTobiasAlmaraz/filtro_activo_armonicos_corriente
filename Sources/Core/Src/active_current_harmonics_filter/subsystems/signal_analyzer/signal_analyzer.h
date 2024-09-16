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
#include "app_processing/current_sensor_api/current_sensor_api.h"

//======================================
// Public Defines
//======================================

//======================================
// Public Structures and Data Types
//======================================
typedef enum {
	PROCESSING_IN_PROGRESS,
	PROCESSING_COMPLETED,
}status_processing_t;

//======================================
// Global Variables
//======================================

//======================================
// Public Function Declarations
//======================================

/**
 * @brief Set parameters to analyze
 * @param cycles average_cycle
 * @param zero_offset Value of ADC when the sensor measures 0A
 */
void signal_analyzer_api_set_signal_to_analyze(cycle_t ave_cycle, uint16_t zero_offset);

/**
 * @brief Analyzes the signal setted with signal_analyzer_api_set_signal_to_analyze()
 * @retval Status of analyzes (Completed / In progress)
 */
status_processing_t signal_analyzer_api_analyze_loop();

/**
 * @brief Retrieves the Total Harmonic Distortion (THD)
 * @retval THD [%]
 */
uint16_t signal_analyzer_api_get_thd();

/**
 * @brief Retrieves the injected current
 * @retval Cycle to inject
 */
cycle_t signal_analyzer_api_get_cycle_to_inject();

#endif /* SRC_MY_APP_SIGNAL_ANALYZER_SIGNAL_ANALYZER_H_ */
