 /******************************************************************************
 *
 * Module: MOTOR
 *
 * File Name: motor.c
 *
 * Description: source file for motor external driver
 *
 * Author: Karim Mohamed Amin
 *
 *******************************************************************************/
#include "motor.h"
#include "gpio.h"
#include "pwm.h"
/*
 * Description:
 *	set up the direction of pins connected to the motor
 *	initially the motor will have no speed
 */
void DcMotor_init(void){
	/*set up the direction for two pins in1 and in2 to be output pins*/
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID,IN1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID,IN2_PIN_ID,PIN_OUTPUT);
	/*Turn off the motor at the beginning of the application */
	GPIO_writePin(DC_MOTOR_PORT_ID,IN1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT_ID,IN2_PIN_ID,LOGIC_LOW);

}
/*
 * Description:
 * 	1)control the direction of rotation for the motor
 * 	2)set up the motor speed
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	if(state == clock_wise){
		/*to make the motor rotates clock wise we should in1->1 : in2->0*/
		GPIO_writePin(DC_MOTOR_PORT_ID,IN1_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT_ID,IN2_PIN_ID,LOGIC_LOW);
	}else if(state == anti_clock_wise){
		/*to make the motor rotates clock anti-wise we should in1->0 : in2->1*/
		GPIO_writePin(DC_MOTOR_PORT_ID,IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID,IN2_PIN_ID,LOGIC_HIGH);
	}else{
		/*Turn off the motor*/
		GPIO_writePin(DC_MOTOR_PORT_ID,IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID,IN2_PIN_ID,LOGIC_LOW);
	}
	PWM_Timer0_Start(speed);/*The PWM signal will be driven in PB3*/

}
