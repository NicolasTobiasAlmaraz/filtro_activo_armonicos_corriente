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

#include "inject_simulator.h"
#include "current_sensor.h"
#include "push_button.h"
#include "cycle_detector.h"
#include "timer_driver.h"

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
			cycle_detector_GPIO_IRQHandler();
			break;
		case USER_BUTTON_Pin:
			user_button_GPIO_IRQHandler();
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
        current_sensor_Timer_IRQHandler();
        inject_simulator_timer_IRQHandler();
    }
}


/**
 * @brief  Redefinition of WEAK function. Regular conversion complete callback in non blocking mode
 * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
 *         the configuration information for the specified ADC.
 * @retval None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    current_sensor_ADC_DMA_IRQHandler();
}
