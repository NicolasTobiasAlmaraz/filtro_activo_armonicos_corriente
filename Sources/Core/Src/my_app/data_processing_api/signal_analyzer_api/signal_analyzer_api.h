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
#include "current_sensor_api/current_sensor_api.h"

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
 * @brief Initializes the API
 */
void signal_analyzer_api_init();

/**
 * @brief Analyzes the signal
 * @param cycles Samples of each cycle
 * @param len Number of cycles
 * @param zero_offset Zero crossing offset value
 * @retval Status code
 */
status_processing_t signal_analyzer_api_analyze_loop(cycle_t *cycles, uint32_t len, uint16_t zero_offset);

/**
 * @brief Retrieves the Total Harmonic Distortion (THD)
 * @retval THD [%]
 */
uint8_t signal_analyzer_api_get_thd();

/**
 * @brief Retrieves the injected current
 * @retval Cycle to inject
 */
cycle_t signal_analyzer_api_get_cycle_to_inject();

/**
 * @brief Restart the processing
 */
void signal_analyzer_api_clear();

#endif /* SRC_MY_APP_SIGNAL_ANALYZER_SIGNAL_ANALYZER_H_ */
