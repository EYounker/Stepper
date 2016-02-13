// ST7735TestMain.c
// Runs on LM4F120/TM4C123
// Test the functions in ST7735.c by printing basic
// patterns to the LCD.
//    16-bit color, 128 wide by 160 high LCD
// Daniel Valvano
// September 1, 2014

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected 
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

/*======================================================================================
 Student: Alex Gallant
 Teacher: James Vardy
 Holland College
 Jan 29, 2016
 Microcontroller: TM4C123GH6PM or TM4C123 family
========================================================================================
This program does the following:

--  Objective:
--  This lab teaches you how to interface the ST7735 LCD to the TM4C123Microcontroller. Already
--  developed libraries and functions will be used to control what is displayed on the LCD

Prepare by connecting the board safely. Then prepare the
code to perform the following actions.
Create a new project that will display the following:
Name: Your Name
Date: Due Date (or date handed in whichever comes first)
Course Code:
Course Name:
Lab: description of the lab
The system will then wait until there is a button press which will cause the display to 
change to the Holland college symbol.

========================================================================================
**minor adjustment**
I had to shrink the Holland College symbol in order to have the program be smaller than 32k
========================================================================================
*/

#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"
#include "PortF.h"
#include "PortE.h"
#include "PortD.h"
#include "SysTick.h"  // why not here but in PortE??

void EnableInterrupts(void);
void DisableInterrupts(void);
void WaitForInterrupt(void);
void GPIOPortF_Handler(void);
void GPIOPortE_Handler(void);
	


/*
	States are defined below as the magnetic states of the coils, they will 
	maintain the same orientation for simplicity
	
	If the Motor's internal structure is imagined, there are two pairs of coils
	oriented in perpendicular direction.
	
	The Naming sturcture is as follows: The first character represents the vertical
	magnetic coils, the second character represents the horizontal magnetic coils.		
	
	The angles of the magnetic filedls are listed with their definitions.

	only change state if there is 1 button pressed... if there is none, or both
  function will maintain current state.

	main function gives specific action to single step or continuous buttons.
*/	

	uint32_t delay = 0x01; //1ms
	//uint32_t State = 0; 
	
#define GPIO_PORTD_OUT         			(*((volatile uint32_t *)0x4000703C)) // bits 1-0


struct State {
  uint32_t Step;            						// output designated to control the stepper motor
  //uint32_t Time;            					// this is the number of 10ms delays that will occur between actions
																				// this number will be gradually decreased to increase speed.
  const struct State *Next[4];};				// depends on 3-bit input

typedef const struct State STyp;
	
#define NN  	&FSM[0]  //NN 45  degrees,
#define NS 		&FSM[1]  //NS 135 degrees,
#define SS 		&FSM[2]  //SS 225 degrees,
#define SN  	&FSM[3]  //SN 315 degrees,


STyp FSM[4]={
 {0xA, {NS, SN}}, //0
 {0x9, {SS, NN}}, //1
 {0x5, {SN, NS}}, //2
 {0x6, {NN, SS}}, //3
};


//========================================================================================
// Main Function
int main(void){
	STyp *Pt;  														// state pointer
	PLL_Init();                  					// configure for 50 MHz clock
	SysTick_Init();              					// initialize SysTick timer
	Output_Init();
	DisableInterrupts();
	PortF_Init();
	PortE_Init();
	PortD_Init();
	int cursY = 0;
	int cursX = 0;
  printf("Alex Gallant\nEvan Younker\nFeb 2016\nELTR-2320\nReal Time Processing\n& DSPs\nLab2: Controlling a\nstepper motor;\nsingle and\ncontinuous stepping)\n");
  Pt = NN;  														// initial state: Green north; Red east; NOwalk
	ST7735_SetTextColor(0x0000);
	EnableInterrupts();
	while(1){
		WaitForInterrupt();
		if (update == 1){
			Pt = Pt->Next[NextState]; 							// transition to next state
			GPIO_PORTD_OUT = Pt->Step;
			update = 0;
			EnableInterrupts();
		}
		ST7735_SetCursor(cursX, cursY);
		cursX ++;
		cursX ++;
		cursX ++;
		if (cursX > 20){
			cursX = 0;
			cursY ++;
		}	//end if
		if (cursY > 15){
			cursY = 0;
			ST7735_FillScreen(0xFFFF);
		}	//end if
		ST7735_OutUDec(Pt->Step);
  }
} 
