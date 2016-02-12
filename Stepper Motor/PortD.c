#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"
#include "SysTick.h"

#define b0  0x01;
#define b1  0x02;
#define b2  0x04;
#define b3  0x08;
#define b4  0x10;
#define b5  0x20;
#define b6  0x40;
#define b7  0x80;


//========================================================================================
//Initialize Port D

void PortD_Init(void){ volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= b3;  														// Activate clock for Port F
  delay = SYSCTL_RCGCGPIO_R;        										// Allow time for clock to start
  GPIO_PORTD_AMSEL_R &= ~0xF;        										// Disable analog on PF
  GPIO_PORTD_PCTL_R &= ~0x7FFF;   											// PCTL GPIO on PF4-0
  GPIO_PORTD_DIR_R |= 0xF;  														// PF4,PF0 in, PF3-1 out
  GPIO_PORTD_AFSEL_R &= ~0xF;        										// Disable alternate functions on PF7-0
  GPIO_PORTD_PUR_R &= ~0xF;          										// Enable pull-up on PF0 and PF4
  GPIO_PORTD_DEN_R |= 0xF;      											// Enable digital I/O on PF4-0
	GPIO_PORTD_DATA_R = 0;
}//suck it alex
//========================================================================================
