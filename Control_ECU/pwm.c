 /******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.c
 *
 * Description: source file for AVR PWM driver
 *
 * Author: Karim Mohamed Amin
 *
 *******************************************************************************/
#include"pwm.h"
#include"common_macros.h"
#include<avr/io.h>
/*
 * Description:
 * 	1) initialise the Timer0 registers to generate PWM signal
 * 	2) choose the type of PWM
 * 	3) set up the direction of the pin (PB3) will drive PWM
 */
void PWM_Timer0_Start(uint8 duty_cycle){
	TCCR0 = (1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS01);
	TCNT0 = 0;
	OCR0 = (uint8)(255 * ((float)duty_cycle/100));
	CLEAR_BIT(TIMSK,TOIE0);/*disable the interrupt*/
	SET_BIT(DDRB,3);/*PB3 is output pin*/
}
