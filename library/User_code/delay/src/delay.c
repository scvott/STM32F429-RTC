/* include */
#include "delay.h"
#include "core_cm4.h"
/* define */
#define STSTICK_MS_DIVISION  180000

/* global variable*/
static volatile int TimingDelay = 0;


/* function */
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    if(TimingDelay!=0)
        TimingDelay--;
}

void delay_init(void)
{
    SysTick_Config(STSTICK_MS_DIVISION);
}
void delay_ms(uint16_t ms)
{
	TimingDelay = ms;
    
    
    while(TimingDelay !=0);
}
