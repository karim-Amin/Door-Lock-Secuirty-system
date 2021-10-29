
/*****************************************************************************************
 *  module : External Buzzer
 *  file name : Buzzer.c
 *  description : source file for external buzzer module
 *  Author :  Karim Mohamed Amin
 *****************************************************************************************/
#include "gpio.h"
#include "buzzer.h"
/****************************************************************************************
 * 										Function Definitions
 ***************************************************************************************/
/*
 * Description : make the pin connected to the buzzer output to be able to drive
 * buzzer on or off
 */
void BUZZER_init(void){
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
}
/*
 * Description : Turn on the buzzer by output high on its pin
 */
void BUZZER_ON(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}
/*
 * Description : Turn off the buzzer by output low on its pin
 */
void BUZZER_OFF(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}

