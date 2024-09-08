/**
 * @file callback_functions_manager.c
 * @author Nicolás Almaraz
 * @brief Manages interrupt callbacks and events for the entire application.
 *
 * This file handles callback functions for various interrupts and events across the entire application.
 * It centralizes the management of these interrupts and events and directs them to the appropriate
 * API-specific callback functions with custom names.
 *
 * The purpose of this file is to provide a single point of management for all application-wide
 * interrupts and events, ensuring that each API can handle them correctly without interference.
 *
 * Usage:
 * - Define custom callback functions for each API and event.
 * - Implement the generic interrupt and event handling in this file.
 * - The appropriate API callback function is called based on the interrupt or event source.
 */

//======================================
// Dependencies
//======================================
#include "main.h"
#include "common_apis/cycle_detector_api/cycle_detector_api.h"
#include "data_processing_api/push_button_driver/push_button_driver.h"

//======================================
// Callbacks
//======================================

/**
 * @brief Redefinition of WEAK function. GPIO EXTI Callback
 * @param GPIO_Pin Specifies the pins connected EXTI line
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch(GPIO_Pin) {
		case CYCLE_DETECTOR_Pin:
			cycle_detector_api_callback();
			break;
		case USER_BUTTON_Pin:
			push_button_driver_callback();
			break;
	}
}

/**
 * @brief  Redefinition of WEAK function. DAC DMA Tx complete
 */
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
	//Do nothing
}
