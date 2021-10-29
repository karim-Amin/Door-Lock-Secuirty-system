 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: source file for timer driver in AVR this will support all the timers
 * with (compare mode OR over flow mode (normal mode) )
 *
 * Author: Karim Mohamed Amin
 *
 *******************************************************************************/
#include "timer.h"
#include "avr/io.h"
#include "avr/interrupt.h"
/***********************************************************************************************************
 *												private global variables
 ************************************************************************************************************/
static volatile void (*g_ptr2fun0)(void) = NULL_PTR;
static volatile void (*g_ptr2fun1)(void) = NULL_PTR;
static volatile void (*g_ptr2fun2)(void) = NULL_PTR;
/***********************************************************************************************************
 *												Interrupt service routines
 ************************************************************************************************************/
ISR(TIMER0_OVF_vect){
	/*THIS condition avoid the program crashing*/
	if(g_ptr2fun0 != NULL_PTR){
		(*g_ptr2fun0)();
	}
}
ISR(TIMER0_COMP_vect){
	/*THIS condition avoid the program crashing*/
	if(g_ptr2fun0 != NULL_PTR){
		(*g_ptr2fun0)();
	}
}
/***********************************************************************************************************
 *												Function Definitions
 ************************************************************************************************************/
/*
 * Function Description :
 */
void TIMER_init(const configType* config_struct_ptr){
	/*first check the timer id*/
	if(config_struct_ptr->timer_id == timer0_ID){
		/*check the timer mode you want to work with*/
		if(config_struct_ptr->mode == normal_mode){
			/*
			 * 1) FOC0 = 1 -> force output compare pin must equal to 1 when non PWM
			 * 2) WGM00 = 0 & WGM01 =0 -> TO work with normal mode
			 * 3) COM01 =0 & COM00 = 0 -> TO disconnect compare match output mode
			 */
			TCCR0 = (1<<FOC0);
			/*enable the timer over flow interrupt*/
			TIMSK = (1<<TOIE0);
		}else if(config_struct_ptr->mode == compare_mode){
			/*
			 * 1) FOC0 = 1 -> force output compare pin must equal to 1 when non PWM
			 * 2) WGM00 = 0 & WGM01 =1 -> TO work with compare  mode
			 * 3) COM01 =0 & COM00 = 0 -> TO disconnect compare match output mode
			 */
			TCCR0 = (1<<FOC0)|(1<<WGM01);
			/*set the output compare value*/
			OCR0 = (uint8)config_struct_ptr->timer_compare_value;
			/*enable the output compare match interrupt*/
			TIMSK = (1<<OCIE0);
		}
		/*
		 * insert the prescaler value in the first three bits in the control register:)
		 */
		TCCR0 = (TCCR0 & 0xF8)|(config_struct_ptr->prescaler_value);
		/*
		 * loading the timer init value in timer counter register
		 */
		TCNT0 = (uint8)config_struct_ptr->timer_init_value;

	}else if(config_struct_ptr->timer_id == timer1_ID){

	}else if(config_struct_ptr->timer_id == timer2_ID){

	}
}
/*
 * Function Description : set the function which will be called after the timer finishes counting
 */
void TIMER0_setCallBack(const void(*a_ptr2fun0)(void)){
	g_ptr2fun0 = a_ptr2fun0;
}
