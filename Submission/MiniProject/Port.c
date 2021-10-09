/*
 * Port.c
 *
 *  Created on: 5 Jan 2021
 *      Author: muham
 */

#include "Port.h"

//PE3-PE0 used for keypad input
void PortE_Init(void){

  SYSCTL_RCGCGPIO_R |= 0x00000010;  // 1) E clock
  while((SYSCTL_PRGPIO_R&0x00000010)==0){} // delay
  GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock PortE
  GPIO_PORTE_CR_R = 0x0F;           // allow changes to PE3-PE0
  GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTE_DIR_R = 0x00;          // 5) PE3-PE0 inputs
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTE_DEN_R = 0x0F;          // 7) enable digital pins PE3-PE0

}

//PD3-PD0 used as keypad outputs
void PortD_Init(void){

  SYSCTL_RCGCGPIO_R |= 0x00000008;  // 1) D clock
  while((SYSCTL_PRGPIO_R&0x00000008)==0){} // delay
  GPIO_PORTD_LOCK_R = 0x4C4F434B;   // 2) unlock PortD
  GPIO_PORTD_CR_R = 0x0F;           // allow changes to PD3-PD0
  GPIO_PORTD_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTD_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTD_DIR_R = 0x0F;          // 5) PD3-PD0 output
  GPIO_PORTD_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTD_DEN_R = 0x0F;          // 7) enable digital pins PD3-PD0

}

//PB3-PB0 used for LCD data bits (output pins)
void PortB_Init(void){

  SYSCTL_RCGCGPIO_R |= 0x00000002;  // 1) B clock
  while((SYSCTL_PRGPIO_R&0x00000002)==0){} // delay
  GPIO_PORTB_LOCK_R = 0x4C4F434B;   // 2) unlock PortB
  GPIO_PORTB_CR_R = 0x0F;           // allow changes to PB3-PB0
  GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTB_DIR_R = 0x0F;          // 5) PB3-PB0 output
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTB_DEN_R = 0x0F;          // 7) enable digital pins PB4-PB0

}

//Used for EN bit and RS bit
void PortA_Init(void){

  SYSCTL_RCGCGPIO_R |= 0x00000001;  // 1) A clock
  while((SYSCTL_PRGPIO_R&0x00000001)==0){} // delay
  GPIO_PORTA_LOCK_R = 0x4C4F434B;   // 2) unlock PortA
  GPIO_PORTA_CR_R = 0x0F;           // allow changes to PA3-PA0
  GPIO_PORTA_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTA_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTA_DIR_R = 0x0F;          // 5) PA3-PA0 output
  GPIO_PORTA_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTA_DEN_R = 0x0F;          // 7) enable digital pins PA4-PA0

}
