/**
 * @file signal_analyzer.h
 * @author Nicolás Almaraz
 * @brief Procesamiento principal de señales para obtener THD e ic(t)
 */


#ifndef SRC_MY_APP_SIGNAL_ANALYZER_SIGNAL_ANALYZER_H_
#define SRC_MY_APP_SIGNAL_ANALYZER_SIGNAL_ANALYZER_H_

//======================================
// Dependencias
//======================================
#include "main.h"

#include "current_sensor/current_sensor_api.h"
//======================================
// Defines Públicos
//======================================

//======================================
// Estructuras y Tipos de Datos Públicos
//======================================

//======================================
// Variables Globales
//======================================

//======================================
// Declaración de Funciones Públicas
//======================================

/**
 * @brief Inicializa la API
 */
void signal_analyzer_api_init();

/**
 * @brief Analyzes the signal
 * @param cycles Samples of each cycle
 * @param len Quantity of cycles
 */
void signal_analyzer_api_start_new_analyze(cycle_t *cycles, uint32_t len, uint16_t zero_offset);

/**
 *
 */
uint8_t signal_analyzer_api_get_thd();

/**
 *@brief
 */
cycle_t signal_analyzer_api_get_inject_current();

/**
 * @brief
 */
void signal_analyzer_api_set_fs(uint32_t fs);

#endif /* SRC_MY_APP_SIGNAL_ANALYZER_SIGNAL_ANALYZER_H_ */
