/**
 * @file current_sensor.c
 * @author Nicolás Almaraz
 * @brief Implementa las funcionalidades para lectura del sensor de corriente
 *
 * Sensor de corriente:
 * S = 185mV/A
 * |--> 1mV = 1.2409 cuentas
 * |--> S = 229.5681 cuentas / A
 *
 * cuentas = S * corriente
 * |--> corriente = cuentas * 1/S
 *
 * 1/S = 0.004356004 A / cuentas
 * 1/S = 4.356004 mA / cuentas
 *
 *
 */

//======================================
// Dependencias
//======================================
#include "main.h"
#include "current_sensor.h"

//======================================
// Defines Privados
//======================================
#define MAX_UINT16_t 				65535
#define TOLERANCIA_CALIBRACION_MA	1
#define PENDIENTE_CORRIENTE 		(float) 4.356004 //mA / cuentas

//======================================
// Estructuras y Tipos de Datos Privados
//======================================

//======================================
// Variables Privadas
//======================================
static uint16_t g_offset;
static bool g_f_eoc;
static sample_t g_current_sample;

//======================================
// Declaración de Funciones Privadas
//======================================

/**
 * @brief Retorna el valor leído por el ADC
 */
uint16_t current_sensor_get_sample();

//======================================
// Implementación de Funciones Privadas
//======================================

uint16_t current_sensor_get_sample_ADC() {
	//todo
	return 0;
}

//======================================
// Implementación de Funciones Públicas
//======================================
void current_sensor_init() {

}

bool curent_sensor_calibrate() {
	uint16_t sample_max = 0;
	uint16_t sample_min = MAX_UINT16_t;
	uint32_t suma = 0;

	for(uint32_t i=0; i<100; i++) {
		//Tomo muestra
		uint16_t sample = current_sensor_get_sample_ADC();

		//Actualizo maximos y minimos
		 if(sample>sample_max)
			 sample_max = sample;

		 if(sample<sample_min)
			 sample_min = sample;

		 suma += sample;

		//Espero 10ms
		HAL_Delay(10);
	}

	float variabilidad = (sample_max - sample_min) * PENDIENTE_CORRIENTE;
	if(variabilidad > TOLERANCIA_CALIBRACION_MA)
		return CALIBRATE_ERROR;

	g_offset = (uint16_t) suma / 100;
	return CALIBRATE_OK;
}

bool current_sensor_sampling_loop() {
	if( timer_api_timeout() ) {
		current_sensor_get_sample_ADC();
		timer_api_set_count();
	}
	return g_f_eoc;
}

void current_sensor_clean_samples() {

}

sample_t current_sensor_get_samples() {

}
