/*************************************************************

*** @File        : UART_LOG.c
*** @Author      : Tori Lin
*** @Date        : 15/6/2018
*** @description :This file provide trace solution for User.
                  User can use uart connector with PC by using USB to TTL connector table.
 @verbatim 
 ===============================================================================
                      ##### How to use this drive #####
 ===============================================================================  
			1.connect with PC
				 	TTL 			              STM32F429 discovery
			-------------                 ----------------
			|           |                 |              |
			|  Tx(green)|---------------->|  Rx(PA10)    |					 
			|  Rx(white)|<----------------|  Tx(PA9)     |					 
			| GND(black)|<--------------->|   GND        |					 
			|           |                 |              |					 			 
			-------------                 ----------------
			
			2.initial UART_LOG in main() and printf LOG by using printf() ;
			example:
			void main(void)
			{
				// initial UART_LOG
				LOG_init();
				
				//prinf LOG
				printf("Hello world\r\n");
				
				while(1);
			}
			
			3.Startup AccessPort on PC  (download URL: http://www.sudt.com/en/ap/download.htm )
					a.select comport 
					b.select BuadRate to 115200
					c.select Parity to NONE
					d.select wordlength to 8b
					e.select stopbit to 1b
					
					example: COM7(115200,N,8,1)
					
			4.close comport to receive LOG
*************************************************************/


/************************  include  *************************/
#include "UART_LOG.h"

#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>




//driver usart
#include "stm32f4xx_usart.h"

/************************  define  **************************/
//GPIO define
/*
**PIN_GROUP      : GPIOx                 x can be A, B ... I for STM32F429 device
**CLK_GPIO_GROUP : RCC_AHB1Periph_GPIOx  x can be A, B ... I for STM32F429 device
**USART_TX       : GPIO_Pin_x            x can be 0~15 for STM32F429 device
**USART_RX       : GPIO_Pin_x            x can be 0~15 for STM32F429 device
*/
#define PIN_GROUP       GPIOA
#define CLK_GPIO_GROUP  RCC_AHB1Periph_GPIOA
#define USART_TX_PIN    GPIO_Pin_9
#define USART_RX_PIN    GPIO_Pin_10
#define USART_TX_SRC    GPIO_PinSource9
#define USART_RX_SRC    GPIO_PinSource10

//UART define
#define LOG_USART         USART1
#define CLK_USART         RCC_APB2Periph_USART1
#define USART_BR          115200
#define USART_WORDLENGTH  USART_WordLength_8b
#define USART_STOPBIT     USART_StopBits_1
#define USART_PARITY      USART_Parity_No


/***********************  Variable  ************************/

FILE __stdout;
FILE __stdin;

/************************  prototype  ***********************/
void LOG_PIN_init(void);
void LOG_USART_init(void);

/************************  function  ************************/

//init function
void LOG_init(void)
{
    LOG_PIN_init();
    LOG_USART_init();
}

void LOG_PIN_init(void)
{
	GPIO_InitTypeDef GPIO_config;
	
	//UART's GPIOA RCC
	RCC_AHB1PeriphClockCmd(CLK_GPIO_GROUP,ENABLE);
    
    
	//pin function init
	GPIO_config.GPIO_Pin = USART_TX_PIN;
	GPIO_config.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_config.GPIO_Mode = GPIO_Mode_AF ;
	GPIO_config.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_config.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(PIN_GROUP,&GPIO_config);
	GPIO_PinAFConfig(PIN_GROUP, USART_TX_SRC,  GPIO_AF_USART1); //usart1_TX
	
	
	GPIO_config.GPIO_Pin = USART_RX_PIN;
	GPIO_config.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_config.GPIO_Mode = GPIO_Mode_AF ;
	GPIO_config.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(PIN_GROUP,&GPIO_config);
	GPIO_PinAFConfig(PIN_GROUP, USART_RX_SRC,  GPIO_AF_USART1); //usart1_RX
}
void LOG_USART_init(void)
{
    RCC_APB2PeriphClockCmd(CLK_USART, ENABLE);
    
    //usart init
	USART_InitTypeDef usart_config;
	usart_config.USART_BaudRate    = USART_BR;
	usart_config.USART_WordLength  = USART_WORDLENGTH;
	usart_config.USART_StopBits    = USART_STOPBIT;
	usart_config.USART_Parity      = USART_PARITY;
	usart_config.USART_Mode        = USART_Mode_Rx | USART_Mode_Tx;
	usart_config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(LOG_USART,&usart_config);
	
	USART_Cmd(LOG_USART, ENABLE);
	
}
//end of init function

//USART Read and Write function
uint8_t USART_Read(void)
{
    while(!(USART_GetFlagStatus(LOG_USART,USART_FLAG_RXNE)));
    
    return USART_ReceiveData(LOG_USART);
}

void USART_Write(uint8_t *ch)
{
     while(!(USART_GetFlagStatus(LOG_USART,USART_FLAG_TXE)));
     
     USART_SendData(LOG_USART, *ch);
     
}
//end of USART Read and Write function


//retarget printf() and scanf() to LOG function
int fputc(int ch, FILE *f)
{
    uint8_t c;
    c = ch & 0x00ff;
    USART_Write(&c);
    return(ch);
    
}

int fgetc(FILE *f)
{
    uint8_t rxByte;
    rxByte = USART_Read();
    return rxByte ;
}
//end of retarget printf() and scanf() to LOG function


