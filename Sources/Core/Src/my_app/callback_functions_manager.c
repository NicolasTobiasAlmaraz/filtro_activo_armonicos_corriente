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


/**
 * @brief  Redefinition of WEAK function. GPIO EXTI Callback
 * @param  GPIO_Pin Specifies the pins connected EXTI line
 * @retval None
 */

#include "main.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch(GPIO_Pin) {
		case 0:
			break;
		case 1:
			break;
	}
}


