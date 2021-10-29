<<<<<<< HEAD
/**********************************************************************************************************
 * Module : USART
 * File name : uart.h
 * Description : header file for UART driver in AVR micro.
 * AUTH : Karim Mohamed Amin
 ***********************************************************************************************************/

#ifndef UART_H_
#define UART_H_
#include"std_types.h"

/************************************************************************************************************
 * 											Definitions
 ************************************************************************************************************/
#define UART_MODE_SELECT_MASK 0xBF
#define UART_PARTIY_MODE_MASK 0xCF
#define UART_STOP_BIT_MASK 0xF7
#define UART_CHARACTER_SIZE_FIRST_BITS_MASK 0XF9
#define UART_CHARACTER_SIZE_LAST_BIT_MASK 0XFB
/************************************************************************************************************
 * 											Types definitions
 ************************************************************************************************************/
typedef enum{
	no_parity,even_parity = 2, odd_parity
}parity_type;
typedef enum{
	five_bits,six_bits,seven_bits,eigth_bits,nine_bits = 7
}message_size;
typedef enum{
	one_stop_bit,two_stop_bits
}stop_bit;
typedef enum{
	Asynch,Synch
}operation_type;
typedef struct{
	parity_type parity;
	message_size character_size;
	stop_bit num_stop_bit;
	operation_type comm_type;
	uint32 baud_rate;
}config_struct;
/************************************************************************************************************
 * 											Functions Declaration
 ************************************************************************************************************/
void UART_init(const config_struct* configStruct);
void UART_sendByte(const uint8 data);
uint8 UART_recieveByte(void);
void UART_sendString(const uint8* str);
void UART_recieveString(uint8* recievedData);

#endif /* UART_H_ */
=======
/**********************************************************************************************************
 * Module : USART
 * File name : uart.h
 * Description : header file for UART driver in AVR micro.
 * AUTH : Karim Mohamed Amin
 ***********************************************************************************************************/

#ifndef UART_H_
#define UART_H_
#include"std_types.h"

/************************************************************************************************************
 * 											Definitions
 ************************************************************************************************************/
#define UART_MODE_SELECT_MASK 0xBF
#define UART_PARTIY_MODE_MASK 0xCF
#define UART_STOP_BIT_MASK 0xF7
#define UART_CHARACTER_SIZE_FIRST_BITS_MASK 0XF9
#define UART_CHARACTER_SIZE_LAST_BIT_MASK 0XFB
/************************************************************************************************************
 * 											Types definitions
 ************************************************************************************************************/
typedef enum{
	no_parity=0,even_parity = 2, odd_parity
}parity_type;
typedef enum{
	five_bits,six_bits,seven_bits,eigth_bits
}message_size;
typedef enum{
	one_stop_bit,two_stop_bits
}stop_bit;
typedef enum{
	Asynch,Synch
}operation_type;
typedef struct{
	parity_type parity;
	message_size character_size;
	stop_bit num_stop_bit;
	operation_type comm_type;
	uint32 baud_rate;
}config_struct;
/************************************************************************************************************
 * 											Functions Declaration
 ************************************************************************************************************/
void UART_init(const config_struct* configStruct);
void UART_sendByte(const uint8 data);
uint8 UART_recieveByte(void);
void UART_sendString(const uint8* str);
void UART_recieveString(uint8* recievedData);

#endif /* UART_H_ */
>>>>>>> main program structure
