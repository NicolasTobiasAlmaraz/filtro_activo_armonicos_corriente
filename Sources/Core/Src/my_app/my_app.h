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

//======================================
// Public Structures and Data Types
//======================================

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
