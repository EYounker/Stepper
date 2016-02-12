#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "PLL.h"
#include "SysTick.h"
#include "tm4c123gh6pm.h"
#include "Logo.txt"

void PortF_Init(void);


//========================================================================================
//Initialize Port F
void PortF_Init(void){ volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x00000020;  										// Activate clock for Port F
  delay = SYSCTL_RCGCGPIO_R;        										// Allow time for clock to start
  GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;  									// Unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           										// Allow changes to PF4-0
  GPIO_PORTF_AMSEL_R = 0x00;        										// Disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   										// PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E; //0x11;  										// PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        										// Disable alternate functions on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          										// Enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;        											// Enable digital I/O on PF4-0
	GPIO_PORTF_IS_R &= ~0x11;    													// PF1 & PF4 is edge-sensitive
	GPIO_PORTF_IBE_R |= 0x11;    													// PF1 & PF4 is both edges
	GPIO_PORTF_IM_R |= 0x11;      												// Mask set to interrupt on PF1 & PF4 only       	*** No IME bit as mentioned in Book ***
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000;		// Priority 5		
  NVIC_EN0_R = 0x40000000;     	 												// Enable interrupt 30 in NVIC
	GPIO_PORTF_ICR_R = 0x11;}    													// Clear flag 4 and 1


	
//========================================================================================
// Port F's Interrupt Service Routine (ISR)
void GPIOPortF_Handler(){
	GPIO_PORTF_ICR_R = 0x11;	
	ST7735_DrawBitmap(48, 160, Logo, 80, 80);
	SysTick_Wait10ms(100);
	ST7735_FillScreen(0xFFFF);
	}
