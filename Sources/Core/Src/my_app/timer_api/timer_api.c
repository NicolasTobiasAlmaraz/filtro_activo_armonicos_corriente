/**
 * @file timer_api.c
 * @author Nicolás Almaraz
 * @brief Funciones de timer y gestion de tiempos
 */

//======================================
// Dependencias
//======================================
#include "main.h"
#include "timer_api.h"

//======================================
// Defines Privados
//======================================
#define TICK_TIME 1
#define NUM_TIMERS 3

//======================================
// Estructuras y Tipos de datos privados
//======================================
typedef struct {
	uint32_t end_count;
	timer_state_t state;
}my_polling_timer_t;

//======================================
// Handlers STM32
//======================================
extern TIM_HandleTypeDef htim2;

//======================================
// Variables Privadas
//======================================
static my_polling_timer_t g_timers[3];

//======================================
// Declaración de Funciones Privadas
//======================================

//======================================
// Implementación de Funciones Privadas
//======================================

//======================================
// Implementación de Funciones Públicas
//======================================

void timer_api_init() {
	//Init timers
	for(uint8_t i=0; i<NUM_TIMERS; i++)
		g_timers[i].state = TIMER_NO_CONFIG;

	//Timer start
	HAL_TIM_Base_Start(&htim2);
}

timer_state_t timer_api_check_timer(timer_id_t id) {
	//Verify if timer was configured
	if(g_timers[id].state == TIMER_NO_CONFIG)
		return TIMER_NO_CONFIG;

	//Get current count and end time
	uint32_t tim_count = __HAL_TIM_GET_COUNTER(&htim2);
	uint32_t tim_end = g_timers[id].end_count;

	//Verify timeout
	if(tim_count >= tim_end) {
		g_timers[id].state = TIMER_FINISH;
		g_timers[id].state = TIMER_NO_CONFIG;
		return TIMER_FINISH;
	} else {
		return TIMER_RUNNING;
	}
}

void timer_api_set_count(timer_id_t id, uint32_t time_us) {
	//Get current time
	uint32_t tim_count = __HAL_TIM_GET_COUNTER(&htim2);

	//Set end time and status
	my_polling_timer_t timer;
	timer.end_count = tim_count + time_us / TICK_TIME;
	timer.state = TIMER_RUNNING;

	//Append timer
	g_timers[id] = timer;
}
