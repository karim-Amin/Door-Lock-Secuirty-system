 /******************************************************************************
 *
 * Module: CONTROL ECU main function
 *
 * File Name: control.c
 *
 * Description: to contain the app functions for the control ecu
 *
 * Author: Karim Mohamed Amin
 *
 *******************************************************************************/
#include "buzzer.h"
#include "eeprom.h"
#include "motor.h"
#include "uart.h"
#include "I2C.h"
#include "timer.h"
#include <avr/io.h> /* to enable the global interrupt*/
#include "util/delay.h"
/*******************************************************************************
 *                                global variables                                  *
 *******************************************************************************/
/*setup the TIMER1 configuration */
TimerconfigType s_timer1_config = {timer1_ID,compare_mode,prescaler_1024,0,15000};
/*to know how many times the user entered wrong password*/
uint8 wrong_pass_count = 0;
uint8 g_flag = 0;
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define CONTROL_ECU_READY 0x10
#define CONTROL_ECU_BAUD_RATE 9600
#define CONTROL_PASSWORD_MATCH 0x11
#define CONTROL_PASSWORD_DISMATCH 0x00
#define CONTROL_ADDRESS 0x44
#define HMI_ECU_READY 0x20
#define NUM_OF_PASSWORD_DIGIT 5
#define PASSWORD_ADDRESS_IN_EEPROM 0x000
#define ERROR_MESSAGE 0xFF
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void CONTROL_receivePasswordFromHMI(uint8* password_ptr);
void CONTROL_savePasswordInEEPROM(const uint8* password_ptr);
void CONTROL_sendStatus(uint8 state);
void CONTROL_getPasswordEEPROM(uint8* password_ptr);
void CONTROL_handelOpenDoor(uint8* password_ptr,uint8* existing_password);
void CONTROL_handelMotorTimeOut(void);
void CONTROL_stopMotor(void);
uint8 CONTROL_setupFirstPassword(uint8* a_first_password_ptr,uint8* a_second_password_ptr);
uint8 CONTROL_checkTwoPasswords(const uint8* a_first_password_ptr,const uint8* a_second_password_ptr);
/*******************************************************************************
 *                             The entry point (main method)                  *
 *******************************************************************************/
int main(void){
	/*to hold the first_password taken from the user*/
	uint8 first_password_buff[NUM_OF_PASSWORD_DIGIT];
	/*to hold the second_password taken from the user*/
	uint8 second_password_buff[NUM_OF_PASSWORD_DIGIT];
	/*setup the UART configuration*/
	config_struct s_uart_config = {no_parity,eigth_bits,one_stop_bit,Asynch,CONTROL_ECU_BAUD_RATE};
	/*setup the I2C configuration*/
	ConfigType s_twi_config = {Normal_mode,CONTROL_ADDRESS};

	/*setting the callback functions*/
	TIMER1_setCallBack(CONTROL_handelMotorTimeOut);
	/* 	calling the init functions for each driver */
	UART_init(&s_uart_config);
	TWI_init(&s_twi_config);
	DcMotor_init();
	BUZZER_init();
	/*set the I-bit to be able to use the timer driver*/
	SREG |= (1<<7);
	DDRB |= (1<<PB0);
	while(1){
		/*check if the passwords sent by HMI_ECU are identical and send to it the status*/
		if(CONTROL_setupFirstPassword(first_password_buff,second_password_buff) == SUCCESS){
			CONTROL_sendStatus(CONTROL_PASSWORD_MATCH);
		}else{
			CONTROL_sendStatus(CONTROL_PASSWORD_DISMATCH);
		}
		while(g_flag == 0){
		CONTROL_handelOpenDoor(first_password_buff,second_password_buff);
		}
	}
	return 0;
}
/*******************************************************************************
 *                              Functions Definitions                           *
 *******************************************************************************/
/*
 * Description : this function is synchronised with the UART in human machine interface and
 * it loops until it takes the entire password.
 */
void CONTROL_receivePasswordFromHMI(uint8* password_ptr){
	/*counter for the loop*/
	uint8 count;
	/*waiting until the HMI_ECU to be ready to send the password*/
	while(UART_recieveByte()!= HMI_ECU_READY);
	/*telling the HMI_ECU that control ECU is ready to receive the password */
	UART_sendByte(CONTROL_ECU_READY);
	/*looping until the length of the password*/
	for(count = 0;count<NUM_OF_PASSWORD_DIGIT;count++){
		password_ptr[count] = UART_recieveByte();
	}
}
/*
 * Description : to create new password you have to enter it twice this function will check
 * if this two password matching or not
 */
uint8 CONTROL_checkTwoPasswords(const uint8* a_first_password_ptr,const uint8* a_second_password_ptr){
	/*
	 * 1) i is the counter for the loop
	 * 2) flag is used to know if they matching or not
	 */
	uint8 i,flag = 0;
	/*compare each number in first password with each number in the other one*/
	for(i = 0;i < NUM_OF_PASSWORD_DIGIT;i++){
		if(a_first_password_ptr[i] != a_second_password_ptr[i]){
			/*flag raised means they not matching*/
			flag = 1;
		}
	}
	if(flag == 1){
		return FALSE;
	}else{
		return TRUE;
	}
}
/*
 * Description : set the entire password in EEPROM in specific locations ,and make constraints on
 *  this function do not let it change in the password
 */
void CONTROL_savePasswordInEEPROM(const uint8* password_ptr){
	uint8 counter;
	for(counter = 0;counter < NUM_OF_PASSWORD_DIGIT;counter++){
		/*
		 * 1) PASSWORD_ADDRESS_IN_EEPROM is the base address
		 * 2) counter will be used as an offset
		 */
		EEPROM_writeByte(PASSWORD_ADDRESS_IN_EEPROM + counter,password_ptr[counter]);
	}
}
/*
 * Description : this function receives two passwords for HMI_ECU and check them
 * if they match ,it will save this password in the EEPROM.
 */
uint8 CONTROL_setupFirstPassword(uint8* a_first_password_ptr,uint8* a_second_password_ptr){
	/*take the first password THROUGH the UART*/
	CONTROL_receivePasswordFromHMI(a_first_password_ptr);
	/*take the second password THROUGH the UART*/
	CONTROL_receivePasswordFromHMI(a_second_password_ptr);
	/*if the two passwords are identical ,then go save them in EEPROM*/
	if(CONTROL_checkTwoPasswords(a_first_password_ptr,a_second_password_ptr)){
		CONTROL_savePasswordInEEPROM(a_first_password_ptr);
		return SUCCESS;
	}else{
		return ERROR;
	}
}
/*
 * Description : it will send the status (matching or not matching).
 */
void CONTROL_sendStatus(uint8 state){
	UART_sendByte(CONTROL_ECU_READY);
	while(UART_recieveByte()!= HMI_ECU_READY);
	UART_sendByte(state);
}

void CONTROL_getPasswordEEPROM(uint8* password_ptr){
	uint8 counter;
	uint8 byte;
	for(counter = 0;counter < NUM_OF_PASSWORD_DIGIT;counter++){
		/*
		 * 1) PASSWORD_ADDRESS_IN_EEPROM is the base address
		 * 2) counter will be used as an offset
		 */
		EEPROM_readByte(PASSWORD_ADDRESS_IN_EEPROM + counter,&byte);
		password_ptr[counter] = byte;
	}
}

void CONTROL_handelOpenDoor(uint8* password_ptr,uint8* existing_password){

	CONTROL_receivePasswordFromHMI(password_ptr);
	if(CONTROL_checkTwoPasswords(password_ptr,existing_password) == TRUE){
		CONTROL_sendStatus(CONTROL_PASSWORD_MATCH);
		DcMotor_Rotate(clock_wise,100);
		/*start counting 15 seconds*/
		TIMER_init(&s_timer1_config);
	}else{
		wrong_pass_count++;
		if(wrong_pass_count < 3){
			CONTROL_sendStatus(CONTROL_PASSWORD_DISMATCH);
		}else{
			CONTROL_sendStatus(ERROR_MESSAGE);
			BUZZER_ON();
		}
	}
}
void CONTROL_stopMotor(void){
	DcMotor_Rotate(stop_motor,0);
	g_flag = 1;
}
void CONTROL_handelMotorTimeOut(void){
	/*to stop the DC motor after 15 seconds*/
	DcMotor_Rotate(stop_motor,0);
	/*TO disable timer 1*/
	TIMER_deinit(timer1_ID);
	/*wait three seconds and close the door*/
	_delay_ms(3000);
	DcMotor_Rotate(anti_clock_wise,100);
	TIMER1_setCallBack(CONTROL_stopMotor);
	/*start counting 15 seconds*/
	TIMER_init(&s_timer1_config);
}
