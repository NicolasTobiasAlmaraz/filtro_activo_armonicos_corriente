/**
 * @file current_sensor.c
 * @author Nicolás Almaraz
 * @brief Implementa las funcionalidades para lectura del sensor de corriente
 *
 * Cálculos del Sensor de Corriente:
 * 		Sensibilidad:
 * 		S = 185mV/A
 * 		|--> 1mV = 1.2409 cuentas
 * 		|--> S = 229.5681 cuentas / A
 *
 * 		cuentas = S * corriente
 * 		|--> corriente = cuentas * 1/S
 * 		|--> 1/S = 0.004356004 A / cuentas
 * 		|--> 1/S = 4.356004 mA / cuentas
 */

//======================================
// Dependencias
//======================================
#include <string.h>

#include "main.h"
#include "current_sensor.h"
#include "timer_api/timer_api.h"

//======================================
// Defines Privados
//======================================
#define MAX_UINT16_t 				65535
#define TOLERANCIA_CALIBRACION_MA	1
#define PENDIENTE_CORRIENTE 		(float) 4.356004 //mA / cuentas

//======================================
// Estructuras y Tipos de Datos Privados
//======================================
typedef enum{
	STATE_START,
	STATE_SAMPLING,
	STATE_WAITING,
	STATE_EOC,
}state_current_sensor_t;

//======================================
// Handlers STM32
//======================================
extern ADC_HandleTypeDef hadc1;

//======================================
// Variables Privadas
//======================================
static state_current_sensor_t g_state = STATE_START;	//<! Estado maquina de estados

static cycle_t g_current_cycles[CYCLES];				//<! Muestras
static uint32_t g_cycles_index = 0;						//<! Indice del vector
static uint16_t g_offset = MAX_UINT16_t / 2;			//<! Offset que hay que ponerle a las muestras

static bool g_f_new_cycle = false;						//<! Flag que notifica si llegó un nuevo ciclo en la tensión de línea

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
	uint32_t sample = HAL_ADC_GetValue(&hadc1);
	return (uint16_t)sample;
}

//======================================
// Implementación de Funciones Públicas
//======================================
void current_sensor_init() {
	current_sensor_clean_samples();
}

bool current_sensor_calibrate() {
	uint16_t sample_max = 0;
	uint16_t sample_min = MAX_UINT16_t;
	uint32_t suma = 0;

	for(uint32_t i=0; i<100; i++) {
		//Sample
		uint16_t sample = current_sensor_get_sample_ADC();

		//Update max and min
		 if(sample>sample_max)
			 sample_max = sample;

		 if(sample<sample_min)
			 sample_min = sample;

		 suma += sample;

		//Wait 10ms
		HAL_Delay(10);
	}

	//Max deviation
	float variabilidad = (sample_max - sample_min) * PENDIENTE_CORRIENTE;
	if(variabilidad > TOLERANCIA_CALIBRACION_MA)
		return CALIBRATE_ERROR;

	//Calculate offset
	g_offset = (uint16_t) suma / 100;
	return CALIBRATE_OK;
}

bool current_sensor_sampling_loop() {
	bool f_eoc = false;

	switch(g_state) {
		case STATE_START:
			if(g_f_new_cycle) {
				g_state = STATE_SAMPLING;
			}
			break;

		case STATE_SAMPLING:
			//Take a new sample
			uint16_t sample = current_sensor_get_sample_ADC();

			//Timer start
			timer_api_set_count(TIMER_SAMPLING, 200);

			//If i receive a new cycle notification, change the cycle
			if( g_f_new_cycle ) {
				g_f_new_cycle = false;
				g_cycles_index++;

				//If the CYCLES cycles are completed return EOC
				if(g_cycles_index == CYCLES) {
					g_state = STATE_EOC;
					f_eoc = true;
					break;
				}
			}

			//Save the sample
			uint32_t len = g_current_cycles[g_cycles_index].len;
			g_current_cycles[g_cycles_index].cycle[len] = sample;
			len++;
			g_current_cycles[g_cycles_index].len = len;
			break;

		case STATE_WAITING:
			if( timer_api_check_timer(TIMER_SAMPLING) ==  TIMER_FINISH) {
				g_state = STATE_SAMPLING;
			}
			break;

		case STATE_EOC:
			f_eoc = true;
			break;
	}

	return f_eoc;
}

void current_sensor_clean_samples() {
	g_state = STATE_START;
	g_cycles_index = 0;
	for(uint32_t i=0; i<CYCLES; i++) {
		g_current_cycles[i].len=0;
	}
}

void current_sensor_get_samples(cycle_t *ptr_out) {
    memcpy(ptr_out, g_current_cycles, sizeof(g_current_cycles));
}

void current_sensor_set_new_cycle() {
	g_f_new_cycle = true;
}

uint16_t current_sensor_get_offset() {
	return g_offset;
}

