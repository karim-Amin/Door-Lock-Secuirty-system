
/*************************************************************************************************************
 *  module : I2C
 *  file name : I2C.c
 *  description : source file for I2C module in AVR microcontroller
 *  Author :  Karim Mohamed Amin
 *************************************************************************************************************/
#include"I2C.h"
#include"common_macros.h"
#include"avr/io.h"
/************************************************************************************************************
 * 									functions Definitions
 ************************************************************************************************************/
void TWI_init(const ConfigType* config_struct_ptr){

	/* Fscl = Fcpu/(16+2(TWBR)*4^TWPS)
	 * i will always choose the pre-scaler to be one TWSR = 0
	 */
	TWSR = 0x00;
	TWBR = ((uint8)(F_CPU/config_struct_ptr->bit_rate_type)-16)/2;
	/* the seven MSB are the address of the device so we have to shift
	 * the address received by one bit to the left :) */
	TWAR = (config_struct_ptr->bit_rate_type)<<1;
	/*
	 *1) disable the acknowledge bit => TWEA = 0
	 *2) start condition bit = 0
	 *3) stop condition bit = 0
	 *4) enable I2C module => TWEN = 1
	 */
	TWCR = (1<<TWEN);
}
void TWI_start(void){
	/*
	* Clear the TWINT flag before sending the start bit TWINT=1
	* send the start bit by TWSTA=1
	* Enable TWI Module TWEN=1
	 */
	TWCR = (1<<TWSTA)|(1<<TWINT)|(1<<TWEN);
    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
}
void TWI_stop(void){
	/*
	* Clear the TWINT flag before sending the start bit TWINT=1
	* send the stop bit by TWSTO=1
	* Enable TWI Module TWEN=1
	 */
	TWCR = (1<<TWSTO)|(1<<TWINT)|(1<<TWEN);
}
void TWI_write(uint8 data){
	/*write the data in TWDR*/
	TWDR = data;
	/*clear the flag*/
	TWCR = (1<<TWINT)|(1<<TWEN);
	/*wait until the byte is sent successfully*/
	while(BIT_IS_CLEAR(TWCR,TWINT));
}
uint8 TWI_readWithACK(void){
	/*
	 * clear the flag before i read the data
	 * enable the ack in TWCR
	 * enable I2c module TWEN
	 */
	TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWINT);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	  while(BIT_IS_CLEAR(TWCR,TWINT));
	  return TWDR;
}
uint8 TWI_readWithNACK(void){
	/*
	 * clear the flag before i read the data
	 * enable the ack in TWCR
	 * enable I2c module TWEN
	 */
	TWCR = (1<<TWEN)|(1<<TWINT);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
	return TWDR;
}
uint8 TWI_getStatus(void){
	uint8 status;
	status = TWSR & TWI_STATUS_MASK;
	return status;
}
