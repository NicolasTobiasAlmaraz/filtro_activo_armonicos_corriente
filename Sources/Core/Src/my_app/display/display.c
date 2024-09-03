/**
 * @file display.h
 * @author Nicolás Almaraz
 * @brief Actualización de display con mensajes al usuario
 */

//======================================
// Dependencias
//======================================
#include "main.h"
#include "display.h"
#include "display_driver/LCDI2C.h"

//======================================
// Defines Privados
//======================================

//======================================
// Estructuras y Tipos de Datos Privados
//======================================

//======================================
// Variables Privadas
//======================================

//======================================
// Declaración de Funciones Privadas
//======================================

//======================================
// Implementación de Funciones Privadas
//======================================

//======================================
// Implementación de Funciones Públicas
//======================================

void display_init() {
	LCD_Init();	//Initialization

	LCD_setCursor(0,0);
	LCD_sendSTR("N. Almaraz - TP3:");

	LCD_setCursor(1,0);
	LCD_sendSTR("Filt Act Armon i(t)");

	LCD_setCursor(2,0);
	LCD_sendSTR("Corriente de liena");

	LCD_setCursor(3,0);
	LCD_sendSTR("THD: XX %");
}

void display_update_THD(uint8_t thd) {
	if(thd >= 100)
		thd=99;

	LCD_setCursor(3,5);
	char numero[2];
	numero[0] = '0'+thd/10;
	numero[1] = '0'+thd%10;
	LCD_sendSTR(numero);
}
