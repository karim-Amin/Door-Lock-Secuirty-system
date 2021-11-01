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
 *                                global variables                            *
 *******************************************************************************/
/*to hold the status of the passwords sent by control ECU*/
uint8 g_status;
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define CONTROL_ECU_READY 0x10
#define CONTROL_PASSWORD_MATCH 0x11
#define CONTROL_PASSWORD_DISMATCH 0x00
#define HMI_ECU_READY 0x20
#define NUM_OF_PASSWORD_DIGIT 5
#define HMI_BAUD_RATE 9600
#define OPEN_DOOR_OPTION '+'
#define CHANGE_PASSWORD_OPTION '-'
#define ERROR_MESSAGE 0xFF
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void HMI_takePassword(uint8* password_ptr);
void HMI_sendPasswordToControl(const uint8* password_ptr);
void HMI_setPasswordFirstTime(uint8* a_first_password_ptr,uint8* a_second_password_ptr);
void HMI_displayMainOptions(void);
void HMI_setAndCheckStatus(uint8* a_first_password_ptr,uint8* a_second_password_ptr);
void HMI_handleOptions(uint8* password_ptr);
uint8 HMI_takeOption(void);
uint8 HMI_receiveStatus(void);
/*******************************************************************************
 *                             The entry point (main method)                  *
 *******************************************************************************/
int main(void){
	/*to hold the first_password taken from the user*/
	uint8 first_password_buff[NUM_OF_PASSWORD_DIGIT];F_CPU;
	/*to hold the second_password taken from the user*/
	uint8 second_password_buff[NUM_OF_PASSWORD_DIGIT];

	/*setup the UART configuration*/
	config_struct s_uart_config = {no_parity,eigth_bits,one_stop_bit,Asynch,HMI_BAUD_RATE};

	/* 	calling the init functions for each driver */
	LCD_init();
	UART_init(&s_uart_config);
	/*set the I-bit to be able to use the timer driver*/
	SREG |= (1<<7);

	while(1){
		/************************************************************************************************/
												/*step1 code*/
		/************************************************************************************************/
		HMI_setAndCheckStatus(first_password_buff,second_password_buff);
		/************************************************************************************************/
												/*step2 code*/
		/************************************************************************************************/
		HMI_handleOptions(first_password_buff);
		/************************************************************************************************/

	}
	return 0;
}
/*******************************************************************************
 *                              Functions Definitions                           *
 *******************************************************************************/
/*
 * Description : taking the password form keypad byte by byte until the NUM_OF_PASSWORD_DIGIT
 * for each byte ,i will display "*"corresponding to the byte to save the privacy
 */
void HMI_takePassword(uint8* password_ptr){
	/*to loop throw the password*/
	uint8 count;
	/*taking the password form keypad byte by byte until the NUM_OF_PASSWORD_DIGIT */
	/*for each byte ,i will display "*"corresponding to the byte to save the privacy */
	for(count = 0;count<NUM_OF_PASSWORD_DIGIT;count++){
		password_ptr[count] = KEYPAD_getPressedKey();
		LCD_displayStringRowColumn(1,count,"*");
		/*to avoid taking the pressed number more than one time*/
		_delay_ms(400);
	}
}
/*
 * Description : this function send the password to control ECU if it is ready to receive
 */
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
/*
 * Description : used to take to passwords and send them
 */
void HMI_setPasswordFirstTime(uint8* a_first_password_ptr,uint8* a_second_password_ptr){
	LCD_clear();
	/* take the first one*/
	LCD_displayStringRowColumn(0,0,"Please Enter password : ");
	HMI_takePassword(a_first_password_ptr);
	LCD_clear();
	/*take the second one*/
	LCD_displayStringRowColumn(0,0,"Please re-enter password: ");
	HMI_takePassword(a_second_password_ptr);
	LCD_clear();
	/*send them all*/
	HMI_sendPasswordToControl(a_first_password_ptr);
	HMI_sendPasswordToControl(a_second_password_ptr);
}
void HMI_setAndCheckStatus(uint8* a_first_password_ptr,uint8* a_second_password_ptr){
	while(1)
	{
		HMI_setPasswordFirstTime(a_first_password_ptr,a_second_password_ptr);
		g_status = HMI_receiveStatus();
		if(g_status == CONTROL_PASSWORD_MATCH){
			LCD_displayString("Correct!!");
			_delay_ms(500);/*to be able to see the message*/
			break;/*go to the next step*/
		}else{
			/*i will repeat the step1 if the passwords do not match*/
			LCD_displayString("Not Correct");
			_delay_ms(500);/*to be able to see the message*/
		}
	}
}
/*
 * Description : this function gets the status of the passwords from control ECU
 * matching or not
 */
uint8 HMI_receiveStatus(void){
	/*wait until the control ECU is ready to send the status*/
	while(UART_recieveByte()!= CONTROL_ECU_READY);
	UART_sendByte(HMI_ECU_READY);
	/*read the status*/
	return UART_recieveByte();
}
/*
 * Description : display the main menu which the user will choose from
 */
void HMI_displayMainOptions(void){
	LCD_clear();
	LCD_displayString("( + ) : Open The Door.");
	LCD_displayStringRowColumn(1,0,"( - ) : Change Password");
}
/*
 * Description : take the users option
 */
uint8 HMI_takeOption(void){
	return KEYPAD_getPressedKey();
}
void HMI_handleOptions(uint8* password_ptr){
	/*to hold the option that the user choose*/
	uint8 selected_option;
	while(1){
		HMI_displayMainOptions();
		selected_option = HMI_takeOption();
		if(selected_option == OPEN_DOOR_OPTION){
			LCD_clear();
			LCD_displayString("Please Enter password : ");
			HMI_takePassword(password_ptr);
			HMI_sendPasswordToControl(password_ptr);
			g_status = HMI_receiveStatus();
			if(g_status == CONTROL_PASSWORD_MATCH){
				LCD_clear();
				LCD_displayString("Opening The Door");
				_delay_ms(15000);
			}else if(g_status == CONTROL_PASSWORD_DISMATCH){

			}else if(g_status == ERROR_MESSAGE){
				LCD_clear();
				LCD_displayString("ERROR !");
			}
		}else if(selected_option == CHANGE_PASSWORD_OPTION){

		}
	}
}
