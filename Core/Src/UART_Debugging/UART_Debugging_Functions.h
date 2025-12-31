
/*
 * UART_Communication_Functions.c
 *
 *  Created on: Apr 19, 2019
 *      Author: krishna
 */
#ifndef __UART_Debugging_Slave_Functions_H
#define __UART_Debugging_Slave_Functions_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "usart.h"
#include "UART_Debugging_Variables.h"
#include "UART_Debugging_Functions.h"

void UART_Debugging_Send_Function_Inside_1ms_Timer_Interrupt(void); ///put this code in timer interrupt which is called every 200uS or so

void UART_Debugging_Populate_Txdata_Array_With_Message_Debug_Data(void);

void UART_Debugging_Slave_Variables_Init(void);

#ifdef __cplusplus
}
#endif
#endif //__UART_Debugging_Slave_Functions_H
