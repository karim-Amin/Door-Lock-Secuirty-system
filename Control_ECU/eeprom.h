
/*************************************************************************************************************
 *  module : EEPROM
 *  file name : eeprom.h
 *  description : header file for external EEPROM
 *  Author :  Karim Mohamed Amin
 *************************************************************************************************************/

#ifndef EEPROM_H_
#define EEPROM_H_
#include"std_types.h"
/************************************************************************************************************
 * 												Definitions
 ************************************************************************************************************/
#define ERROR 0
#define SUCCESS 1
/************************************************************************************************************
 * 												functions prototypes
 ************************************************************************************************************/

uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);
#endif /* EEPROM_H_ */