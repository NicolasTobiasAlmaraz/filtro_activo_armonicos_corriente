/**
 * @file data_processing_api.h
 * @author Nicolás Almaraz
 * @brief This API resolves the data processing
 */

#ifndef SRC_MY_APP_APP_PROCESSING_APP_PROCESSING_H_
#define SRC_MY_APP_APP_PROCESSING_APP_PROCESSING_H_

//======================================
// Dependencies
//======================================

//======================================
// Public Defines
//======================================

//======================================
// Public Data Structures and Types
//======================================

//======================================
// Global Variables
//======================================

//======================================
// Public Function Declarations
//======================================

/**
 * @brief Initialize the API
 */
void app_processing_init();

/**
 * @brief Loop for processing
 */
void app_processing_loop();

/**
 * @brief Notify to the API that the button was pressed
 */
void data_processing_api_set_edge_button();


#endif /* SRC_MY_APP_APP_PROCESSING_APP_PROCESSING_H_ */
