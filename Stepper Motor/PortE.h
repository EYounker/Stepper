#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"
//#include "SysTick.h"

extern volatile uint8_t update;
extern volatile uint32_t NextState;

void PortE_Init(void);

void GPIOPortE_Handler(void);
