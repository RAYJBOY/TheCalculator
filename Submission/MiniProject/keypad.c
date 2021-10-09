/*
 * keypad.c
 *
 *  Created on: 28 Dec 2020
 *      Author: muham
 */

#include "keypad.h"

//Input: array that stores the user input (expression[]), current index to store the characters (appendIndex), and a pointer to an integer representing if the shift key is pressed (isShifted)
//Output: the character corresponding to the button pressed by the user. Also stores the numbers in the user input array
//All port D pins are set to high, then a pulse of 0 is sent over to check which button is pressed. Corresponding character is first added to user input array, and the returned to calling function
char getChar(char expression[], int* appendIndex, int* isShifted){

    //-----------------------------------------------------------
       GPIO_PORTD_DATA_R = 0x0F; //make PD[0]:PD[3] = 1
       GPIO_PORTD_DATA_R &= ~(0x01); //make PD[0] = 0

       if(!(GPIO_PORTE_DATA_R & (0x01))){ //check if PE[0]==0
           expression[*appendIndex] = '1'; //add 1 user input array
           (*appendIndex)++;
           return '1';
       }
       else if(!(GPIO_PORTE_DATA_R & (0x02))){ //check if PE[1]==0
           expression[*appendIndex] = '4'; //add 4 to user input array
           (*appendIndex)++;
           return '4';
       }
       else if(!(GPIO_PORTE_DATA_R & (0x04))){ //check if PE[2]==0
           expression[*appendIndex] = '7'; //add 7 to user input array
           (*appendIndex)++;
           return '7';
       }
       else if(!(GPIO_PORTE_DATA_R & (0x08))){ //check if PE[3]==0
           return '*'; //Enter pressed
       }



       //------------------------------------------------------------
       GPIO_PORTD_DATA_R |= (0x01); //make PD[0] = 1
       GPIO_PORTD_DATA_R &= ~(0x02); //make PD[1] = 0

       if(!(GPIO_PORTE_DATA_R & (0x01))){ //check if PE[0]==0
           expression[*appendIndex] = '2'; //add 2 to user input array
           (*appendIndex)++;
           return '2';
       }
       else if(!(GPIO_PORTE_DATA_R & (0x02))){ //check if PE[1]==0
           expression[*appendIndex] = '5'; //add 5 to user input array
           (*appendIndex)++;
           return '5';
       }
       else if(!(GPIO_PORTE_DATA_R & (0x04))){ //check if PE[2]==0
           expression[*appendIndex] = '8'; //add 8 to user input array
           (*appendIndex)++;
           return '8';
       }
       else if(!(GPIO_PORTE_DATA_R & (0x08))){ //check if PE[3]==0
           expression[*appendIndex] = '0';//add 0 to user input array
           (*appendIndex)++;
           return '0';
       }



       //------------------------------------------------------------
       GPIO_PORTD_DATA_R |= (0x02); //make PD[1] = 1
       GPIO_PORTD_DATA_R &= ~(0x04); //make PD[2] = 0

       if(!(GPIO_PORTE_DATA_R & (0x01))){ //check if PE[0]==0
           expression[*appendIndex] = '3';
           (*appendIndex)++;
           return '3';
       }
       else if(!(GPIO_PORTE_DATA_R & (0x02))){ //check if PE[1]==0
           expression[*appendIndex] = '6'; //add 6 to user input array
           (*appendIndex)++;
           return '6';
       }
       else if(!(GPIO_PORTE_DATA_R & (0x04))){ //check if PE[2]==0
           expression[*appendIndex] = '9'; //add 9 to user input array
           (*appendIndex)++;
           return '9';
       }
       else if(!(GPIO_PORTE_DATA_R & (0x08))){ //check if PE[3]==0
           return '#'; //Hashtag pressed
       }



       //------------------------------------------------------------
       GPIO_PORTD_DATA_R |= (0x04); //make PD[2] = 1
       GPIO_PORTD_DATA_R &= ~(0x08); //make PD[3] = 0

       if(!(GPIO_PORTE_DATA_R & (0x01))){ //check if PE[0]==0
           return 'A'; //A pressed
       }
       else if(!(GPIO_PORTE_DATA_R & (0x02))){ //check if PE[1]==0
           return 'B'; //B pressed
       }
       else if(!(GPIO_PORTE_DATA_R & (0x04))){ //check if PE[2]==0
           return 'C'; //C pressed
       }
       else if(!(GPIO_PORTE_DATA_R & (0x08))){ //check if PE[3]==0
           (*isShifted) = 1; //D is used for shift
       }
       else if((GPIO_PORTE_DATA_R & (0x08))){ //check if PE[3]==1
           (*isShifted) = 0; //D is used for shift
       }

       return 'Z'; //return arbitrary value if nothing pressed

}


