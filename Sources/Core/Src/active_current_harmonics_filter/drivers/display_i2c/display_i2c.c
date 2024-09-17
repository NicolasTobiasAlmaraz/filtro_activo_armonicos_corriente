/**
 * @file display_i2c.c
 * @brief LCD Display Driver with I2C bridge
 * @author Nicolas Almaraz
 */

//======================================
// Dependencies
//======================================
#include "display_i2c.h"

//======================================
// Private Defines
//======================================
#define ADDR 0x4E	//!< I2C address of the LCD display (depends of A0)

//======================================
// Private Structures and Data Types
//======================================

//======================================
// Private Variables
//======================================

//======================================
// STM32 Handlers
//======================================
extern I2C_HandleTypeDef hi2c1;  //!< I2C peripheral handler (declared on main.c)

//======================================
// Private Function Declarations
//======================================

/**
 * @brief Sends a command to the LCD display.
 * @param cmd Command to send.
 */
static void m_send_cmd (char cmd);

/**
 * @brief Sends data to the LCD display.
 * @param data Data (character) to send.
 */
static void display_driver_send_data (char data);

//======================================
// Private Function Implementations
//======================================

void m_send_cmd (char cmd) {
    char _U, _L;
    uint8_t _T[4];

    _U = (cmd & 0xf0);           // Most significant part of the command
    _L = ((cmd << 4) & 0xf0);    // Least significant part of the command

    // Setting up the high and low 4 bits
    _T[0] = _U | 0x0C;
    _T[1] = _U | 0x08;
    _T[2] = _L | 0x0C;
    _T[3] = _L | 0x08;

    // Sending the command via I2C
    HAL_I2C_Master_Transmit(&hi2c1, ADDR, (uint8_t *) _T, 4, 100);
}

void display_driver_send_data (char data) {
    char _U, _L;
    uint8_t _T[4];

    _U = (data & 0xf0);          //!< Most significant part of the data
    _L = ((data << 4) & 0xf0);   //!< Least significant part of the data

    // Setting up the high and low 4 bits
    _T[0] = _U | 0x0D;
    _T[1] = _U | 0x09;
    _T[2] = _L | 0x0D;
    _T[3] = _L | 0x09;

    // Sending the data via I2C
    HAL_I2C_Master_Transmit(&hi2c1, ADDR, (uint8_t *) _T, 4, 100);
}

//======================================
// Public Function Implementations
//======================================

void display_i2c_clear (void) {
    display_driver_send_data(0x00); //!< Set cursor to the start of the display

    for (int i = 0; i < 100; i++) {
        display_driver_send_data(' '); //!< Fill the display with spaces
    }
}

void display_i2c_init () {
    // 4-bit initialization sequence
    HAL_Delay(50);              //!< Stabilization wait
    m_send_cmd(0x30);          //!< Initialization command
    HAL_Delay(5);
    m_send_cmd(0x30);
    HAL_Delay(1);
    m_send_cmd(0x30);
    HAL_Delay(10);
    m_send_cmd(0x20);          //!< Set to 4-bit mode
    HAL_Delay(10);

    // Additional LCD configurations
    m_send_cmd(0x28);          // LCD 4-bit mode, 2-line configuration
    HAL_Delay(1);
    m_send_cmd(0x08);          // Turn off the display
    HAL_Delay(1);
    m_send_cmd(0x01);          // Clear the display
    HAL_Delay(1);
    HAL_Delay(1);
    m_send_cmd(0x06);          // Input mode: auto increment, no shift
    HAL_Delay(1);
    m_send_cmd(0x0C);          // Turn on the display, no cursor, no blinking
}

void display_i2c_send_string(char *str) {
    // Send each character of the string
    while (*str) {
        display_driver_send_data(*str++);
    }
}

void display_i2c_set_cursor(uint8_t row, uint8_t column) {
    uint8_t cursor;

    // Determine the cursor address based on the row and column
    switch (row) {
        case 0:
            cursor = 0x00 + column;
            break;
        case 1:
            cursor = 0x40 + column;
            break;
        case 2:
            cursor = 0x14 + column;
            break;
        case 3:
            cursor = 0x54 + column;
            break;
    }

    // Send the command to set the cursor position
    m_send_cmd(0x80 | cursor);
}
