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
#include <avr/io.h> /* to enable the global interrupt*/
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
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void CONTROL_receivePasswordFromHMI(uint8* password_ptr);
void CONTROL_savePasswordInEEPROM(const uint8* password_ptr);
void CONTROL_sendStatus(uint8 state);
uint8 CONTROL_setupFirstPassword(uint8* a_first_password_ptr,uint8* a_second_password_ptr);
uint8 CONTROL_checkTwoPasswords(const uint8* a_first_password_ptr,const uint8* a_second_password_ptr);
/*******************************************************************************
 *                             The entry point (main method)                  *
 *******************************************************************************/
int main(void){
	/*to hold the password taken from the user*/
	uint8 first_password_buff[NUM_OF_PASSWORD_DIGIT];
	uint8 second_password_buff[NUM_OF_PASSWORD_DIGIT];
	config_struct s_uart_config = {no_parity,eigth_bits,one_stop_bit,Asynch,CONTROL_ECU_BAUD_RATE};
	ConfigType s_twi_config = {Normal_mode,CONTROL_ADDRESS};
	UART_init(&s_uart_config);
	TWI_init(&s_twi_config);
	/*set the I-bit to be able to use the timer driver*/
	SREG |= (1<<7);
	while(1){
		if(CONTROL_setupFirstPassword(first_password_buff,second_password_buff) == SUCCESS){
			CONTROL_sendStatus(CONTROL_PASSWORD_MATCH);
		}else{
			CONTROL_sendStatus(CONTROL_PASSWORD_DISMATCH);
		}

	}
	return 0;
}
/*******************************************************************************
 *                              Functions Definitions                           *
 *******************************************************************************/
void CONTROL_receivePasswordFromHMI(uint8* password_ptr){
	uint8 count;
	while(UART_recieveByte()!= HMI_ECU_READY);
	UART_sendByte(CONTROL_ECU_READY);
	for(count = 0;count<NUM_OF_PASSWORD_DIGIT;count++){
		password_ptr[count] = UART_recieveByte();
	}
}
uint8 CONTROL_checkTwoPasswords(const uint8* a_first_password_ptr,const uint8* a_second_password_ptr){
	uint8 i,flag = 0;
	for(i = 0;i < NUM_OF_PASSWORD_DIGIT;i++){
		if(a_first_password_ptr[i] != a_second_password_ptr[i]){
			flag = 1;
		}
	}
	if(flag == 1){
		return FALSE;
	}else{
		return TRUE;
	}
}
void CONTROL_savePasswordInEEPROM(const uint8* password_ptr){
	uint8 counter;
	for(counter = 0;counter < NUM_OF_PASSWORD_DIGIT;counter++){
		EEPROM_writeByte(PASSWORD_ADDRESS_IN_EEPROM + counter,password_ptr[counter]);
	}
}
uint8 CONTROL_setupFirstPassword(uint8* a_first_password_ptr,uint8* a_second_password_ptr){
	CONTROL_receivePasswordFromHMI(a_first_password_ptr);
	CONTROL_receivePasswordFromHMI(a_second_password_ptr);
	if(CONTROL_checkTwoPasswords(a_first_password_ptr,a_second_password_ptr)){
		CONTROL_savePasswordInEEPROM(a_first_password_ptr);
		return SUCCESS;
	}else{
		return ERROR;
	}
}
void CONTROL_sendStatus(uint8 state){
	UART_sendByte(CONTROL_ECU_READY);
	while(UART_recieveByte()!= HMI_ECU_READY);
	UART_sendByte(state);
}
