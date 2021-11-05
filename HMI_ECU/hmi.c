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
#include "timer.h"
#include <avr/io.h> /* to enable the global interrupt*/
#include <util/delay.h>
/*******************************************************************************
 *                                global variables                            *
 *******************************************************************************/
/*to know when the timer finishes counting*/
uint8 g_timer_flag=0;
/*to hold the status of the passwords sent by control ECU*/
uint8 g_status;
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define NUM_OF_PASSWORD_DIGIT 5
#define HMI_BAUD_RATE 9600
#define OPEN_DOOR_OPTION '+'
#define CHANGE_PASSWORD_OPTION '-'
/*							ECU communication                                */
#define CONTROL_ECU_READY 0x10
#define CONTROL_PASSWORD_MATCH 0x11
#define CONTROL_PASSWORD_DISMATCH 0x00
#define HMI_ECU_READY 0x20
#define ERROR_MESSAGE 0xFF
#define OPENING_DOOR 0X22
#define CLOSING_DOOR 0X33
#define DOOR_CLOSED 0X44
#define CONTINUE_PROGRAM 0X55
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void HMI_takePassword(uint8* password_ptr);
void HMI_sendPasswordToControl(const uint8* password_ptr);
void HMI_setPasswordFirstTime(uint8* a_first_password_ptr,uint8* a_second_password_ptr);
void HMI_displayMainOptions(void);
void HMI_setAndCheckStatus(uint8* a_first_password_ptr,uint8* a_second_password_ptr);
void HMI_handleOptions(uint8* a_first_password_ptr,uint8* a_second_password_ptr);
void HMI_handleTimer(void);
void HMI_sendOption(uint8 option);
uint8 HMI_takeOption(void);
uint8 HMI_receiveStatus(void);
/*******************************************************************************
 *                             The entry point (main method)                  *
 *******************************************************************************/
int main(void){
	/*to hold the first_password taken from the user*/
	uint8 first_password_buff[NUM_OF_PASSWORD_DIGIT];
	/*to hold the second_password taken from the user*/
	uint8 second_password_buff[NUM_OF_PASSWORD_DIGIT];
	/*setup the UART configuration*/
	config_struct s_uart_config = {no_parity,eigth_bits,one_stop_bit,Asynch,HMI_BAUD_RATE};
	/*setting the callback functions*/
	TIMER1_setCallBack(HMI_handleTimer);
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
		HMI_handleOptions(first_password_buff,second_password_buff);
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
	for(count = 0; count<NUM_OF_PASSWORD_DIGIT ;count++){
		password_ptr[count] = KEYPAD_getPressedKey();
		/*display "*" sign to hide the passwword*/
		LCD_displayStringRowColumn(1,count,(uint8*)"*");
		/*to avoid taking the pressed number more than one time*/
		_delay_ms(500);
	}
}
/*
 * Description : this function send the password to control ECU if it is ready to receive
 */
void HMI_sendPasswordToControl(const uint8* password_ptr){
	/*this is counter for the loop*/
	uint8 count;
	/*send ready to synchronise between two ECUS*/
	UART_sendByte(HMI_ECU_READY);
	/*wait until the control ECU is ready to read the password*/
	while(UART_recieveByte()!= CONTROL_ECU_READY);
	/*send the password to control ECU by UART driver byte by byte*/
	for(count = 0;count<NUM_OF_PASSWORD_DIGIT;count++){
			UART_sendByte(password_ptr[count]);
		}
}
/*
 * Description : used to take the passwords twice and send them to control ECU to check
 * if they matching or not
 */
void HMI_setPasswordFirstTime(uint8* a_first_password_ptr,uint8* a_second_password_ptr){
	LCD_clear();
	/* take the first one*/
	LCD_displayStringRowColumn(0,0,(uint8*)"Please Enter password : ");
	HMI_takePassword(a_first_password_ptr);
	LCD_clear();
	/*take the second one*/
	LCD_displayStringRowColumn(0,0,(uint8*)"Please re-enter password: ");
	HMI_takePassword(a_second_password_ptr);
	LCD_clear();
	/*send them all*/
	HMI_sendPasswordToControl(a_first_password_ptr);
	HMI_sendPasswordToControl(a_second_password_ptr);
}
/*
 * Description : this function takes two passwords , check them and display the result on LCD */
void HMI_setAndCheckStatus(uint8* a_first_password_ptr,uint8* a_second_password_ptr){
	/*this loop to keep asking to enter two passwords if they not matching*/
	while(1)
	{
		/*take two passwords and send them to control ECU*/
		HMI_setPasswordFirstTime(a_first_password_ptr,a_second_password_ptr);
		/*keep waiting until HMI ECU receives the status of the two passwords*/
		g_status = HMI_receiveStatus();
		/*if they matching*/
		if(g_status == CONTROL_PASSWORD_MATCH){
			/*display the result on LCD */
			LCD_displayString((uint8*)"Correct Password saved...");
			_delay_ms(500);/*to be able to see the message*/
			break;/*go to the next step*/
		}else{
			/*i will repeat the step1 if the passwords do not match*/
			LCD_displayString((uint8*)"Not Correct Password not saved...");
			/*to be able to see the message*/
			_delay_ms(500);
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
	LCD_displayString((uint8*)"( + ) : Open The Door.");
	LCD_displayStringRowColumn(1,0,(uint8*)"( - ) : Change Password");
}
/*
 * Description : take the users option
 */
uint8 HMI_takeOption(void){
	return KEYPAD_getPressedKey();
}
/*
 * Description : increment the global counter for the timer
 */
void HMI_handleTimer(void){
	g_timer_flag ++;
}
/*
 * Description : this synchronized function sends the option from the main menu
 * to be handled in control ECU side
 */
void HMI_sendOption(uint8 option){
	UART_sendByte(HMI_ECU_READY);
	while(UART_recieveByte()!= CONTROL_ECU_READY);
	 UART_sendByte(option);
}
/*
 * Description : this is responsible for displaying the main option ,then send the option to
 * the control ECU ,and communicate with control ECU and handle two option
 */
void HMI_handleOptions(uint8* a_first_password_ptr,uint8* a_second_password_ptr){
	/*to hold the option that the user choose*/
	uint8 selected_option;
	/*this while loop used to keep asking the user to choose from the main menu*/
	while(1){
		/*display option*/
		HMI_displayMainOptions();
		/*take the input*/
		selected_option = HMI_takeOption();
		/*send the order to CONTROL ECU to handle it*/
		HMI_sendOption(selected_option);
		/*check the selected option first*/
		if(selected_option == OPEN_DOOR_OPTION){
			/*this loop used to stuck asking about the password if the wrong one entered*/
			while(1){
				LCD_clear();
			/*tell the user to enter the password*/
			LCD_displayString((uint8*)"Please Enter password : ");
			_delay_ms(200);
			/*take the password from the user*/
			HMI_takePassword(a_first_password_ptr);
			/*send the password to control 	ECU to check it*/
			HMI_sendPasswordToControl(a_first_password_ptr);
			/*receive the status of the password sent by the control ECU*/
			g_status = HMI_receiveStatus();
			/*check on the status to display suitable message*/
			if(g_status == OPENING_DOOR){
				/*opening the door status means that correct password */
				LCD_clear();
				/*display this message synchronised with the action */
				LCD_displayString((uint8*)"Opening The Door");
				/*keep displaying this until receiving closing the door status*/
				g_status = HMI_receiveStatus();
				/*if the status sent by control ecu is closing the door*/
				if(g_status == CLOSING_DOOR){
					LCD_clear();
					/*display message closing on the screen*/
					LCD_displayString((uint8*)"closing The Door");
					/*i will wait and receive door closed DOOR_CLOSED macro*/
					g_status = HMI_receiveStatus();
					/*to display the main menu again*/
					break;
				}
			}
			/**/
			else if(g_status == CONTROL_PASSWORD_DISMATCH){
				LCD_clear();
				LCD_displayString((uint8*)"Wrong Password !");
				_delay_ms(500);
				/*No break statement to keep asking about the password and do not let it go to the main menu*/
			}

			else if(g_status == ERROR_MESSAGE){
				LCD_clear();
				LCD_displayString((uint8*)"ERROR !");
				HMI_receiveStatus();/*to wait until receive continue program status*/
				/*to display the main menu again*/
				break;
			}
		}
		}else if(selected_option == CHANGE_PASSWORD_OPTION){
			/*this loop used to stuck asking about the password if the wrong one entered*/
			while(1){
				LCD_clear();
				/*tell the user to enter the password*/
				LCD_displayString((uint8*)"Please Enter password : ");
				_delay_ms(200);
				/*take the password from the user*/
				HMI_takePassword(a_first_password_ptr);
				/*send the password to control 	ECU to check it*/
				HMI_sendPasswordToControl(a_first_password_ptr);
				/*receive the status of the password sent by the control ECU*/
				g_status = HMI_receiveStatus();
				if(g_status == CONTROL_PASSWORD_MATCH){
					LCD_displayString("changing the password....");
					_delay_ms(1000);
					HMI_setAndCheckStatus(a_first_password_ptr,a_second_password_ptr);
					break;
				}else if(g_status == CONTROL_PASSWORD_DISMATCH){
					LCD_clear();
					LCD_displayString((uint8*)"Wrong Password !");
					_delay_ms(500);
					/*No break statement to keep asking about the password and do not let it go to the main menu*/
				}
				else if(g_status == ERROR_MESSAGE){
					LCD_clear();
					LCD_displayString((uint8*)"ERROR !");
					HMI_receiveStatus();/*to wait until receive continue program status*/
					/*to display the main menu again*/
					break;
				}

			}
		}
	}
}
