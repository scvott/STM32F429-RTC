/*************************************************************

*** @File        : UART_LOG.h
*** @Author      : Tori Lin
*** @Date        : 15/6/2018
*** @description :This file provide trace solution for User.
                 Use usart connector with PC.

*************************************************************/

/************************  include  *************************/
#include <stdint.h>

/************************  struct  **************************/
struct __FILE {
    int dummy;
};


/************************  function  ************************/
void LOG_init(void);
