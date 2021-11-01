/*****************************************************************************************
 *  module : External Buzzer
 *  file name : buzzer.h
 *  description : header file for external buzzer module
 *  Author :  Karim Mohamed Amin
 *****************************************************************************************/
#ifndef BUZZER_H_
#define BUZZER_H_
#include "std_types.h"
/****************************************************************************************
 * 										Definitions
 ***************************************************************************************/
#define BUZZER_PORT_ID PORTB_ID
#define BUZZER_PIN_ID PIN0_ID
/****************************************************************************************
 * 										Function ProtoTypes
 ***************************************************************************************/
void BUZZER_init(void);
void BUZZER_ON(void);
void BUZZER_OFF(void);
#endif /* BUZZER_H_ */
