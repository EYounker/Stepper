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


volatile uint32_t NextState = 0; 
//========================================================================================
//Initialize Port E

void PortE_Init(void){ volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= b4;  														// Activate clock for Port F
  delay = SYSCTL_RCGCGPIO_R;        										// Allow time for clock to start
  GPIO_PORTE_AMSEL_R &= ~0xF;        										// Disable analog on PF
  GPIO_PORTE_PCTL_R &= ~0x7FFF;   											// PCTL GPIO on PF4-0
  GPIO_PORTE_DIR_R &= ~0xF;  														// PF4,PF0 in, PF3-1 out
  GPIO_PORTE_AFSEL_R &= ~0xF;        										// Disable alternate functions on PF7-0
  GPIO_PORTE_PUR_R &= ~0xF;          										// Enable pull-up on PF0 and PF4
  GPIO_PORTE_DEN_R |= 0xF;        											// Enable digital I/O on PF4-0
	GPIO_PORTE_IEV_R |= 0xF;
	GPIO_PORTE_IS_R &= ~0x9;    													// PF1 & PF4 is edge-sensitive
	GPIO_PORTE_IS_R |= 0x6;    														// PF1 & PF4 is edge-sensitive
	GPIO_PORTE_IBE_R &= ~0xF;   													// PF1 & PF4 is both edges
	GPIO_PORTE_IM_R |= 0xF;      													// Mask set to interrupt on PF1 & PF4 only       	*** No IME bit as mentioned in Book ***
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF0F)|0x00000040;		// Priority 2		
  NVIC_EN0_R = 0x10;     	 												// Enable interrupt 30 in NVIC
	GPIO_PORTE_ICR_R = 0xF;    													// Clear flag 4 and 1
	GPIO_PORTE_DATA_R = 0;
}
	
//========================================================================================
// Port E's Interrupt Service Routine (ISR)
void GPIOPortE_Handler(){
	if ((GPIO_PORTE_MIS_R & 9) != 0){											  //single step inputs 0 and 3 rising edge trigger... reset flag on 
		GPIO_PORTE_ICR_R = 9;}
	if ((GPIO_PORTE_MIS_R & 3) != 0){											  //Left buttons
		NextState = 1;}
	if ((GPIO_PORTE_MIS_R & 12) != 0){											//Right buttons
		NextState = 0;}
	SysTick_Wait10ms(5);																	// wait 10 ms * current state's Time value
}//end Portehandler
