<<<<<<< HEAD
/**********************************************************************************************************
 * Module : USART
 * File name : uart.c
 * Description : source file for UART driver in AVR micro.
 * AUTH : Karim Mohamed Amin
 ***********************************************************************************************************/

#include"uart.h"
#include"common_macros.h"
#include"avr/io.h"

/************************************************************************************************************
 * 											Functions Definitions
 ************************************************************************************************************/
void UART_init(const config_struct* configStruct){
	uint16 ubrr_value = 0;
	/* U2X = 1*/
	UCSRA |= (1<<U2X);
	/*
	 * 1) enable transmitter
	 * 2) enable receiver
	 */
	UCSRB = (1<<TXEN)|(1<<RXEN);
	/*
	 * 1)select to write in UCSRC URSEL = 1
	 */
	UCSRC = (1<<URSEL);
	/*insert mode selected in configuration structure*/
	UCSRC = (UCSRC & UART_MODE_SELECT_MASK)|((configStruct->comm_type)<<UMSEL);
	/*insert parity type in configuration structure*/
	UCSRC = (UCSRC & UART_PARTIY_MODE_MASK)|((configStruct->parity)<<UPM0);
	/*write the no.of stop bits in the control register*/
	UCSRC = (UCSRC & UART_STOP_BIT_MASK)|((configStruct->num_stop_bit)<<USBS);
	/*select the message size will be in two steps as there are two bits in register and there is one bit
	 * in other register
	 */
	/*insert the first two bits*/
	UCSRC = (UCSRC & UART_CHARACTER_SIZE_FIRST_BITS_MASK)|((configStruct->character_size & 0x3)<<UCSZ0);
	/*
	 * select the baud rate to be 9600 bit/sec
	 * UBRR =( Fosc / (8*BAUD) ) - 1
	 */
	ubrr_value = (uint16)(((F_CPU/(8UL * configStruct->baud_rate))) - 1);
	UBRRH = ubrr_value>>8;
	UBRRL=ubrr_value;
}
void UART_sendByte(const uint8 data){
	UDR = data;
	/*wait until the transmission complete*/
	while(BIT_IS_CLEAR(UCSRA,TXC));
	SET_BIT(UCSRA,TXC);/*clear the flag*/
}
uint8 UART_recieveByte(void){
	while(BIT_IS_CLEAR(UCSRA,RXC));
	return UDR;
}
void UART_sendString(const uint8* str){
	uint8 i=0;
	while(str[i] != '\0'){
		UART_sendByte(str[i]);
		i++;
	}
}
void UART_recieveString(uint8* recievedData){
	uint8 i=0;

	recievedData[i] = UART_recieveByte();
	while(recievedData[i] != '#'){
		i++;
		recievedData[i] = UART_recieveByte();
	}
	recievedData[i] = '\0';
}
=======
/**********************************************************************************************************
 * Module : USART
 * File name : uart.c
 * Description : source file for UART driver in AVR micro.
 * AUTH : Karim Mohamed Amin
 ***********************************************************************************************************/

#include"uart.h"
#include"common_macros.h"
#include"avr/io.h"

/************************************************************************************************************
 * 											Functions Definitions
 ************************************************************************************************************/
void UART_init(const config_struct* configStruct){
	uint16 ubrr_value = 0;
	/* U2X = 1*/
	UCSRA |= (1<<U2X);
	/*
	 * 1) enable transmitter
	 * 2) enable receiver
	 */
	UCSRB = (1<<TXEN)|(1<<RXEN);
	/*
	 * - select to write in UCSRC URSEL = 1
	 */
	UCSRC = (1<<URSEL);
	/*insert mode selected in configuration structure*/
	UCSRC = (UCSRC & UART_MODE_SELECT_MASK)|((configStruct->comm_type)<<UMSEL);
	/*insert parity type in configuration structure*/
	UCSRC = (UCSRC & UART_PARTIY_MODE_MASK)|((configStruct->parity)<<UPM0);
	/*write the no.of stop bits in the control register*/
	UCSRC = (UCSRC & UART_STOP_BIT_MASK)|((configStruct->num_stop_bit)<<USBS);
	/*select the message size will be in two steps as there are two bits in register and there is one bit
	 * in other register
	 */
	/*insert the first two bits*/
	UCSRC = (UCSRC & UART_CHARACTER_SIZE_FIRST_BITS_MASK)|((configStruct->character_size & 0x3)<<UCSZ0);

	/*
	 * select the baud rate to be 9600 bit/sec
	 * UBRR =( Fosc / (8*BAUD) ) - 1
	 */
	ubrr_value = (uint16)(((F_CPU/(8UL * configStruct->baud_rate))) - 1);
	UBRRH = ubrr_value>>8;
	UBRRL=ubrr_value;
}
void UART_sendByte(const uint8 data){
	UDR = data;
	/*wait until the transmission complete*/
	while(BIT_IS_CLEAR(UCSRA,TXC));
	SET_BIT(UCSRA,TXC);/*clear the flag*/
}
uint8 UART_recieveByte(void){
	while(BIT_IS_CLEAR(UCSRA,RXC));
	return UDR;
}
void UART_sendString(const uint8* str){
	uint8 i=0;
	while(str[i] != '\0'){
		UART_sendByte(str[i]);
		i++;
	}
}
void UART_recieveString(uint8* recievedData){
	uint8 i=0;

	recievedData[i] = UART_recieveByte();
	while(recievedData[i] != '#'){
		i++;
		recievedData[i] = UART_recieveByte();
	}
	recievedData[i] = '\0';
}
>>>>>>> main program structure
