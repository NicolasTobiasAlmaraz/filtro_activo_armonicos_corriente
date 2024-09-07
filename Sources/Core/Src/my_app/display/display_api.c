/**
 * @file display.h
 * @author Nicolás Almaraz
 * @brief Display update with user messages
 */

//======================================
// Dependencies
//======================================
#include <display/display_driver/display_driver.h>
#include "main.h"
#include "display_api.h"

//======================================
// Private Defines
//======================================

//======================================
// Private Data Structures and Types
//======================================

//======================================
// Private Variables
//======================================

//======================================
// Private Function Declarations
//======================================

//======================================
// Private Function Implementations
//======================================

//======================================
// Public Function Implementations
//======================================

void display_init() {
	// Display Initialization
	display_driver_init();

	//Display Set Initial Messages

	//Line 1
	display_driver_set_cursor(0,0);
	display_driver_send_string("N. Almaraz - TP3:");

	//Line 2
	display_driver_set_cursor(1,0);
	display_driver_send_string("Filt Act Harmonic i(t)");

	//Line 3
	display_driver_set_cursor(2,0);
	display_driver_send_string("Line Current");

	//Line 4
	display_driver_set_cursor(3,0);
	display_driver_send_string("THD: XX %");
}

void display_update_THD(uint8_t thd) {
	//Check ranges
	if (thd >= 100)
		thd = 99;

	//Write THD value on display
	display_driver_set_cursor(3,5);
	char number[2];
	number[0] = '0' + thd / 10;
	number[1] = '0' + thd % 10;
	display_driver_send_string(number);
}
