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

#include "common_apis/timer_api/timer_api.h"
#include "common_apis/cycle_detector_api/cycle_detector_api.h"

#include "app_processing/push_button_driver/push_button_driver.h"
#include "app_processing/current_sensor_api/current_sensor_api.h"
#include "app_inject_simulator/inject_simulator_api.h"

//======================================
// Callbacks - GPIO
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

//======================================
// Callbacks - ADC / DAC
//======================================

/**
 * @brief  Redefinition of WEAK function. Period elapsed callback in non-blocking mode
 * @param  htim TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM3) {
        current_sensor_api_timer_callback();
        inject_simulator_api_timer_callback();
    }
}


/**
 * @brief  Redefinition of WEAK function. Regular conversion complete callback in non blocking mode
 * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
 *         the configuration information for the specified ADC.
 * @retval None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    current_sensor_api_dma_callback();
}

/**
 * @brief  Redefinition of WEAK function. DAC DMA Tx complete
 */
//void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
	//Do nothing
//}
