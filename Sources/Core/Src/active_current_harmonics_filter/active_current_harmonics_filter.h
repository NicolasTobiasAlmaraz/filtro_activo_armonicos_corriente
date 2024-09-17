/**
 * @file data_processing_api.h
 * @author Nicolás Almaraz
 * @brief This API resolves the data processing
 */

#ifndef ACTIVE_CURRENT_HARMONICS_FILTER_H_
#define ACTIVE_CURRENT_HARMONICS_FILTER_H_

//======================================
// Dependencies
//======================================
#include "main.h"

//======================================
// Public Defines
//======================================
//Common defines
#define SAMPLING_FREQUENCY_HZ 	20000	//fs = 20kHz
#define SAMPLING_PERIOD_US 		50		//fs = 20kHz -> TS = 50 useg
#define CYCLES_TO_PROCESS		10		//Processing 10 cycles
#define SETTING_TIME_US 		5000000 //5 seg
#define LEN_MAX             	450		//Max samples on 50Hz cycle

//======================================
// Public Data Structures and Types
//======================================
/**
 * @brief This structure contains current samples for a 50Hz cycle
 */
typedef struct {
    uint16_t buffer[LEN_MAX];	//!< Samples
    uint32_t len;				//!< Number of Samples
} cycle_t;


//======================================
// Global Variables
//======================================

//======================================
// Public Function Declarations
//======================================

/**
 * @brief Initialize the system
 */
void my_system_init();

/**
 * @brief System state machine
 */
void my_system_state_machine();

/**
 * @brief Handles the event triggered by the user pressing the button.
 *
 * This function should be called when the user presses the button to
 * perform the necessary actions in the system. It ensures the button
 * press is registered and processed according to the system's logic.
 *
 * @note This function does not perform debouncing, so it should be
 * called from an ISR or a task where debouncing has been managed.
 */
void my_system_set_user_button();

#endif
