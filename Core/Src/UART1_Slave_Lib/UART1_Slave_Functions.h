
/*
 * UART_Communication_Functions.c
 *
 *  Created on: Apr 19, 2019
 *      Author: krishna
 */
#ifndef __UART1_Slave_Functions_H
#define __UART1_Slave_Functions_H
#ifdef __cplusplus
 extern "C" {
#endif




#include "usart.h"
#include "UART1_Slave_Variables.h"
#include "UART1_Slave_Functions.h"

 void UART1_Store_Received_Data_In_Variables(void);

 void UART1_Update_Variables_if_Mesage_is_S(void);

 void UART1_Update_Variables_if_Mesage_is_C(void);

 void UART1_Update_Variables_if_Mesage_is_M(void);

 void UART1_Update_Variables_if_Mesage_is_R(void);

 unsigned char UART1_crcFast(unsigned char message[],unsigned int nbyte);

 void UART1_Use_Received_Data_S(void);

 void UART1_Use_Received_Data_C(void);

 void UART1_Use_Received_Data_M(void);

 void UART1_Use_Received_Data_R(void);

 void UART1_Receive_ISR_Program(void);

 void UART1_Send_Function_Inside_Timer_Interrupt(void); ///put this code in timer interrupt which is called every 200uS or so

 void UART1_Request_Send_Message(void);

 void UART1_Populate_Txdata_Array_With_Message_s(void);

 void UART1_Populate_Txdata_Array_With_Message_c(void);

 void UART1_Populate_Txdata_Array_With_Message_m(void);

 void UART1_Populate_Txdata_Array_With_Message_r(void);

 void UART1_Slave_Variables_Init(void);

 void UART1_Recieved_Settings_Error_Check(void);

#ifdef __cplusplus
}
#endif
#endif //__UART1_Slave_Functions_H
