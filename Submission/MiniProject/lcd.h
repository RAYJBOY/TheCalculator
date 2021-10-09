/*
 * lcd.h
 *
 *  Created on: 28 Dec 2020
 *      Author: muham
 */

#ifndef LCD_H_ //header guard
#define LCD_H_

//Libraries used
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "calculate.h"
#include "PLL.h"
#include "Port.h"

//Functions used

//Initialises the LCD based on commands in data sheet
void lcd_Init(void);

//Sends a nibble to LCD. Used by almost all functions.
void SendDisplayNibble(char number, char InsOrData);

//Uses SendDisplayNibble to send a byte of instructions
void SendInsByte(unsigned int number, unsigned int number2);

//Uses SendDisplayNibble to send a byte of data
void SendDataByte(unsigned int number, unsigned int number2);

//Uses SendDisplayNibble to send a byte of data
void printDisplay(char myChar, int* appendIndex, int* displayAnswer, int isShifted, char expression[], int* Index);

//Turns on the LCD and sets cursor to beginning
void onDisplay(void);

//clears the LCD
void clearDisplay(int* appendIndex, int* displayAnswer, int* Index);

//sets cursor to start on line 2
void nextLine(void);

//shows error message to user if he tries to divide by zero
void divByZero(void);


#endif /* LCD_H_ */
