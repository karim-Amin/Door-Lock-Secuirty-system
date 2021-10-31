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
#include <avr/io.h> /* to enable the global interrupt*/
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define CONTROL_ECU_READY 0x10
#define HMI_ECU_READY 0x20
#define NUM_OF_PASSWORD_DIGIT 5
#define CONTROL_ECU_BAUD_RATE 9600
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void CONTROL_receivePasswordFromHMI(uint8* password_ptr);
/*******************************************************************************
 *                             The entry point (main method)                  *
 *******************************************************************************/
int main(void){
	/*to hold the password taken from the user*/
	uint8 password_buff[NUM_OF_PASSWORD_DIGIT];
	config_struct s_uart_config = {no_parity,eigth_bits,one_stop_bit,Asynch,CONTROL_ECU_BAUD_RATE};
	UART_init(&s_uart_config);
	/*set the I-bit to be able to use the timer driver*/
	SREG |= (1<<7);
	while(1){
		CONTROL_receivePasswordFromHMI(password_buff);
		CONTROL_receivePasswordFromHMI(password_buff);
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

