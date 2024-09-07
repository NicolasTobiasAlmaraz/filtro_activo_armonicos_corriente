/**
 * @file LCDI2C.h
 * @brief Driver LCD Display with bridge I2C
 * @author Nicolas Almaraz
 */

#ifndef _LCDI2C_H_
#define _LCDI2C_H_

/**
 * @brief Inicializa el display LCD.
 *
 * Esta función realiza la secuencia de inicialización para el modo de 4 bits.
 * Es necesaria para preparar el LCD para la comunicación y el uso adecuado.
 */
void display_driver_init();

/**
 * @brief Envía una cadena de caracteres al display LCD.
 * @param str Cadena de caracteres a enviar.
 */
void display_driver_send_string(char *str);

/**
 * @brief Limpia el display LCD.
 *
 * Esta función borra el contenido del display LCD posicionando el cursor al inicio
 * y llenando la pantalla con espacios en blanco.
 */
void display_driver_clear (void);

/**
 * @brief Posiciona el cursor del display en una fila y columna específicas. *
 * @param renglon Fila donde posicionar el cursor (0-3).
 * @param letra Columna donde posicionar el cursor (0-19).
 */
void display_driver_set_cursor(uint8_t renglon, uint8_t letra);

#endif
