/**
 * @file signal_analyzer.h
 * @author Nicolás Almaraz
 * @brief Header file for main signal processing to obtain THD and correction waveform.
 *
 * This API processes a `cycle_t` signal to obtain the Total Harmonic Distortion (THD)
 * and the correction waveform needed to compensate for harmonics.
 */

#ifndef SIGNAL_ANALYZER_H_
#define SIGNAL_ANALYZER_H_

//======================================
// Dependencies
//======================================
#include "active_current_harmonics_filter.h"

//======================================
// Public Defines
//======================================

//======================================
// Public Structures and Data Types
//======================================

/**
 * @brief Status of the signal processing.
 */
typedef enum {
    PROCESSING_IN_PROGRESS, //!< Signal processing is ongoing
    PROCESSING_COMPLETED,   //!< Signal processing is complete
} status_processing_t;

//======================================
// Global Variables
//======================================

//======================================
// Public Function Declarations
//======================================

/**
 * @brief Sets parameters for signal analysis.
 * @param ave_cycle Average cycle to analyze
 * @param zero_offset ADC value when the sensor measures 0A
 */
void signal_analyzer_set_signal_to_analyze(cycle_t ave_cycle, uint16_t zero_offset);

/**
 * @brief Processes the signal set with `signal_analyzer_set_signal_to_analyze()`.
 * @retval Status of processing (Completed or In Progress)
 */
status_processing_t signal_analyzer_state_machine();

/**
 * @brief Retrieves the Total Harmonic Distortion (THD) value.
 * @retval THD in percentage [%]
 */
uint16_t signal_analyzer_get_thd();

/**
 * @brief Retrieves the correction waveform to inject.
 * @retval Cycle to inject
 */
cycle_t signal_analyzer_get_cycle_to_inject();

#endif /* SIGNAL_ANALYZER_H_ */
