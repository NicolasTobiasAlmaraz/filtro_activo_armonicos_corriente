/**
 * @file my_app.c
 * @author Nicolás Almaraz
 * @brief Lógica a nivel sistema
 */

//======================================
// Dependencias
//======================================
#include <stdbool.h>

#include "my_app.h"
#include "cycle_detector/cycle_detector_api.h"
#include "signal_analyzer/signal_analyzer_api.h"
#include "current_sensor/current_sensor_api.h"
#include "inject_simulator/inject_simulator_api.h"
#include "display/display_api.h"

//======================================
// Defines Privados
//======================================

//======================================
// Estructuras y Tipos de datos privados
//======================================
typedef enum {
	STATE_INICIO,
	STATE_CALIBRANDO,
	STATE_STAND_BY,
	STATE_TOMANDO_MUESTRAS_ADC,
	STATE_PROCESANDO,
	STATE_ACTUALIZANDO_INYECCION,
	STATE_ESPERANDO_ESTABLECIMIENTO,
}app_state_t;

//======================================
// Variables Privadas
//======================================
static app_state_t g_estado = STATE_INICIO;

//======================================
// Declaración de Funciones Privadas
//======================================

//======================================
// Implementación de Funciones Privadas
//======================================

//======================================
// Implementación de Funciones Privadas
//======================================

//======================================
// Callbacks
//======================================

//======================================
// Implementación de funciones Públicas
//======================================
void my_app_init() {
	cycle_detector_api_init();
	signal_analyzer_init();
	display_init();
	current_sensor_api_init();
	inject_simulator_init();
}

void my_app_loop() {
	switch (g_estado) {
		case STATE_INICIO:
			break;
		case STATE_CALIBRANDO:
			bool status = current_sensor_api_calibrate();
			if(status)
				g_estado = STATE_STAND_BY;
			else
				g_estado = STATE_INICIO;
			break;

		case STATE_STAND_BY:
			break;

		case STATE_TOMANDO_MUESTRAS_ADC:
			bool f_eoc = current_sensor_api_sampling_loop();
			if(f_eoc)
				g_estado = STATE_PROCESANDO;
			break;

		case STATE_PROCESANDO:

			break;

		case STATE_ACTUALIZANDO_INYECCION:
			break;

		case STATE_ESPERANDO_ESTABLECIMIENTO:
			break;
	}
}
