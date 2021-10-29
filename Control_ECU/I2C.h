
/*************************************************************************************************************
 *  module : I2C
 *  file name : I2C.h
 *  description : header file for I2C module in AVR microcontroller
 *  Author :  Karim Mohamed Amin
 *************************************************************************************************************/

#ifndef I2C_H_
#define I2C_H_
#include"std_types.h"
/************************************************************************************************************
 * 										Definitions
 ************************************************************************************************************/
/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */
#define TWI_STATUS_MASK   0XF8 /*use this mask to get the status of TWI bus from the register TWSR*/
/************************************************************************************************************
 * 									Types Declaration
 ************************************************************************************************************/
/*
 * Normal Mode 100 Kb/s
 * Fast Mode 400 Kb/s
 */
typedef enum{
	Normal_mode =100000UL,Fast_mode =400000UL
}BitRateType;
typedef struct{
	BitRateType bit_rate_type;
	uint8 adress;
}ConfigType;
/************************************************************************************************************
 * 									functions prototypes
 ************************************************************************************************************/
void TWI_init(const ConfigType* config_struct_ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8 data);
uint8 TWI_readWithACK(void);
uint8 TWI_readWithNACK(void);
uint8 TWI_getStatus(void);

#endif /* I2C_H_ */
