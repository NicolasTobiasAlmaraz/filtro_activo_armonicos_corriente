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
 * @brief Inicializa timer por polling
 */
void timer_api_init();

/**
 * @brief Verifica si venció el timer consultado
 * @param id Timer a consultar
 * @retval status
 */
timer_state_t timer_api_timeout(timer_id_t id);

/**
 * @brief Le da inicio al timer indicado
 */
void timer_api_set_count(timer_id_t id, uint32_t count);

#endif /* SRC_MY_APP_TIMER_API_TIMER_API_H_ */
