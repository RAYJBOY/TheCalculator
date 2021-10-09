/*
 * keypad.h
 *
 *  Created on: 28 Dec 2020
 *      Author: muham
 */

#ifndef KEYPAD_H_ //header guard
#define KEYPAD_H_

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

//All port D pins are set to high, then a pulse of 0 is sent over to check which button is pressed. Corresponding character is first added to user input array, and the returned to calling function
char getChar(char expression[], int* appendIndex, int* isShifted);

#endif /* KEYPAD_H_ */
