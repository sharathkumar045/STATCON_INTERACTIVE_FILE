
/*
 * UART_Communication_Functions.c
 *
 *  Created on: Apr 19, 2019
 *      Author: krishna
 */

#include "usart.h"
#include "UART_Debugging_Variables.h"
#include "UART_Debugging_Functions.h"
#include "New_Struct_Variables.h"
#include "definations.h"



// header files with variable declarations

void UART_Debugging_Send_Function_Inside_1ms_Timer_Interrupt(void) ///put this code in timer interrupt which is called every 200uS or so
{
   if((UART_Debugging_Flags.Send_Enable==1)&&(UART_Debugging_TRx_Complete))
    {
      if(UART_Debugging_Var.Array_Index<UART_Debugging_Var.Msg_Length)
         {
           UART_Debugging_Transmit_Data_register=UART_Debugging_Txdata[UART_Debugging_Var.Array_Index];
           UART_Debugging_Var.Array_Index= UART_Debugging_Var.Array_Index+1;
         }
      else
      {
    	  UART_Debugging_Var.Nxt_Msg_Delay_Counter = UART_Debugging_Var.Nxt_Msg_Delay_Counter + 1;
    	  if(UART_Debugging_Var.Nxt_Msg_Delay_Counter >= 1000) //1sec counter
    	  {
  			UART_Debugging_Flags.Send_Enable = 0;
  			UART_Debugging_Var.Array_Index = 0;
  			UART_Debugging_Var.Nxt_Msg_Delay_Counter = 0;
  			UART_Debugging_Var.Packet_Number = UART_Debugging_Var.Packet_Number +1; //for loading the new packet
//  			UART_Debugging_Var.Packet_Number = 0;
    	  }
      }
    }   
}


void UART_Debugging_Populate_Txdata_Array_With_Message_Debug_Data(void)
{
	if((Inv_Cntrl.Prev_State == 1)&&(Inv_Cntrl.State == 0))
	{
		UART_Debugging_Var.Data_Acq_Stop = 1;
	}

	if(UART_Debugging_Flags.Send_Enable == 0)
	{
		switch(UART_Debugging_Var.Packet_Number)
		{
		case 0:
			UART_Debugging_Txdata[0] = 0; // sending case index numbers to identify the packets
			UART_Debugging_Txdata[1] = 0; // sending case index numbers to identify the packets

			UART_Debugging_Var.Temp_Var = (int16_t)Earth_Fault_ADC;

			break;

//		case 1:
//			UART_Debugging_Txdata[0] = 1; // sending case index numbers to identify the packets
//			UART_Debugging_Txdata[1] = 0; // sending case index numbers to identify the packets
//
//			UART_Debugging_Var.Temp_Var = (int16_t)Norm_ADC.I_Inv_Pri_AC;
//			break;
//
//		case 2:
//			UART_Debugging_Txdata[0] = 2; // sending case index numbers to identify the packets
//			UART_Debugging_Txdata[1] = 0; // sending case index numbers to identify the packets
//
//			UART_Debugging_Var.Temp_Var = (int16_t)Inv_Cntrl.I_Ref;
//			break;

//		case 3:
//			UART_Debugging_Txdata[0] = 3; // sending case index numbers to identify the packets
//			UART_Debugging_Txdata[1] = 0; // sending case index numbers to identify the packets
//
//			UART_Debugging_Var.Temp_Var = (uint16_t)Norm_ADC.I_Inv_Pri_AC;
//			break;
//
//		case 4:
//			UART_Debugging_Txdata[0] = 4; // sending case index numbers to identify the packets
//			UART_Debugging_Txdata[1] = 0; // sending case index numbers to identify the packets
//
//			UART_Debugging_Var.Temp_Var = (int16_t)Inv_Cntrl.I_Ref;
//			break;
//
//		case 5:
//			UART_Debugging_Txdata[0] = 5; // sending case index numbers to identify the packets
//			UART_Debugging_Txdata[1] = 0; // sending case index numbers to identify the packets
//
//			UART_Debugging_Var.Temp_Var = (int16_t)Inv_Cntrl.V_Batt_ref;
//			break;
//
//		case 6:
//			UART_Debugging_Txdata[0] = 6; // sending case index numbers to identify the packets
//			UART_Debugging_Txdata[1] = 0; // sending case index numbers to identify the packets
//
//			UART_Debugging_Var.Temp_Var = (int16_t)Norm_ADC.V_Mains_AC;
//
//		case 7:
//			UART_Debugging_Txdata[0] = 7; // sending case index numbers to identify the packets
//			UART_Debugging_Txdata[1] = 0; // sending case index numbers to identify the packets
//
//			UART_Debugging_Var.Temp_Var = (int16_t)Inv_Cntrl.I_Error_Inv;
//			break;

		default: UART_Debugging_Var.Packet_Number = 0;
			break;
		}

		UART_Debugging_Txdata[UART_Debugging_Var.Array_Index]    = 0x00FF & UART_Debugging_Var.Temp_Var;
		UART_Debugging_Txdata[UART_Debugging_Var.Array_Index + 1]= (0xFF00 & UART_Debugging_Var.Temp_Var)>>8;

		UART_Debugging_Var.Array_Index = UART_Debugging_Var.Array_Index + 2;

		if(UART_Debugging_Var.Array_Index >= UART_Debugging_Var.Msg_Length)
		{
			UART_Debugging_Flags.Send_Enable = 1;
			UART_Debugging_Var.Array_Index = 0;
		}

//		if(UART_Debugging_Var.Data_Acq_Stop == 0)
//		{
//			UART_Debugging_Var.Temp_Var = (int16_t)(Inv_Cntrl.I_Ref);
//
//			UART_Debugging_Txdata[UART_Debugging_Var.Array_Index]    = 0x00FF & UART_Debugging_Var.Temp_Var;
//			UART_Debugging_Txdata[UART_Debugging_Var.Array_Index + 1]= (0xFF00 & UART_Debugging_Var.Temp_Var)>>8;
//
//			UART_Debugging_Var.Temp_Var = (int16_t)Inv_Cntrl.I_Ref;
//
//			UART_Debugging_Txdata[UART_Debugging_Var.Array_Index + 2]    = 0x00FF & UART_Debugging_Var.Temp_Var;
//			UART_Debugging_Txdata[UART_Debugging_Var.Array_Index + 3]= (0xFF00 & UART_Debugging_Var.Temp_Var)>>8;
//
//			UART_Debugging_Var.Temp_Var = (int16_t)Inv_Cntrl.I_Ref;
//
//			UART_Debugging_Txdata[UART_Debugging_Var.Array_Index + 4]    = 0x00FF & UART_Debugging_Var.Temp_Var;
//			UART_Debugging_Txdata[UART_Debugging_Var.Array_Index + 5]= (0xFF00 & UART_Debugging_Var.Temp_Var)>>8;
//
//			UART_Debugging_Var.Temp_Var = (int16_t)Inv_Cntrl.I_Ref;
//
//			UART_Debugging_Txdata[UART_Debugging_Var.Array_Index + 6]    = 0x00FF & UART_Debugging_Var.Temp_Var;
//			UART_Debugging_Txdata[UART_Debugging_Var.Array_Index + 7]= (0xFF00 & UART_Debugging_Var.Temp_Var)>>8;
//
//			UART_Debugging_Var.Array_Index = UART_Debugging_Var.Array_Index + 2;
//		}
//		else if((UART_Debugging_Var.Data_Acq_Stop == 1)&&(UART_Debugging_Var.Array_Index != 0))
//		{
//			UART_Debugging_Var.Temp_Var = (int16_t)0;
//
//			UART_Debugging_Txdata[UART_Debugging_Var.Array_Index]    = 0x00FF & UART_Debugging_Var.Temp_Var;
//			UART_Debugging_Txdata[UART_Debugging_Var.Array_Index + 1]= (0xFF00 & UART_Debugging_Var.Temp_Var)>>8;
//
//			UART_Debugging_Var.Array_Index = UART_Debugging_Var.Array_Index + 2;
//		}
//
//		if((UART_Debugging_Var.Array_Index >= UART_Debugging_Var.Msg_Length))
//		{
//			UART_Debugging_Flags.Send_Enable = 1;
//			UART_Debugging_Var.Array_Index = 0;
//		}
//		//below condition is added to continuously send previously stored array when latch is 1
//		if((UART_Debugging_Var.Data_Acq_Stop == 1)&&(UART_Debugging_Var.Array_Index == 0))
//		{
//			UART_Debugging_Flags.Send_Enable = 1;
//			UART_Debugging_Var.Array_Index = 0;
//		}
	}
}

void UART_Debugging_Slave_Variables_Init(void)
{
    UART_Debugging_Rec_Flags.Not_Used1 = 0;
    UART_Debugging_Rec_Flags.Not_Used2 = 0;
    UART_Debugging_Rec_Flags.Not_Used3 = 0;
    UART_Debugging_Rec_Flags.Not_Used4 = 0;
    UART_Debugging_Rec_Flags.Not_Used5 = 0;
    UART_Debugging_Rec_Flags.Not_Used6 = 0;
    UART_Debugging_Rec_Flags.Not_Used7 = 0;
    UART_Debugging_Rec_Flags.Not_Used8 = 0;
    UART_Debugging_Rec_Flags.Not_Used9 = 0;
    UART_Debugging_Rec_Flags.Not_Used10 = 0;
    UART_Debugging_Rec_Flags.Not_Used11 = 0;
    UART_Debugging_Rec_Flags.Not_Used12 = 0;
    UART_Debugging_Rec_Flags.T_Parameters_Copied = 0;
    UART_Debugging_Rec_Flags.Mode_Error= 0;
    UART_Debugging_Rec_Flags.recv_check= 0;
    UART_Debugging_Rec_Flags.busy_flag = 0;


    UART_Debugging_Rec_Var.recdata = 0;
    UART_Debugging_Rec_Var.U1_CRC = 0;
    UART_Debugging_Rec_Var.Array_Index = 0;
    UART_Debugging_Rec_Var.Not_Used4 = 0;
    UART_Debugging_Rec_Var.Not_Used5 = 0;
    UART_Debugging_Rec_Var.Not_Used6 = 0;
    UART_Debugging_Rec_Var.Not_Used7 = 0;
    UART_Debugging_Rec_Var.Not_Used8 = 0;
    UART_Debugging_Rec_Var.Not_Used9 = 0;
    UART_Debugging_Rec_Var.Not_Used11 = 0;
    UART_Debugging_Rec_Var.Not_Used12 = 0;
    UART_Debugging_Rec_Var.Not_Used13 = 0;
    UART_Debugging_Rec_Var.Not_Used14 = 0;
    UART_Debugging_Rec_Var.Not_Used15 = 0;
    UART_Debugging_Rec_Var.Not_Used16 = 0;

    UART_Debugging_Flags.Msg_Send_Fail = 0;
    UART_Debugging_Flags.Waiting_for_Ack = 0;
    UART_Debugging_Flags.Send_Enable = 0;
    UART_Debugging_Flags.Ackw_Byte_Expected = 0;
    UART_Debugging_Flags.Ackw_Data_Expected = 0;


    UART_Debugging_Var.Ack_Wait_Time_cntr = 0;
    UART_Debugging_Var.Ack_Fail_Num = 0;
    UART_Debugging_Var.Array_Index = 0;
    UART_Debugging_Var.Msg_Length = 0;
    UART_Debugging_Var.Arbitration_Val = 0;
    UART_Debugging_Var.Received_Ackw = 0;
    UART_Debugging_Var.Expected_Ackw_Byte = 0;
    UART_Debugging_Var.Count = 0;
	UART_Debugging_Var.Msg_Length = 400;
	UART_Debugging_Var.Packet_Number = 0;
	UART_Debugging_Var.Data_Acq_Stop = 0;



    UART_Debugging_Msg_Send_Reqt_Flags.Msg_A = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_B = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_C = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_D = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_E = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_F = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_G = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_H = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_I = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_J = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_K = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_L = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_M = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_N = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_O = 0;
    UART_Debugging_Msg_Send_Reqt_Flags.Msg_P = 0;


}
