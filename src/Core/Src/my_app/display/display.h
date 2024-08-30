/**
 * @file display.h
 * @author Nicolás Almaraz
 * @brief Actualización de display con mensajes al usuario
 */

#ifndef SRC_SUBSYSTEMS_DISPLAY_LOGS_H_
#define SRC_SUBSYSTEMS_DISPLAY_LOGS_H_

//======================================
// Dependencias
//======================================
#include <stdbool.h>
#include "main.h"

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
 * @brief
 */
void display_init();

/**
 * @brief
 */
void display_update_THD(uint8_t thd);

/**
 * @brief
 */
void display_loop();

#endif /* SRC_SUBSYSTEMS_DISPLAY_LOGS_H_ */
