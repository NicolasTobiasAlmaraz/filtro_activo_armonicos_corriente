/**
 * @file my_app.c
 * @author Nicolás Almaraz
 * @brief Lógica a nivel sistema
 */

//======================================
// Dependencias
//======================================
#include "my_app.h"
#include "zero_cross/zero_cross.h"
#include "signal_analyzer/signal_analyzer.h"
#include "current_sensor/current_sensor.h"
#include "inject_simulator/inject_simulator.h"
#include "display/display.h"

//======================================
// Defines Privados
//======================================

//======================================
// Estructuras y Tipos de datos privados
//======================================

//======================================
// Variables Privadas
//======================================

//======================================
// Declaración de Funciones Privadas
//======================================

//======================================
// Implementación de Funciones Privadas
//======================================

//======================================
// Implementación de Funciones Públicas
//======================================

void my_app_init(peripherals_handler_t handlers) {
	zero_cross_init();
	signal_analyzer_init();
	display_init();
	current_sensor_init();
	inject_simulator_init();
}

void my_app_loop() {
	HAL_Delay(1000);
	display_update_THD(75);
	HAL_Delay(1000);
	display_update_THD(50);
	HAL_Delay(1000);
	display_update_THD(25);
	HAL_Delay(1000);
	display_update_THD(0);
}

//======================================
// Implementación de funciones públicas
//======================================
