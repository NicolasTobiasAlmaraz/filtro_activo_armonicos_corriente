/**
 * @file LCDI2C.c
 * @brief Driver LCD Display with bridge I2C
 * @author Nicolas Almaraz
 */

//======================================
// Dependencias
//======================================
#include "main.h"
#include "LCDI2C.h"

//======================================
// Defines Privados
//======================================

/**
 * @brief Dirección I2C del display LCD.
 * Esta dirección depende de cómo esté configurado el pin A0 del LCD.
 */
#define ADDR 0x4E

//======================================
// Estructuras y Tipos de Datos Privados
//======================================

//======================================
// Variables Privadas
//======================================

//======================================
// Handlers STM32
//======================================
extern I2C_HandleTypeDef hi2c1;  //!< Handler del periférico I2C utilizado

//======================================
// Declaración de Funciones Privadas
//======================================

/**
 * @brief Envía un comando al display LCD.
 * Esta función envía comandos de configuración al display,
 * como el ajuste de modo, encendido, apagado, etc.
 * @param cmd Comando a enviar.
 */
void LCD_sendCMD (char cmd);

/**
 * @brief Envía un dato al display LCD.
 * Esta función se utiliza para enviar caracteres a mostrar en el LCD.
 * @param data Dato (carácter) a enviar.
 */
void LCD_sendData (char data);

//======================================
// Implementación de Funciones Privadas
//======================================

void LCD_sendCMD (char cmd) {
    char _U, _L;
    uint8_t _T[4];

    _U = (cmd & 0xf0);           // Más significativo del comando
    _L = ((cmd << 4) & 0xf0);    // Menos significativo del comando

    // Configuración de los 4 bits altos y bajos
    _T[0] = _U | 0x0C;
    _T[1] = _U | 0x08;
    _T[2] = _L | 0x0C;
    _T[3] = _L | 0x08;

    // Transmisión del comando a través de I2C
    HAL_I2C_Master_Transmit(&hi2c1, ADDR, (uint8_t *) _T, 4, 100);
}

void LCD_sendData (char data) {
    char _U, _L;
    uint8_t _T[4];

    _U = (data & 0xf0);          //!< Más significativo del dato
    _L = ((data << 4) & 0xf0);   //!< Menos significativo del dato

    // Configuración de los 4 bits altos y bajos
    _T[0] = _U | 0x0D;
    _T[1] = _U | 0x09;
    _T[2] = _L | 0x0D;
    _T[3] = _L | 0x09;

    // Transmisión del dato a través de I2C
    HAL_I2C_Master_Transmit(&hi2c1, ADDR, (uint8_t *) _T, 4, 100);
}

//======================================
// Implementación de Funciones Públicas
//======================================

void LCD_Clear (void) {
    LCD_sendData(0x00); //!< Posiciono en el inicio del display

    for (int i = 0; i < 100; i++) {
        LCD_sendData(' '); //!< Lleno el display con espacios
    }
}

void LCD_Init () {
    // Secuencia de inicialización en 4 bits
    HAL_Delay(50);              //!< Espera de estabilización
    LCD_sendCMD(0x30);          //!< Comando de inicialización
    HAL_Delay(5);
    LCD_sendCMD(0x30);
    HAL_Delay(1);
    LCD_sendCMD(0x30);
    HAL_Delay(10);
    LCD_sendCMD(0x20);          //!< Configuración a 4 bits
    HAL_Delay(10);

    // Configuraciones adicionales del LCD
    LCD_sendCMD(0x28);          // Configuración del LCD en modo 4 bits, 2 líneas
    HAL_Delay(1);
    LCD_sendCMD(0x08);          // Apaga el display
    HAL_Delay(1);
    LCD_sendCMD(0x01);          // Limpia el display
    HAL_Delay(1);
    HAL_Delay(1);
    LCD_sendCMD(0x06);          // Modo de entrada: incremento automático, sin desplazamiento
    HAL_Delay(1);
    LCD_sendCMD(0x0C);          // Enciende el display, sin cursor y sin parpadeo
}

void LCD_sendSTR(char *str) {
	//Envía cada carácter de la cadena
    while (*str) {
    	LCD_sendData(*str++);
    }
}

void LCD_setCursor(uint8_t renglon, uint8_t letra) {
    uint8_t cursor;

    // Determina la dirección del cursor basada en la fila y la columna
    switch (renglon) {
        case 0:
            cursor = 0x00 + letra;
            break;
        case 1:
            cursor = 0x40 + letra;
            break;
        case 2:
            cursor = 0x14 + letra;
            break;
        case 3:
            cursor = 0x54 + letra;
            break;
    }

    // Envía el comando para posicionar el cursor
    LCD_sendCMD(0x80 | cursor);
}
