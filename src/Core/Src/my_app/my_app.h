/**
 * @file my_app.h
 * @author Nicolás Almaraz
 * @brief Lógica a nivel sistema
 */

#ifndef SRC_SYSTEM_MY_APP_H_
#define SRC_SYSTEM_MY_APP_H_

//======================================
// Dependencias
//======================================
#include "main.h"

//======================================
// Defines Públicos
//======================================

//======================================
// Estructuras y Tipos de datos públicos
//======================================
typedef struct {
	I2C_HandleTypeDef *hi2c;
} peripherals_handler_t;

//======================================
// Variables Globales
//======================================

//======================================
// Declaración de Funciones Privadas
//======================================


/**
 * @brief Hace la inicializacion de todo el sistema
 */
void my_app_init(peripherals_handler_t handlers);

/**
 * @brief Tiene el main loop de toda la app
 */
void my_app_loop();

#endif /* SRC_SYSTEM_MY_APP_H_ */
