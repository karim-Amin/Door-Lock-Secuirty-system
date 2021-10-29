 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: header file for timer driver in AVR this will support all the timers
 * with (compare mode OR over flow mode (normal mode) )
 *
 * Author: Karim Mohamed Amin
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"
/***********************************************************************************************************
 *												 Definitions
 ************************************************************************************************************/

/***********************************************************************************************************
 *												Type Definitions
 ************************************************************************************************************/
typedef enum{
	no_prescaler = 1,prescaler_8,prescaler_64,prescaler_256,prescaler_1024
}prescaler;
typedef enum{
	normal_mode,compare_mode
}timerMode;
typedef enum{
	timer0_ID,timer1_ID,timer2_ID
}timerID;
typedef struct {
	timerID timer_id;
	timerMode mode;
	prescaler prescaler_value;
	uint16 timer_init_value;
	uint16 timer_compare_value;
}configType;
/***********************************************************************************************************
 *												Function prototypes
 ************************************************************************************************************/
void TIMER_init(const configType* config_struct_ptr);
void TIMER0_setCallBack(void(*a_ptr2fun0)(void));
void TIMER1_setCallBack(const void(*a_ptr2fun1)(void));
void TIMER2_setCallBack(const void(*a_ptr2fun2)(void));
void TIMER_deinit(uint8 timer_id);
#endif /* TIMER_H_ */
