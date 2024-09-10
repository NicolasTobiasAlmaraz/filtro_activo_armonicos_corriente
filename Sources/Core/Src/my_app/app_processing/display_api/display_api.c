/**
 * @file display.h
 * @author Nicolás Almaraz
 * @brief Display update with user messages
 */

//======================================
// Dependencies
//======================================
#include <app_processing/display_api/display_api.h>
#include <app_processing/display_api/display_driver/display_driver.h>

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


//--- Initial messages

void display_api_init() {
	// Display Initialization
	display_driver_init();

	//Display Set initial messages

	//Line 1
	display_driver_set_cursor(0,0);
	display_driver_send_string("Nico Almaraz - TP3: ");

	//Line 2
	display_driver_set_cursor(1,0);
	display_driver_send_string("Filtro Act Armo i(t)");
}


//--- Calibration messages

void display_api_set_msg_start_calibration() {
	//Line 3
	display_driver_set_cursor(2,0);
	display_driver_send_string("Desconecte cargas y ");

	//Line 4
	display_driver_set_cursor(3,0);
	display_driver_send_string("presione el pulsador");
}

void display_api_set_msg_calibrating() {
	//Line 3
	display_driver_set_cursor(2,0);
	display_driver_send_string("Calibrando ...      ");

	//Line 4
	display_driver_set_cursor(3,0);
	display_driver_send_string("                    ");
}

void display_api_set_msg_calibration_ok() {
	//Line 1
	display_driver_set_cursor(0,0);
	display_driver_send_string("Calibracion Exitosa!");

	//Line 2
	display_driver_set_cursor(1,0);
	display_driver_send_string("Conecte cargas y    ");

	//Line 3
	display_driver_set_cursor(2,0);
	display_driver_send_string("presione pulsador   ");

	//Line 4
	display_driver_set_cursor(3,0);
	display_driver_send_string("para iniciar proceso");
}

void display_api_set_msg_calibration_error() {
	//Line 2
	display_driver_set_cursor(1,0);
	display_driver_send_string("Error de calibracion!");

	//Line 3
	display_driver_set_cursor(2,0);
	display_driver_send_string("Vuelva a intentar   ");

	//Line 4
	display_driver_set_cursor(3,0);
	display_driver_send_string("presionando pulsador");
}


//--- THD messages

void display_api_set_msg_THD() {
	//Line 1
	display_driver_set_cursor(0,0);
	display_driver_send_string("Nico Almaraz - TP3: ");

	//Line 2
	display_driver_set_cursor(1,0);
	display_driver_send_string("State - Working     ");

	//Line 3
	display_driver_set_cursor(2,0);
	display_driver_send_string("Corriente de linea: ");

	//Line 4
	display_driver_set_cursor(3,0);
	display_driver_send_string("THD: -- %           ");
}

void display_api_update_THD(uint8_t thd) {
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


// -- State messages
void display_api_set_msg_stand_by() {
	//Line 2
	display_driver_set_cursor(1,0);
	display_driver_send_string("State - Stand By    ");
}
