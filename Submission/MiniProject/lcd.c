#include "lcd.h"
/*
 * lcd.c
 *
 *  Created on: 28 Dec 2020
 *      Author: muham
 */

//Initialises the LCD based on commands in data sheet
void lcd_Init(void){

    //Timings in data sheet of LCD
    SysTick_Wait(3200000); //wait 40 ms
    SendDisplayNibble(0x3,'i');

    SysTick_Wait(6000); //equivalent to my_delay()
    SendDisplayNibble(0x3,'i');

    SysTick_Wait(8000); //wait 100 us
    SendDisplayNibble(0x3,'i');

    SysTick_Wait(2960); //wait 37 microseconds
    SendDisplayNibble(0x2,'i');
    SendInsByte(0x2,0x8); //set lines/font
    SendInsByte(0x0,0x8); //display off
    SendInsByte(0x0,0x1); //display clear
    SendInsByte(0x0,0x6); //entry mode set

}

//Input: A nibble to be sent to LCD
//Output: Sends a nibble to the LCD
//Sends a nibble to LCD. Used by almost all functions.
void SendDisplayNibble(char number, char InsOrData){ //PB[3],   PB[2],   PB[1],   PB[0]

    if(InsOrData == 'i'){
        GPIO_PORTA_DATA_R &= ~(1<<3); //RS is set to 0 for instructions
    }
    else if(InsOrData == 'd'){
        GPIO_PORTA_DATA_R |= (1<<3); //RS is set to 1 for data
    }

    GPIO_PORTA_DATA_R |= (1<<2); //EN line high
    SysTick_Wait(21); //wait for > 255 ns to send data

    GPIO_PORTB_DATA_R = number;

    SysTick_Wait(16); //wait 200 ns
    GPIO_PORTA_DATA_R &= ~(1<<2); //make EN low
    SysTick_Wait(44); //44 needed to complete cycle time of 1000 ns

}

//Input: 2 nibbles, or 1 byte representing an instruction.
//Output: Sends the byte to the LCD.
 //Uses SendDisplayNibble to send a byte of instructions
void SendInsByte(unsigned int number, unsigned int number2){ //PB[3],   PB[2],   PB[1],   PB[0],   PB[3],   PB[2],   PB[1],   PB[0]

    SendDisplayNibble(number,'i');
    SendDisplayNibble(number2,'i');
    SysTick_Wait(2960); //wait for 37 microseconds

}

//Input: 2 nibbles, or 1 byte representing data.
//Output: Sends the byte to the LCD.
//Uses SendDisplayNibble to send a byte of data
void SendDataByte(unsigned int number, unsigned int number2){ //PB[3],   PB[2],   PB[1],   PB[0],   PB[3],   PB[2],   PB[1],   PB[0]

    SendDisplayNibble(number,'d');
    SendDisplayNibble(number2,'d');
    SysTick_Wait(2960); //wait for 37 microseconds

}

//Turns on the LCD and sets cursor to beginning
void onDisplay(void){

    SendInsByte(0x0,0xE); //display on
    SendInsByte(0x8,0x0); //set cursor to correct position

}

//Input: character to be printed on LCD, current index of user input, integer to represent if the answer should be displayed, integer representing if shift key is pressed, user input array, and the index used for calculations
//Output: Sends the byte to the LCD.
//Uses SendDisplayNibble to send a byte of data
void printDisplay(char myChar, int* appendIndex, int* displayAnswer, int isShifted, char expression[], int* Index){

    (*displayAnswer) = 0; //assume not to display answer until enter pressed

    //send corresponding character to LCD
    if(myChar == '0'){
        SendDataByte(0x3,0x0);
    }

    else if(myChar == '1'){
        SendDataByte(0x3,0x1);
    }

    else if(myChar == '2'){
        SendDataByte(0x3,0x2);
    }

    else if(myChar == '3'){
        SendDataByte(0x3,0x3);
    }

    else if(myChar == '4'){
        SendDataByte(0x3,0x4);
    }

    else if(myChar == '5'){
        SendDataByte(0x3,0x5);
    }

    else if(myChar == '6'){
        SendDataByte(0x3,0x6);
    }

    else if(myChar == '7'){
        SendDataByte(0x3,0x7);
    }

    else if(myChar == '8'){
        SendDataByte(0x3,0x8);
    }

    else if(myChar == '9'){
        SendDataByte(0x3,0x9);
    }

    else if(myChar == '-'){
        SendDataByte(0x2,0xD);
    }

    else if(myChar == '.'){
        SendDataByte(0x2,0xE);
    }


    else if(myChar == 'A'){ //if A pressed...
        if(isShifted){ //if shift key pressed
            expression[*appendIndex] = 'x'; //add 'x' to user input
            (*appendIndex)++;
            SendDataByte(0x7,0x8); //prints 'x'
        }
        else{ //shift key not pressed...
            expression[*appendIndex] = '+'; //add '+' to user input
            (*appendIndex)++;
            SendDataByte(0x2,0xB); //prints '+'
        }
    }

    else if(myChar == 'B'){ //if B pressed...
        if(isShifted){
            expression[*appendIndex] = '/'; //add '/' to user input
            (*appendIndex)++;
            SendDataByte(0x2,0xF); //prints '/'
        }
        else{ //if shift not pressed...
            expression[*appendIndex] = '-'; //add '-' to user input
            (*appendIndex)++;
            SendDataByte(0x2,0xD); //prints '-'
        }
    }

    else if(myChar == 'C'){ //if C pressed...
        if(isShifted){
            expression[*appendIndex] = 'E'; //add 'E' to user input
            (*appendIndex)++;
            SendDataByte(0x4,0x5); //prints 'E'
        }
        else{ //if shift not pressed...
            expression[*appendIndex] = '.'; //add '.' to user input
            (*appendIndex)++;
            SendDataByte(0x2,0xE); //prints '.'
        }
    }

    else if(myChar == '*'){ //if enter pressed...
        expression[*appendIndex] = '\0'; //end user input
        (*displayAnswer) = 1; //answer should be displayed
    }

    else if(myChar == '#'){ //if '#' pressed...
        if(isShifted){ //if shift is pressed...
            clearDisplay(appendIndex, displayAnswer, Index); //clear display
        }
        else{
            //rubout last character
            SendInsByte(0x0,0x4); //cursor moves left by one after write
            SendDataByte(0x2,0x0); //prints blank space
            (*appendIndex)--;
            expression[*appendIndex] = '\0';
            SendInsByte(0x0,0x6); //cursor moves right by one after write
            SendDataByte(0x2,0x0); //prints blank space
            SendInsByte(0x0,0x4); //cursor moves left by one after write
            SendDataByte(0x2,0x0); //prints blank space
            SendInsByte(0x0,0x6); //cursor moves right by one after write
        }
    }

}

//Input: current index of user input, integer to represent whether answer should be displayed, and integer representing the computation index
//Output: clears the LCD
void clearDisplay(int* appendIndex, int* displayAnswer, int* Index){

    (*appendIndex) = 0; //restart user input
    (*displayAnswer) = 0; //do not display answer
    (*Index) = 0; //restart computation
    SendInsByte(0x0,0x1); //clears display and moves cursor to beginning

}

//sets cursor to start on line 2
void nextLine(void){

    SendInsByte(0xC,0x0);

}

//shows error message to user if he tries to divide by zero
void divByZero(void){

    SendInsByte(0x0,0x1);
    SendDataByte(0x4,0x3); //C
    SendDataByte(0x4,0x1); //A
    SendDataByte(0x4,0xE); //N
    SendDataByte(0x4,0xE); //N
    SendDataByte(0x4,0xF); //O
    SendDataByte(0x5,0x4); //T
    SendDataByte(0x2,0x0); //Space
    SendDataByte(0x4,0x4); //D
    SendDataByte(0x4,0x9); //I
    SendDataByte(0x5,0x6); //V
    SendDataByte(0x4,0x9); //I
    SendDataByte(0x4,0x4); //D
    SendDataByte(0x4,0x5); //E
    nextLine(); //next line
    SendDataByte(0x4,0x2); //B
    SendDataByte(0x5,0x9); //Y
    SendDataByte(0x2,0x0); //Space
    SendDataByte(0x5,0xA); //Z
    SendDataByte(0x4,0x5); //E
    SendDataByte(0x5,0x2); //R
    SendDataByte(0x4,0xF); //O

}
