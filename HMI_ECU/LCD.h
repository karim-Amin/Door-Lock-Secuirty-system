<<<<<<< HEAD
 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: LCD.h
 *
 * Description: Header file for the AVR LCD driver
 *
 * Author: Karim Mohamed Amin
 *
 *******************************************************************************/
#ifndef LCD_H_
#define LCD_H_
#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define LCD_DATA_PORT_ID PORTA_ID
#define LCD_CONTROL_PORT_ID PORTB_ID
#define LCD_RS_PIN_ID PIN0_ID
#define LCD_RW_PIN_ID PIN1_ID
#define LCD_E_PIN_ID PIN2_ID
/* LCD Commands*/
#define CLEAR_COMMAND 0x01
#define RETURN_HOME_COMMAND 0x02
#define Eight_BITS_COMMAND 0x38
#define CURSOR_ON_COMMAND 0x0E
#define CURSOR_OFF_COMMAND 0x0C
#define SET_CURSOR_LOCATION_COMMAND 0x80
/* start Addresses in lcd*/
#define SECOND_ROW_START_ADDRESS 0x40
#define THIRD_ROW_START_ADDRESS 0x10
#define FOURTH_ROW_START_ADDRESS 0x50
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void LCD_sendCommand(uint8 a_command);
void LCD_displayCharacter(uint8 a_data);
void LCD_displayString(const uint8* a_data_ptr);
void LCD_init(void);
void LCD_clear(void);
void LCD_moveCursor(uint8 a_row,uint8 a_col);
void LCD_displayStringRowColumn(uint8 a_row,uint8 a_col,const uint8* a_data_ptr);
void LCD_intgerToString(const uint32 a_data);

#endif /* LCD_H_ */
=======
 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: LCD.h
 *
 * Description: Header file for the AVR LCD driver
 *
 * Author: Karim Mohamed Amin
 *
 *******************************************************************************/
#ifndef LCD_H_
#define LCD_H_
#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define LCD_DATA_PORT_ID PORTA_ID
#define LCD_CONTROL_PORT_ID PORTB_ID
#define LCD_RS_PIN_ID PIN0_ID
#define LCD_RW_PIN_ID PIN1_ID
#define LCD_E_PIN_ID PIN2_ID
/* LCD Commands*/
#define CLEAR_COMMAND 0x01
#define RETURN_HOME_COMMAND 0x02
#define Eight_BITS_COMMAND 0x38
#define CURSOR_ON_COMMAND 0x0E
#define CURSOR_OFF_COMMAND 0x0C
#define SET_CURSOR_LOCATION_COMMAND 0x80
/* start Addresses in lcd*/
#define SECOND_ROW_START_ADDRESS 0x40
#define THIRD_ROW_START_ADDRESS 0x10
#define FOURTH_ROW_START_ADDRESS 0x50
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void LCD_sendCommand(uint8 a_command);
void LCD_displayCharacter(uint8 a_data);
void LCD_displayString(const uint8* a_data_ptr);
void LCD_init(void);
void LCD_clear(void);
void LCD_moveCursor(uint8 a_row,uint8 a_col);
void LCD_displayStringRowColumn(uint8 a_row,uint8 a_col,const uint8* a_data_ptr);
void LCD_intgerToString(const uint32 a_data);

#endif /* LCD_H_ */
>>>>>>> main program structure
