/*
 * calculate.h
 *
 *  Created on: 28 Dec 2021
 *      Author: muham
 */

#ifndef CALCULATE_H_ //header guard
#define CALCULATE_H_

//Libraries used
#include "PLL.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "lcd.h"
#include "keypad.h"
#include "Port.h"


//Functions to be used

//Given the user input string, it extracts the next number in the string (starting from the first number)
double getNextNumber(char array[]);

//Given the user input string, it extracts the operand from the string
char getNextOperand(char array[]);

//Performs the calculations and outputs answer to be displayed.
double calculate(char array[]);

//Given the string representation of the number containing the 'E', it outputs the value of that number
double getEValue(char array[]);

//waits a microsecond
void waitMicSec(int amount);


#endif /* CALCULATE_H_ */
