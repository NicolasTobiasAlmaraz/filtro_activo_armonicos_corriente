/**
 * @file timer_api.h
 * @author Nicolás Almaraz
 * @brief Funciones de timer y gestion de tiempos
 */


#ifndef SRC_MY_APP_TIMER_API_TIMER_API_H_
#define SRC_MY_APP_TIMER_API_TIMER_API_H_

//======================================
// Dependencias
//======================================
#include <stdbool.h>
#include <stdint.h>
//======================================
// Defines Públicos
//======================================

//======================================
// Estructuras y Tipos de Datos Públicos
//======================================

/**
 * @brief Timers
 */
typedef enum {
	TIMER_SAMPLING,
	TIMER_DISPLAY,
	TIMER_DELAY,
}timer_id_t;

typedef enum {
	TIMER_RUNNING,
	TIMER_FINISH,
	TIMER_NO_CONFIG,
}timer_state_t;

//======================================
// Variables Globales
//======================================

//======================================
// Declaración de Funciones Públicas
//======================================

/**
 * @Init Initialize Timer API by polling
 */
void timer_api_init();

/**
 * @brief Checks the timer state
 * @param id Timer
 * @retval Status
 */
timer_state_t timer_api_check_timer(timer_id_t id);

/**
 * @brief Timer Start
 * @param id Timer to start
 * @param time_us Time in microseconds
 */
void timer_api_set_count(timer_id_t id, uint32_t time_us);

#endif /* SRC_MY_APP_TIMER_API_TIMER_API_H_ */
