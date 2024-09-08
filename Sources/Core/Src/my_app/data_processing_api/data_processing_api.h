/**
 * @file data_processing_api.h
 * @author Nicolás Almaraz
 * @brief This API resolves the data processing
 */

#ifndef SRC_MY_APP_DATA_PROCESSING_API_DATA_PROCESSING_API_H_
#define SRC_MY_APP_DATA_PROCESSING_API_DATA_PROCESSING_API_H_

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
void data_processing_api_init();

/**
 * @brief Loop for processing
 */
void data_processing_api_loop();

/**
 * @brief Notify to the API that the button was pressed
 */
void data_processing_api_set_edge_button();


#endif /* SRC_MY_APP_DATA_PROCESSING_API_DATA_PROCESSING_API_H_ */
