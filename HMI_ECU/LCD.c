<<<<<<< HEAD
 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for the AVR LCD driver
 *
 * Author: Karim Mohamed
 *
 *******************************************************************************/
#include "LCD.h"
#include "gpio.h"
#include "common_macros.h"
#include "util/delay.h"
#include "stdlib.h"
/*
 * Description :
 * sends the command to take some actions on the lcd device like clear , move and turn on the cursor
 * the delay used to achieve the requirment of the timming diagram in data sheet
 */
void LCD_sendCommand(uint8 a_command){
	GPIO_writePin(LCD_CONTROL_PORT_ID,LCD_RS_PIN_ID,LOGIC_LOW);// RS = 0
	GPIO_writePin(LCD_CONTROL_PORT_ID,LCD_RW_PIN_ID,LOGIC_LOW);// R/W = 0
	_delay_ms(1);
	GPIO_writePin(LCD_CONTROL_PORT_ID,LCD_E_PIN_ID,LOGIC_HIGH);// enable =1
	_delay_ms(1);
	GPIO_writePort(LCD_DATA_PORT_ID,a_command);// write the command on portc
	_delay_ms(1);
	GPIO_writePin(LCD_CONTROL_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);// enable =0
	_delay_ms(1);
}
/*
 * Description :
 * sets up the directions for the pins which are connected to the LCD
 * write some commands to initailize the LCD
 */
void LCD_init(void){
	GPIO_setupPinDirection(LCD_CONTROL_PORT_ID,LCD_RS_PIN_ID,PIN_OUTPUT);// pd4 is output pin
	GPIO_setupPinDirection(LCD_CONTROL_PORT_ID,LCD_RW_PIN_ID,PIN_OUTPUT);// pd5 is output pin
	GPIO_setupPinDirection(LCD_CONTROL_PORT_ID,LCD_E_PIN_ID,PIN_OUTPUT);// pd6 is output pin
	GPIO_setupPortDirection(LCD_DATA_PORT_ID,PORT_OUTPUT);// for data or command
	LCD_sendCommand(Eight_BITS_COMMAND);// 2 lines + 8 bits data
	LCD_sendCommand(CURSOR_OFF_COMMAND);// cursor off
	LCD_sendCommand(CLEAR_COMMAND);// clear screen

}
/*
 * Description:
 * 	this function takes a string as input
 * 	and display this string on LCD screen
 */
void LCD_displayString(const uint8* a_data_ptr){
	while((*a_data_ptr) != '\0'){
		LCD_displayCharacter(*a_data_ptr);
		a_data_ptr = a_data_ptr +1;// move ptr
	}
}
/*
 * Description :
 * takes a character to be displayed on the screen
 *
 */
void LCD_displayCharacter(uint8 a_data){
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_RS_PIN_ID,LOGIC_HIGH);// RS = 1
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_RW_PIN_ID,LOGIC_LOW);// R/W = 0
	_delay_ms(1);
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_E_PIN_ID,LOGIC_HIGH);// enable =1
	_delay_ms(1);
	GPIO_writePort(LCD_DATA_PORT_ID,a_data);// write the command on portc
	_delay_ms(1);
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_E_PIN_ID,LOGIC_LOW);// enable =0
	_delay_ms(1);
}
/*
 * Description :
 * 	moves the cursor to specific location measured by the row and the col
 *
 */
void LCD_moveCursor(uint8 a_row,uint8 a_col){
	uint8 lcd_address_location;
	switch(a_row){
	default:
	case 0: lcd_address_location = a_col; // as the start address in lcd 0x00 until 0x0f
		break;
	case 1: lcd_address_location = a_col + SECOND_ROW_START_ADDRESS;
		break;
	case 2: lcd_address_location = a_col + THIRD_ROW_START_ADDRESS;
		break;
	case 3: lcd_address_location = a_col + FOURTH_ROW_START_ADDRESS;
		break;
	}
	LCD_sendCommand(lcd_address_location | SET_CURSOR_LOCATION_COMMAND);// this line will move the cursor to
	// the calculated location
}
/*
 * Description:
 * 	display a string at certain row and column
 */
void LCD_displayStringRowColumn(uint8 a_row,uint8 a_col,const uint8* a_data_ptr){
	LCD_moveCursor(a_row,a_col);
	LCD_displayString(a_data_ptr);
}
/*
 * Description:
 * 	this function converts a number to string and display it on LCD
 */
void LCD_intgerToString(const uint32 a_data){
	uint8 buff[16];
	itoa(a_data,buff,10);
	LCD_displayString(buff);
}
/*
 * Description :
 * 		clears the screen of the LCD
 *
 */
void LCD_clear(void){
	LCD_sendCommand(CLEAR_COMMAND);
}
=======
 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for the AVR LCD driver
 *
 * Author: Karim Mohamed
 *
 *******************************************************************************/
#include "LCD.h"
#include "gpio.h"
#include "common_macros.h"
#include "util/delay.h"
#include "stdlib.h"
/*
 * Description :
 * sends the command to take some actions on the lcd device like clear , move and turn on the cursor
 * the delay used to achieve the requirment of the timming diagram in data sheet
 */
void LCD_sendCommand(uint8 a_command){
	GPIO_writePin(LCD_CONTROL_PORT_ID,LCD_RS_PIN_ID,LOGIC_LOW);// RS = 0
	GPIO_writePin(LCD_CONTROL_PORT_ID,LCD_RW_PIN_ID,LOGIC_LOW);// R/W = 0
	_delay_ms(1);
	GPIO_writePin(LCD_CONTROL_PORT_ID,LCD_E_PIN_ID,LOGIC_HIGH);// enable =1
	_delay_ms(1);
	GPIO_writePort(LCD_DATA_PORT_ID,a_command);// write the command on portc
	_delay_ms(1);
	GPIO_writePin(LCD_CONTROL_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);// enable =0
	_delay_ms(1);
}
/*
 * Description :
 * sets up the directions for the pins which are connected to the LCD
 * write some commands to initailize the LCD
 */
void LCD_init(void){
	GPIO_setupPinDirection(LCD_CONTROL_PORT_ID,LCD_RS_PIN_ID,PIN_OUTPUT);// pd4 is output pin
	GPIO_setupPinDirection(LCD_CONTROL_PORT_ID,LCD_RW_PIN_ID,PIN_OUTPUT);// pd5 is output pin
	GPIO_setupPinDirection(LCD_CONTROL_PORT_ID,LCD_E_PIN_ID,PIN_OUTPUT);// pd6 is output pin
	GPIO_setupPortDirection(LCD_DATA_PORT_ID,PORT_OUTPUT);// for data or command
	LCD_sendCommand(Eight_BITS_COMMAND);// 2 lines + 8 bits data
	LCD_sendCommand(CURSOR_OFF_COMMAND);// cursor off
	LCD_sendCommand(CLEAR_COMMAND);// clear screen

}
/*
 * Description:
 * 	this function takes a string as input
 * 	and display this string on LCD screen
 */
void LCD_displayString(const uint8* a_data_ptr){
	while((*a_data_ptr) != '\0'){
		LCD_displayCharacter(*a_data_ptr);
		a_data_ptr = a_data_ptr +1;// move ptr
	}
}
/*
 * Description :
 * takes a character to be displayed on the screen
 *
 */
void LCD_displayCharacter(uint8 a_data){
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_RS_PIN_ID,LOGIC_HIGH);// RS = 1
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_RW_PIN_ID,LOGIC_LOW);// R/W = 0
	_delay_ms(1);
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_E_PIN_ID,LOGIC_HIGH);// enable =1
	_delay_ms(1);
	GPIO_writePort(LCD_DATA_PORT_ID,a_data);// write the command on portc
	_delay_ms(1);
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_E_PIN_ID,LOGIC_LOW);// enable =0
	_delay_ms(1);
}
/*
 * Description :
 * 	moves the cursor to specific location measured by the row and the col
 *
 */
void LCD_moveCursor(uint8 a_row,uint8 a_col){
	uint8 lcd_address_location;
	switch(a_row){
	default:
	case 0: lcd_address_location = a_col; // as the start address in lcd 0x00 until 0x0f
		break;
	case 1: lcd_address_location = a_col + SECOND_ROW_START_ADDRESS;
		break;
	case 2: lcd_address_location = a_col + THIRD_ROW_START_ADDRESS;
		break;
	case 3: lcd_address_location = a_col + FOURTH_ROW_START_ADDRESS;
		break;
	}
	LCD_sendCommand(lcd_address_location | SET_CURSOR_LOCATION_COMMAND);// this line will move the cursor to
	// the calculated location
}
/*
 * Description:
 * 	display a string at certain row and column
 */
void LCD_displayStringRowColumn(uint8 a_row,uint8 a_col,const uint8* a_data_ptr){
	LCD_moveCursor(a_row,a_col);
	LCD_displayString(a_data_ptr);
}
/*
 * Description:
 * 	this function converts a number to string and display it on LCD
 */
void LCD_intgerToString(const uint32 a_data){
	uint8 buff[16];
	itoa(a_data,buff,10);
	LCD_displayString(buff);
}
/*
 * Description :
 * 		clears the screen of the LCD
 *
 */
void LCD_clear(void){
	LCD_sendCommand(CLEAR_COMMAND);
}
>>>>>>> main program structure
