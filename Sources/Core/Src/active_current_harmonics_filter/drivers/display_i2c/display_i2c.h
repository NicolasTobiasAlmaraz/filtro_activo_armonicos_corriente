/**
 * @file display_i2c.h
 * @brief LCD Display Driver with I2C bridge
 * @author Nicolás Almaraz
 */

#ifndef DISPLAY_I2C_H_
#define DISPLAY_I2C_H_

//======================================
// Dependencies
//======================================
#include "main.h"

//======================================
// Public Defines
//======================================

//======================================
// Public Structures and Data Types
//======================================

//======================================
// Public Function Declarations
//======================================

/**
 * @brief Initializes the LCD display.
 *
 * This function performs the initialization sequence for 4-bit mode.
 * It is necessary to prepare the LCD for communication and proper use.
 */
void display_i2c_init();

/**
 * @brief Sends a string of characters to the LCD display.
 * @param str String of characters to send.
 */
void display_i2c_send_string(char *str);

/**
 * @brief Clears the LCD display.
 *
 * This function clears the content of the LCD display, positions the cursor at the start,
 * and fills the screen with blank spaces.
 */
void display_i2c_clear (void);

/**
 * @brief Positions the display cursor at a specific row and column.
 * @param row Row to position the cursor (0-3).
 * @param column Column to position the cursor (0-19).
 */
void display_i2c_set_cursor(uint8_t row, uint8_t column);

#endif
