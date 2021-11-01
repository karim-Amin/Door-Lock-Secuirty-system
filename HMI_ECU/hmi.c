/******************************************************************************
 *
 * Module: HMI ECU main function
 *
 * File Name: hmi.c
 *
 * Description: to contain the app functions for the human machine interface
 *
 * Author: Karim Mohamed Amin
 *
 *******************************************************************************/
#include "keypad.h"
#include "LCD.h"
#include "timer.h"
#include "uart.h"
#include <avr/io.h> /* to enable the global interrupt*/
#include <util/delay.h>
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define CONTROL_ECU_READY 0x10
#define CONTROL_PASSWORD_MATCH 0x11
#define CONTROL_PASSWORD_DISMATCH 0x00
#define HMI_ECU_READY 0x20
#define NUM_OF_PASSWORD_DIGIT 5
#define HMI_BAUD_RATE 9600
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void HMI_takePassword(uint8* password_ptr);
void HMI_sendPasswordToControl(const uint8* password_ptr);
void HMI_setPasswordFirstTime(uint8* a_first_password_ptr,uint8* a_second_password_ptr);
uint8 HMI_receiveStatus();
/*******************************************************************************
 *                             The entry point (main method)                  *
 *******************************************************************************/
int main(void){
	/*to hold the password taken from the user*/
	uint8 first_password_buff[NUM_OF_PASSWORD_DIGIT];
	uint8 second_password_buff[NUM_OF_PASSWORD_DIGIT];
	uint8 status;
	config_struct s_uart_config = {no_parity,eigth_bits,one_stop_bit,Asynch,HMI_BAUD_RATE};
	/*configType s_timer_config ={};*/
	LCD_init();
	UART_init(&s_uart_config);
	/*set the I-bit to be able to use the timer driver*/
	SREG |= (1<<7);
	while(1){
		HMI_setPasswordFirstTime(first_password_buff,second_password_buff);
		status = HMI_receiveStatus();
		if(status == CONTROL_PASSWORD_MATCH){
			LCD_displayString("verified");
		}else{
			LCD_displayString("not verified");

		}
	}
	return 0;
}
/*******************************************************************************
 *                              Functions Definitions                           *
 *******************************************************************************/
void HMI_takePassword(uint8* password_ptr){
	/*to loop throw the password*/
	uint8 count;
	/*taking the password form keypad byte by byte until the NUM_OF_PASSWORD_DIGIT */
	/*for each byte ,i will display "*"corresponding to the byte to save the privacy */
	for(count = 0;count<NUM_OF_PASSWORD_DIGIT;count++){
		password_ptr[count] = KEYPAD_getPressedKey();
		LCD_displayStringRowColumn(1,count,"*");
		_delay_ms(250);
	}
}
void HMI_sendPasswordToControl(const uint8* password_ptr){
	uint8 count;
	UART_sendByte(HMI_ECU_READY);
	/*wait until the control ECU is ready to read the password*/
	while(UART_recieveByte()!= CONTROL_ECU_READY);
	/*send the password to control ECU by UART driver byte by byte*/
	for(count = 0;count<NUM_OF_PASSWORD_DIGIT;count++){
			UART_sendByte(password_ptr[count]);
		}
}
void HMI_setPasswordFirstTime(uint8* a_first_password_ptr,uint8* a_second_password_ptr){
	LCD_displayStringRowColumn(0,0,"Please Enter pass: ");
	HMI_takePassword(a_first_password_ptr);
	LCD_clear();
	LCD_displayStringRowColumn(0,0,"Please re-enter pass: ");
	HMI_takePassword(a_second_password_ptr);
	LCD_clear();
	HMI_sendPasswordToControl(a_first_password_ptr);
	HMI_sendPasswordToControl(a_second_password_ptr);
}
uint8 HMI_receiveStatus(){
	/*wait until the control ECU is ready to send the status*/
	while(UART_recieveByte()!= CONTROL_ECU_READY);
	UART_sendByte(HMI_ECU_READY);
	return UART_recieveByte();
}
