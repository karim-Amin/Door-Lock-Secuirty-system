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
#include <avr/io.h>
#include <avr/interrupt.h>
/***********************************************************************************************************
 *												private global variables
 ************************************************************************************************************/
static volatile void (*g_ptr2fun0)(void)= NULL_PTR;
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
ISR(TIMER1_OVF_vect){
	/*THIS condition avoid the program crashing*/
	if(g_ptr2fun1 != NULL_PTR){
		(*g_ptr2fun1)();
	}
}
ISR(TIMER1_COMPA_vect){
	/*THIS condition avoid the program crashing*/
	if(g_ptr2fun1 != NULL_PTR){
		(*g_ptr2fun1)();
	}
}
ISR(TIMER2_OVF_vect){
	/*THIS condition avoid the program crashing*/
	if(g_ptr2fun2 != NULL_PTR){
		(*g_ptr2fun2)();
	}
}
ISR(TIMER2_COMP_vect){
	/*THIS condition avoid the program crashing*/
	if(g_ptr2fun2 != NULL_PTR){
		(*g_ptr2fun2)();
	}
}
/***********************************************************************************************************
 *												Function Definitions
 ************************************************************************************************************/
/*
 * Function Description :this function configure any timer in AVR micro with any mode we want
 * normal or compare mode
 */
void TIMER_init(const configType* config_struct_ptr){
	/*first check the timer id*/
	if(config_struct_ptr->timer_id == timer0_ID){
		/*check the timer mode you want to work with*/
		if(config_struct_ptr->mode == normal_mode){
			/*enable the timer over flow interrupt*/
			TIMSK |= (1<<TOIE0);
			/*
			 * 1) FOC0 = 1 -> force output compare pin must equal to 1 when non PWM
			 * 2) WGM00 = 0 & WGM01 =0 -> TO work with normal mode
			 * 3) COM01 =0 & COM00 = 0 -> TO disconnect compare match output mode
			 */
			TCCR0 = (1<<FOC0)|(TIMER_PRESCALER_MASK&config_struct_ptr->prescaler_value);

		}else if(config_struct_ptr->mode == compare_mode){
			/*set the output compare value*/
			OCR0 = (uint8)config_struct_ptr->timer_compare_value;
			/*enable the output compare match interrupt*/
			TIMSK |= (1<<OCIE0);
			/*
			 * 1) FOC0 = 1 -> force output compare pin must equal to 1 when non PWM
			 * 2) WGM00 = 0 & WGM01 =1 -> TO work with compare  mode
			 * 3) COM01 =0 & COM00 = 0 -> TO disconnect compare match output mode
			 */
			/*
			 * insert the prescaler value in the first three bits in the control register:)
			 */
			TCCR0 = (1<<FOC0)|(1<<WGM01)|(TIMER_PRESCALER_MASK&config_struct_ptr->prescaler_value);

		}
		/*
		 * loading the timer init value in timer counter register
		 */
		TCNT0 = (uint8)config_struct_ptr->timer_init_value;

	}else if(config_struct_ptr->timer_id == timer1_ID){
		/*check the mode you want to work with */
		if(config_struct_ptr->mode == normal_mode){
			/*
			 * 1) COM1A1 = 0 & COM1A0 = 0 -> TO disable compare output mode of channel A
			 * 2) COM1B1 = 0 & COM1B0 = 0 -> TO disable compare output mode of channel B
			 * 3) FOC1A = 1 -> as this not PWM mode
			 * 4) WGM11 = 0 & WGM10 =0 -> TO work with the timer normal mode
			 */
			TCCR1A = (1<<FOC1A);
			 /*
			  * 1) WGM12 = 0 & WGM13 =0 -> TO work with the timer normal mode
			  * 2) adjust the clock select bits -> CS10 & CS11 & CS12
			  */
			TCCR1B = (config_struct_ptr->prescaler_value & TIMER_PRESCALER_MASK);
			/*enable the module interrupt for the overflow mode*/
			TIMSK |= (1<<TOIE1);
		}else if(config_struct_ptr->mode == compare_mode){
			/*
			 * 1) COM1A1 = 0 & COM1A0 = 0 -> TO disable compare output mode of channel A
			 * 2) COM1B1 = 0 & COM1B0 = 0 -> TO disable compare output mode of channel B
	     	 * 3) FOC1A = 1 -> as this not PWM mode
			 * 4) WGM11 = 0 & WGM10 =0 -> TO work with the timer compare mode
			 */
			TCCR1A = (1<<FOC1A);
			/*
			 * 1) WGM12 = 1 & WGM13 =0 -> TO work with the timer compare mode
			 * 2) adjust the clock select bits -> CS10 & CS11 & CS12
			 */
			TCCR1B = (1<<WGM12)|(config_struct_ptr->prescaler_value & TIMER_PRESCALER_MASK);
			/*enable the module interrupt for the compare on channel A mode*/
			TIMSK |= (1<<OCIE1A);
			/*set the compare value in OCR1 register*/
			OCR1A =  config_struct_ptr->timer_compare_value;
		}
		/*set the init value in the timer register*/
		TCNT1 = config_struct_ptr->timer_init_value;
	}else if(config_struct_ptr->timer_id == timer2_ID){
		/*check the timer mode you want to work with*/
			if(config_struct_ptr->mode == normal_mode){
			  /*enable the timer over flow interrupt*/
				TIMSK |= (1<<TOIE2);
				/*
				 * 1) FOC2 = 1 -> force output compare pin must equal to 1 when non PWM
				 * 2) WGM20 = 0 & WGM21 =0 -> TO work with normal mode
				 * 3) COM21 =0 & COM20 = 0 -> TO disconnect compare match output mode
				 * Note : this switch case because the prescaler table for timer2 is different than
				 * the prescaler tables for timer0 and timer 1
				 */
				switch(config_struct_ptr->prescaler_value){
				case no_prescaler : TCCR2 = (1<<FOC2)|(1<<CS20);
					break;
				case prescaler_8: TCCR2 = (1<<FOC2)|(1<<CS21);
					break;
				case prescaler_64: TCCR2 = (1<<FOC2)|(1<<CS22);
					break;
				case prescaler_256: TCCR2 = (1<<FOC2)|(1<<CS22)|(1<<CS21);
					break;
				case prescaler_1024: TCCR2 = (1<<FOC2)|(1<<CS22)|(1<<CS21)|(1<<CS20);
					break;
				}

			}else if(config_struct_ptr->mode == compare_mode){
				/*set the output compare value*/
				OCR2 = (uint8)config_struct_ptr->timer_compare_value;
				/*enable the output compare match interrupt*/
				TIMSK |= (1<<OCIE2);
				/*
				 * 1) FOC2 = 1 -> force output compare pin must equal to 1 when non PWM
				 * 2) WGM20 = 0 & WGM21 =1 -> TO work with compare  mode
				 * 3) COM21 =0 & COM20 = 0 -> TO disconnect compare match output mode
				 * Note : this switch case because the prescaler table for timer2 is different than
				 * the prescaler tables for timer0 and timer 1
				 */
				switch(config_struct_ptr->prescaler_value){
					case no_prescaler : TCCR2 = (1<<FOC2)|(1<<WGM21)|(1<<CS20);
						break;
					case prescaler_8: TCCR2 = (1<<FOC2)|(1<<WGM21)|(1<<CS21);
						break;
					case prescaler_64: TCCR2 = (1<<FOC2)|(1<<WGM21)|(1<<CS22);
						break;
					case prescaler_256: TCCR2 = (1<<FOC2)|(1<<WGM21)|(1<<CS22)|(1<<CS21);
						break;
					case prescaler_1024: TCCR2 = (1<<FOC2)|(1<<WGM21)|(1<<CS22)|(1<<CS21)|(1<<CS20);
						break;
					}
				}
				/*
				 * loading the timer init value in timer counter register
				 */
				TCNT2 = (uint8)config_struct_ptr->timer_init_value;
	}
}
/*
 * Function Description : to stop the timer with id and deinitialize the deriver
 */
void TIMER_deinit(timerID timer_id){
	if(timer_id == timer0_ID){
		TCCR0 = 0;
		TCNT0 = 0;
		OCR0 = 0;
		TIMSK &= ~(1<<TOIE0)&~(1<<OCIE0);/*disable module interrupt*/
	}else if(timer_id == timer1_ID){
		TCCR1A = 0;
		TCCR1B = 0;
		OCR1A = 0;
		TIMSK &= ~(1<<TOIE1)& ~(1<<OCIE1A);
	}else if(timer_id == timer2_ID){
		TCCR2 = 0;
		TCNT2 = 0;
		OCR2 = 0;
		TIMSK &= ~(1<<TOIE2)&~(1<<OCIE2);/*disable module interrupt*/
	}
}
/*
 * Function Description : set the function which will be called after the timer0 finishes counting
 */
void TIMER0_setCallBack(void(*a_ptr2fun0)(void)){
	g_ptr2fun0 = a_ptr2fun0;
}
/*
 * Function Description : set the function which will be called after the timer1 finishes counting
 */
void TIMER1_setCallBack(const void(*a_ptr2fun1)(void)){
	g_ptr2fun1 = a_ptr2fun1;
}
/*
 * Function Description : set the function which will be called after the timer2 finishes counting
 */
void TIMER2_setCallBack(const void(*a_ptr2fun2)(void)){
	g_ptr2fun2 = a_ptr2fun2;
}
