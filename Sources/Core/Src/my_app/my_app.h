/**
 * @file my_app.h
 * @author Nicolás Almaraz
 * @brief This is API starts the app
 */

#ifndef SRC_SYSTEM_MY_APP_H_
#define SRC_SYSTEM_MY_APP_H_

//======================================
// Dependencies
//======================================

//======================================
// Public Defines
//======================================

//Common defines
#define SAMPLING_FREQUENCY_HZ 	20000	//fs = 20kHz
#define CYCLES_TO_PROCESS		50		//Processing 50 cycles
#define SETTING_TIME_US 		5000000 //5 seg
#define LEN_MAX             	450		//Max samples on 50Hz cycle

//======================================
// Public Structures and Data Types
//======================================

/**
 * @brief This structure contains current samples for a 50Hz cycle
 */
typedef struct {
    uint16_t buffer[LEN_MAX];	//!< Samples
    uint32_t len;				//!< Number of Samples
} cycle_t;


//======================================
// Public Function Declarations
//======================================

/**
 * @brief Initialize the whole system
 */
void my_app_init();

/**
 * @brief Main loop
 */
void my_app_loop();

#endif /* SRC_SYSTEM_MY_APP_H_ */
