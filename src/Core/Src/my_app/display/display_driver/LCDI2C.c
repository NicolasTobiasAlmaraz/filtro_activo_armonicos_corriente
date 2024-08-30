#include "LCDI2C.h"

#define i2c_handler	&hi2c1			//Change this line based on your I2C
#define ADDR 0x4E

extern I2C_HandleTypeDef hi2c1;		//Also this line :)


void LCD_sendCMD (char cmd) {
	char _U, _L;
	uint8_t _T[4];
	_U = (cmd&0xf0);
	_L = ((cmd<<4)&0xf0);
	_T[0] = _U|0x0C;
	_T[1] = _U|0x08;
	_T[2] = _L|0x0C;
	_T[3] = _L|0x08;
	HAL_I2C_Master_Transmit (i2c_handler, ADDR,(uint8_t *) _T, 4, 100);
}

void LCD_sendData (char data) {
	char _U, _L;
	uint8_t _T[4];
	_U = (data&0xf0);
	_L = ((data<<4)&0xf0);
	_T[0] = _U|0x0D;
	_T[1] = _U|0x09;
	_T[2] = _L|0x0D;
	_T[3] = _L|0x09;
	HAL_I2C_Master_Transmit (i2c_handler, ADDR,(uint8_t *) _T, 4, 100);
}

void LCD_Clear (void) {
	LCD_sendData (0x00);
	for (int i=0; i<100; i++) {
		LCD_sendData (' ');
	}
}

void LCD_Init () {
	HAL_Delay(50);
	LCD_sendCMD (0x30);
	HAL_Delay(5);
	LCD_sendCMD (0x30);
	HAL_Delay(1);
	LCD_sendCMD (0x30);
	HAL_Delay(10);
	LCD_sendCMD (0x20);
	HAL_Delay(10);

	LCD_sendCMD (0x28);
	HAL_Delay(1);
	LCD_sendCMD (0x08);
	HAL_Delay(1);
	LCD_sendCMD (0x01);
	HAL_Delay(1);
	HAL_Delay(1);
	LCD_sendCMD (0x06);
	HAL_Delay(1);
	LCD_sendCMD (0x0C);
}

void LCD_sendSTR(char *str) {
	while (*str) LCD_sendData (*str++);
}

void LCD_setCursor(uint8_t x, uint8_t y) {
	uint8_t cursor;
	switch (x) {
	case 0 :
		switch (y) {
		case 0 : cursor = 0x00; break;
		case 1 : cursor = 0x01; break;
		case 2 : cursor = 0x02; break;
		case 3 : cursor = 0x03; break;
		case 4 : cursor = 0x04; break;
		case 5 : cursor = 0x05; break;
		case 6 : cursor = 0x06; break;
		case 7 : cursor = 0x07; break;
		case 8 : cursor = 0x08; break;
		case 9 : cursor = 0x09; break;
		case 10 : cursor = 0x0A; break;
		case 11 : cursor = 0x0B; break;
		case 12 : cursor = 0x0C; break;
		case 13 : cursor = 0x0D; break;
		case 14 : cursor = 0x0E; break;
		case 15 : cursor = 0x0F; break;
		case 16 : cursor = 0x10; break;
		case 17 : cursor = 0x11; break;
		case 18 : cursor = 0x12; break;
		case 19 : cursor = 0x13; break;
		} break;

	case 1 :
		switch (y) {
		case 0 : cursor = 0x40; break;
		case 1 : cursor = 0x41; break;
		case 2 : cursor = 0x42; break;
		case 3 : cursor = 0x43; break;
		case 4 : cursor = 0x44; break;
		case 5 : cursor = 0x45; break;
		case 6 : cursor = 0x46; break;
		case 7 : cursor = 0x47; break;
		case 8 : cursor = 0x48; break;
		case 9 : cursor = 0x49; break;
		case 10 : cursor = 0x4A; break;
		case 11 : cursor = 0x4B; break;
		case 12 : cursor = 0x4C; break;
		case 13 : cursor = 0x4D; break;
		case 14 : cursor = 0x4E; break;
		case 15 : cursor = 0x4F; break;
		case 16 : cursor = 0x50; break;
		case 17 : cursor = 0x51; break;
		case 18 : cursor = 0x52; break;
		case 19 : cursor = 0x53; break;
		} break;

	case 2 :
		switch (y) {
		case 0 : cursor = 0x14; break;
		case 1 : cursor = 0x15; break;
		case 2 : cursor = 0x16; break;
		case 3 : cursor = 0x17; break;
		case 4 : cursor = 0x18; break;
		case 5 : cursor = 0x19; break;
		case 6 : cursor = 0x1A; break;
		case 7 : cursor = 0x1B; break;
		case 8 : cursor = 0x1C; break;
		case 9 : cursor = 0x1D; break;
		case 10 : cursor = 0x1E; break;
		case 11 : cursor = 0x1F; break;
		case 12 : cursor = 0x20; break;
		case 13 : cursor = 0x21; break;
		case 14 : cursor = 0x22; break;
		case 15 : cursor = 0x23; break;
		case 16 : cursor = 0x24; break;
		case 17 : cursor = 0x25; break;
		case 18 : cursor = 0x26; break;
		case 19 : cursor = 0x27; break;
		} break;

	case 3 :
		switch (y) {
		case 0 : cursor = 0x54; break;
		case 1 : cursor = 0x55; break;
		case 2 : cursor = 0x56; break;
		case 3 : cursor = 0x57; break;
		case 4 : cursor = 0x58; break;
		case 5 : cursor = 0x59; break;
		case 6 : cursor = 0x5A; break;
		case 7 : cursor = 0x5B; break;
		case 8 : cursor = 0x5C; break;
		case 9 : cursor = 0x5D; break;
		case 10 : cursor = 0x5E; break;
		case 11 : cursor = 0x5F; break;
		case 12 : cursor = 0x60; break;
		case 13 : cursor = 0x61; break;
		case 14 : cursor = 0x62; break;
		case 15 : cursor = 0x63; break;
		case 16 : cursor = 0x64; break;
		case 17 : cursor = 0x65; break;
		case 18 : cursor = 0x66; break;
		case 19 : cursor = 0x67; break;
		} break;
	}
	LCD_sendCMD(0x80|cursor);
}
