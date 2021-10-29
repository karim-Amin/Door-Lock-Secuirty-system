<<<<<<< HEAD
 /******************************************************************************
 *
 * Module: MOTOR
 *
 * File Name: motor.h
 *
 * Description: header file for motor external driver
 *
 * Author: Karim Mohamed Amin
 *
 *******************************************************************************/

#ifndef MOTOR_H_
#define MOTOR_H_
#include"std_types.h"
/*
 * *****					Definitions						*******
 */
#define DC_MOTOR_PORT_ID PORTB_ID
#define IN1_PIN_ID PIN1_ID
#define IN2_PIN_ID PIN2_ID
/*
 ************************ Type Definitions *********************************
 */
typedef enum{
	stop_motor,clock_wise,anti_clock_wise
}DcMotor_State;
/*
 * ************************	Functions Prototypes ***********************
 */

void DcMotor_init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);
#endif /* MOTOR_H_ */
=======
 /******************************************************************************
 *
 * Module: MOTOR
 *
 * File Name: motor.h
 *
 * Description: header file for motor external driver
 *
 * Author: Karim Mohamed Amin
 *
 *******************************************************************************/

#ifndef MOTOR_H_
#define MOTOR_H_
#include"std_types.h"
/*
 * *****					Definitions						*******
 */
#define DC_MOTOR_PORT_ID PORTB_ID
#define IN1_PIN_ID PIN1_ID
#define IN2_PIN_ID PIN2_ID
/*
 ************************ Type Definitions *********************************
 */
typedef enum{
	stop_motor,clock_wise,anti_clock_wise
}DcMotor_State;
/*
 * ************************	Functions Prototypes ***********************
 */

void DcMotor_init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);
#endif /* MOTOR_H_ */
>>>>>>> main program structure
