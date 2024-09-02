/**
 * @file current_sensor.h
 * @author Nicolás Almaraz
 * @brief Implementa las funcionalidades para lectura del sensor de corriente
 */


#ifndef SRC_MY_APP_CURRENT_SENSOR_CURRENT_SENSOR_H_
#define SRC_MY_APP_CURRENT_SENSOR_CURRENT_SENSOR_H_

//======================================
// Dependencias
//======================================
#include <stdbool.h>
#include <stdint.h>

//======================================
// Defines Públicos
//======================================
#define CANT_CICLOS 		50
#define LONG_MAX 			450
#define CALIBRATE_OK 		0
#define CALIBRATE_ERROR 	1

//======================================
// Estructuras y Tipos de Datos Públicos
//======================================
typedef struct {
	uint16_t cycle[LONG_MAX];
	uint32_t len;
}cycle_t;

typedef struct {
	cycle_t ciclos[CANT_CICLOS];
}sample_t;

//======================================
// Variables Globales
//======================================

//======================================
// Declaración de Funciones Públicas
//======================================

/**
 * @brief Inicializa Api de Medición
 */
void current_sensor_init();

/**
 * @brief Mide cuanto es el offset de la señal cuando no hay corriente y guarda el valor para las futuras conversiones
 *
 * Mide cuanto es el offset de la señal cuando no hay corriente.
 * Se considera que hay conumo de corriente cuando hay una variación mayor a TOLERANCIA_CALIBRACION_MA mA
 * Internamente guarda en la API el valor para calibrar las próximas mediciones
 *
 * Retorna:
 * 	CALIBRATE_OK: Calibración exitosa, se guardó el offset en el puntero
 *	CALIBRATE_ERROR: Se detectó un consumo mientras se hacia la calibración
 * @retval status
 */
bool curent_sensor_calibrate();

/**
 * @brief Capture 50 cycles of current (non-blocking function)
 *
 * This function indicates when it has finished capturing CYCLES cycles of current.
 * These cycles are stored in 'samples_t' structure.
 * The function should be called repeatedly in a loop and will signal the end of conversion through its return status
 * @retval Flag End Of Conversion: 1-Finish / 0-Not yet
 */
bool current_sensor_sampling_loop();

/**
 * @brief Cleans the 50 cycles captured
 */
void current_sensor_clean_samples();

/**
 * @brief Returns the 50 cycles captured
 */
sample_t current_sensor_get_samples();

#endif /* SRC_MY_APP_CURRENT_SENSOR_CURRENT_SENSOR_H_ */
