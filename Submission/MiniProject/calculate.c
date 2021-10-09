/*
 * calculate.c
 *
 *  Created on: 10 Dec 2020
 *      Author: muham
 */

#include "calculate.h"

// 2. Declarations Section
//   Global Variables
int isShifted; //used in lcd.c and keypad.c
char expression[16]; //used in lcd.c, keypad.c and calculate.c
int Index = 0; //used in calculate.c
int appendIndex =  0; //used in lcd.c and keypad.c
int hasE; //used in calculate.c
int displayAnswer = 0; //used in calculate.c and lcd.c
int divideByZero; //used to check if division by zero

//Function prototypes in header

// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable

int main(void){    

    //Initialise timer
    PLL_Init();
    SysTick_Init();

    //Initialise ports
    PortA_Init();
    PortB_Init();
    PortD_Init();
    PortE_Init();

    //Prepare LCD
    lcd_Init();
    onDisplay();

    char tempChar;

        while(1){
        tempChar = getChar(expression, &appendIndex, &isShifted); //read character from user input
        printDisplay(tempChar, &appendIndex, &displayAnswer, isShifted, expression, &Index); //print the character on LCD

        if(displayAnswer){ //if user presses ENTER...

              nextLine(); //answer displayed on next line

              double ans = calculate(expression); //perform calculation and store answer as a double

              if(divideByZero){ //if there was an error (division by zero)...
                  divByZero(); //show error
              }

              else{ //if calculation was fine, display answer...

                  int beforeDec = (int)ans; //used to display answer before decimal place

                  char belowTen[1] = "";

                  if(abs(100.0*(ans-beforeDec))<10){ //used to display "0" right after decimal point
                      belowTen[0] = '0';
                  }

                  double afterDecimalDigits = abs(100.0*(ans-beforeDec)); //used to display answer after decimal point to 2 dp
                  int afterDec = (int)afterDecimalDigits;

                  char beforeDecimal[16] = "";
                  char afterDecimal[3] = "";

                  __itoa(beforeDec, beforeDecimal, 10); //convert answer before decimal place to integer
                  __itoa(afterDec, afterDecimal, 10); //-convert answer after decimal place to integer


                  for(int i = 0; i<16; i++){
                      printDisplay(beforeDecimal[i], &appendIndex, &displayAnswer, isShifted, expression, &Index); //print everything before dp
                  }

                  printDisplay('.', &appendIndex, &displayAnswer, isShifted, expression, &Index); //print decimal point

                  printDisplay(belowTen[0], &appendIndex, &displayAnswer, isShifted, expression, &Index); //if there is a zero right after dp, then print that

                  for(int i = 0; i<4; i++){
                      printDisplay(afterDecimal[i], &appendIndex, &displayAnswer, isShifted, expression, &Index); //print the rest of the answer after decimal point
                  }
              }
          }

        waitMicSec(10000); //wait 10 millisecond, or character will print multiple times as Tina updates 10 ms
    }

}

//Input: string from user
//Output: answer to expression
//Performs the calculations and outputs answer to be displayed.
double calculate(char array[]){

    divideByZero = 0; //assume there is no division by zero until there is
    double result = 0.0; //stores the answers to calculation

    double Firstnumber; //stores the first number
    double Secondnumber; //stores the second number
    char tempOperand; //stores operand

    Firstnumber = getNextNumber(array);
    tempOperand = getNextOperand(array);
    Secondnumber = getNextNumber(array);

    if(tempOperand == 'x'){ //multiplication
      result = Firstnumber*Secondnumber;
    }

    else if(tempOperand == '/'){ //division
        if(Secondnumber == 0){ //checks for division by zero
            divideByZero = 1; //if so, global variable is updated
        }
      result = Firstnumber/Secondnumber; //performs division regardless of whether there as a div by zero
    }

    else if(tempOperand == '+'){ //addition
      result = Firstnumber+Secondnumber;
    }

    else if(tempOperand == '-'){ //subtraction
      result = Firstnumber-Secondnumber;
    }

    return result; //outputs the answer

}

//Input: User input string
//Output: Gets the number in the string
//Given the user input string, it extracts the next number in the string (starting from the first number)
double getNextNumber(char array[]){

    int i = 0;
    int arraySize = 0; //used to check if there are already numbers in the result hen a '-' sign appears. Was used to stop if a minus sign appears after getting a number
    int isFloat = 0; //used to check if the number is a float
    double result; //stores the result
    char tempArray[16] = ""; //stores the digits of the number
    hasE = 0; //used to check if the number contains 'E'; ...times 10 to the power of...
    int firstNegative = 0; //used to check if it is the first '-' sign to appear

    for(; Index<16; Index++){
      if(isdigit(array[Index]) || array[Index] == '.' || array[Index] == 'E' || array[Index] == '-'){ //adds the corresponding digits (all of these could be part of the number) to tempArray
        if(array[Index] == 'E'){ //it has an 'E'
          hasE = 1;
        }
        if(array[Index] == '.'){ //it is a floating point value
          isFloat = 1;
        }
        if(array[Index] == '-'){
            if(firstNegative || arraySize != 0){ //if the '-' sign appears after getting a number, or if a '-' sign has already appeared before, then break
                break;
            }
            firstNegative = 1; //sets this to 1 after getting the first '-' sign
        }
        tempArray[i] = array[Index]; //tempArray stores all digits of the number
        i++;
      }
      else if(array[Index] == 'x' || array[Index] == '/' || array[Index] == '-' || array[Index] == '+'){ //if it is an operand, then break (not part of the number)
        break;
      }
      else if(array[Index] == '\0'){ //if end of string (last number has been encountered), break
        break;
      }
      arraySize++;
    }

    tempArray[i] = '\0'; //null terminate number string

    if(hasE){ //if the number had 'E', get the value of this by calling "getEValue()" function
      result = getEValue(tempArray);
    }

    else if(isFloat){ //if the number is a float...

        if(tempArray[0] == '-'){ //if it was a negative float...
            char negativeNumber[15];
            int numberIndex;
            for(numberIndex = 0; numberIndex < 15; numberIndex++){
                negativeNumber[numberIndex] = tempArray[numberIndex+1]; //copy only the number (omit the '-' sign) into "negativeNumber[]". Atof was not accepting '-' in the number, so it had to be multiplied by -1 manually
            }

            int tempResult = atoi(negativeNumber); //gets the integer part
            result = -1.0*(atof(negativeNumber) + tempResult); //gets the part after the decimal place, and returns the negative result
        }

        else{ //if float value was positive...
            int tempResult = atoi(tempArray);
            result = atof(tempArray) + tempResult; //return the value
        }

    }

    else{ //if the number was not a float and did not have an 'E'...
        result = atoi(tempArray); //it is an integer, so return the value
    }

    return result;

}

//Input: User input string
//Output: Gets the operand in the string
//Given the user input string, it extracts the operand from the string
char getNextOperand(char array[]){

  char operand; //stores the operand as a char

  for(; Index<16; Index++){

    if(array[Index] == 'x' || array[Index] == '/' || array[Index] == '-' || array[Index] == '+'){ //if operand...
      operand = array[Index];
      Index++;
      return operand; //return the operand
    }

    else{
      return 'z'; //if not an operand, return an arbitrary char
    }

  }

  return 'z'; //no operand, meaning return arbitrary char

}

//Input: String representation of a number which has 'E'
//Output: Returns the value of that number
//Given the string representation of the number containing the 'E', it outputs the value of that number
double getEValue(char array[]){

    int i;
    int j = 0;
    int k = 0;
    char firstNumber[16] = ""; //used to store everything before the 'E'
    char secondNumber[16] = ""; //stores everything after the 'E'

    int firstNumberFloat = 0; //checks if the first number is a float
    int secondNumberFloat = 0; //checks if the second number is a float


    for(i=0; i<16; i++){ //stores the first part of the number (until hitting 'E') in "firstNumberFloat"
      if(array[i] == '\0' || array[i] == 'E'){
        break;
      }
      else if(isdigit(array[i]) || array[i] == '.'){
          if(array[i] == '.'){
              firstNumberFloat = 1;
          }
          firstNumber[j] = array[i];
          j++;
      }
    }

    firstNumber[j] = '\0'; //null terminates the first part of the number

    for(; i<16; i++){ //stores the second part of the number (after hitting the 'E') in "secondNumber"
        if(array[i] == 'E'){ //skips over 'E'

        }
        else if(array[i] == '\0'){ //breaks if end of number reached
            break;
        }
        else{
            if(array[i] == '.'){ //checks if second number is a float
                secondNumberFloat = 1;
            }
            secondNumber[k] = array[i];
            k++;
        }
    }

    secondNumber[k] = '\0'; //null terminates the second part of the string representation of number

    double firstNumberValue;
    double secondNumberValue;

    if(firstNumberFloat){ //if first number is float, gets value of first number
        int tempResult = atoi(firstNumber);
        firstNumberValue = atof(firstNumber) + tempResult;
    }

    if(secondNumberFloat){ //if second number is float, gets value of second number
        int tempResult = atoi(secondNumber);
        secondNumberValue = atof(secondNumber) + tempResult;
    }

    if(!firstNumberFloat){ //if first number is not a float, gets value of first number
        firstNumberValue = atoi(firstNumber);
    }

    if(!secondNumberFloat){ //if second number is not a float, gets value of second number
        secondNumberValue = atoi(secondNumber);
    }

    double finalValue;
    finalValue = firstNumberValue * pow(10.0, secondNumberValue); //gets the value of the number

    return finalValue;

}

//waits a microsecond
void waitMicSec(int amount){

    int i = 0;
    for(i=0;i<amount;i++){
        SysTick_Wait(80); //waits one microsecond
    }

}
