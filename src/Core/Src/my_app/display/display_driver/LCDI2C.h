/* 	LCD Library
	STM32Fxx - HAL STM32CubeIDE
	Author : Fattah .A
	Electronics Engineering | State Polytechnic of Malang
	26 Jan. 2021
*/

#ifndef _LCDI2C_H_
#define _LCDI2C_H_

#include "stm32f4xx_hal.h"

void LCD_initID(uint8_t _ADDR);
void LCD_Init();
void LCD_sendCMD (char cmd);
void LCD_sendData (char data);
void LCD_sendSTR(char *str);
void LCD_Clear (void);
void LCD_setCursor(uint8_t x, uint8_t y);

#endif
