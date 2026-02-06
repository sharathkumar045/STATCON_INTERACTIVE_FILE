#ifndef __UART1_Slave_Variables_H
#define __UART1_Slave_Variables_H
#ifdef __cplusplus
 extern "C" {
#endif


   // variables related to UART1_Rec_Transmission
 extern __IO unsigned char UART1_recvdata[200];
  extern const uint8_t UART1_crcTable[256];
 
  struct U1_Rec_Flags{
	    __IO unsigned Not_Used1 : 1;
	    __IO unsigned Not_Used2 : 1;
	    __IO unsigned Not_Used3 : 1;
	    __IO unsigned Not_Used4 : 1;
	    __IO unsigned Not_Used5 : 1;
	    __IO unsigned Not_Used6 : 1;
	    __IO unsigned Not_Used7 : 1;
	    __IO unsigned Not_Used8 : 1;
	    __IO unsigned Not_Used9 : 1;
	    __IO unsigned Not_Used10 : 1;
	    __IO unsigned Not_Used11 : 1;
	    __IO unsigned Not_Used12 : 1;
	    __IO unsigned T_Parameters_Copied : 1;
	    __IO unsigned Mode_Error: 1;
	    __IO unsigned recv_check: 1;
	    __IO unsigned busy_flag : 1;
  };

  extern struct U1_Rec_Flags UART1_Rec_Flags;

  struct  U1_Rec_Var {
     __IO unsigned char recdata; //note that that this diferrent from recvdata[] array
     __IO uint8_t U1_CRC;
     __IO uint32_t Array_Index;
     __IO uint32_t Not_Used4;
     __IO uint32_t Not_Used5;
     __IO uint32_t Not_Used6;
     __IO uint32_t Not_Used7;
     __IO uint32_t Not_Used8;
     __IO uint32_t Not_Used9;
     __IO uint32_t Not_Used11;
     __IO uint32_t Not_Used12;
     __IO uint32_t Not_Used13;
     __IO uint32_t Not_Used14;
     __IO uint32_t Not_Used15;
     __IO uint32_t Not_Used16;
  };

  extern struct U1_Rec_Var UART1_Rec_Var;

   
#define UART1_Enable_Receive_Interrupt  USART2->CR1|=LL_USART_CR1_RXNEIE
#define UART1_Disable_Receive_Interrupt  USART2->CR1&=~LL_USART_CR1_RXNEIE
#define UART1_Enable  USART2->CR1|=USART_CR1_UE
#define UART1_Transmit_Not_Complete !(USART2->ISR&LL_USART_ISR_TC)
#define UART1_Clear_Receive_Flag USART1->RQR|=USART_RQR_RXFRQ
#define UART1_Rec_Data_Register USART2->RDR
#define UART1_Disable_Recv_INT USART2->CR1&=(~LL_USART_CR1_RXNEIE)



//UART1 variables related to message sending


 extern __IO unsigned char UART1_Txdata[90];

 struct U1_Send_Flags {
	  __IO unsigned Msg_Send_Fail : 1;
	  __IO unsigned Waiting_for_Ack : 1;
	  __IO unsigned Send_Enable : 1;
	  __IO unsigned Ackw_Byte_Expected : 1;
	  __IO unsigned Ackw_Data_Expected : 1;
	  __IO unsigned Not_Used6 : 1;
	  __IO unsigned Not_Used7 : 1;
	  __IO unsigned Not_Used8 : 1;
	  __IO unsigned Not_Used9 : 1;
	  __IO unsigned Not_Used10 : 1;
	  __IO unsigned Not_Used11 : 1;
	  __IO unsigned Not_Used12 : 1;
	  __IO unsigned Not_Used13 : 1;
	  __IO unsigned Not_Used14: 1;
	  __IO unsigned Not_Used15: 1;
	  __IO unsigned Not_Used16 : 1;
 };
 
 extern struct U1_Send_Flags UART1_Send_Flags;


 struct U1_Send_Var {
    __IO uint32_t Ack_Wait_Time_cntr;
    __IO uint8_t Ack_Fail_Num;
    __IO uint16_t Array_Index;
    __IO uint16_t Msg_Length;
    __IO uint16_t Arbitration_Val;
    __IO uint8_t Received_Ackw;
    __IO uint8_t Expected_Ackw_Byte;
    __IO uint8_t Count;
    __IO uint32_t Not_Used9;
    __IO uint32_t Not_Used10;
    __IO uint32_t Not_Used11;
    __IO uint32_t Not_Used12;
    __IO uint32_t Not_Used13;
    __IO uint32_t Not_Used14;
    __IO uint32_t Not_Used15;
    __IO uint32_t Not_Used16;
 };
  
 extern struct U1_Send_Var UART1_Send_Var;

   
#define UART1_Send_Ack_Wait_Time_Limit 30000 // these many counts of interrupt time length
#define UART1_Transmit_Data_register USART2->TDR
#define UART1_TRx_Complete (USART2->ISR&0x00000040)
#define UART1_Expected_Ack_for_msg_A '0'
#define UART1_Send_Number_of_Msg_types 1 //if A ,B, C and X are the message types possible then this will be 4

     
 struct U1_Msg_Send_Reqt_Flags {
	    __IO unsigned Msg_A : 1;
	    __IO unsigned Msg_B : 1;
	    __IO unsigned Msg_C : 1;
	    __IO unsigned Msg_D : 1;
	    __IO unsigned Msg_E : 1;
	    __IO unsigned Msg_F : 1;
	    __IO unsigned Msg_G : 1;
	    __IO unsigned Msg_H : 1;
	    __IO unsigned Msg_I : 1;
	    __IO unsigned Msg_J : 1;
	    __IO unsigned Msg_K : 1;
	    __IO unsigned Msg_L : 1;
	    __IO unsigned Msg_M : 1;
	    __IO unsigned Msg_N: 1;
	    __IO unsigned Msg_O: 1;
	    __IO unsigned Msg_P : 1;

	    __IO unsigned Msg_s : 1;  // message for settings
	    __IO unsigned Msg_c: 1;  // message for calibration
	    __IO unsigned Msg_m: 1; // message for metering
	    __IO unsigned Msg_r: 1;   // message for reset
  } ;
     
  extern struct U1_Msg_Send_Reqt_Flags UART1_Msg_Send_Reqt_Flags;

     
    // variables which can be transfered to user variables.c
  // extern __IO uint8_t D_Mode;
  // extern __IO uint16_t D_M1S1_activation_State;
  // extern __IO uint16_t D_M1S1Steps;
  // extern __IO uint16_t D_M1S1Speed;
  // extern __IO uint16_t D_M1S1Direction;

      
#ifdef __cplusplus
}
#endif
#endif //__UART1_Slave_Functions_H
