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
// Declaración de Funciones Públicas
//======================================

/**
 * @brief Inicializa el display (perifericos y mensajes estáticos)
 */
void display_init();

/**
 * @brief Actualiza el valor de THD en el display
 */
void display_update_THD(uint8_t thd);


#endif /* SRC_SUBSYSTEMS_DISPLAY_LOGS_H_ */
