
/*
 * UART_Communication_Functions.c
 *
 *  Created on: Apr 19, 2019
 *      Author: krishna
 */

#include "usart.h"
#include "UART1_Slave_Variables.h"
#include "UART1_Slave_Functions.h"

#include "User_Defined_Functions.h"
#include "definations.h"
#include "New_Struct_Variables.h"



// header files with variable declarations


void UART1_Store_Received_Data_In_Variables(void)
{
	UART1_Update_Variables_if_Mesage_is_S();//add functions for various messages
	UART1_Update_Variables_if_Mesage_is_C();
	UART1_Update_Variables_if_Mesage_is_M();
	UART1_Update_Variables_if_Mesage_is_R();
}

void UART1_Update_Variables_if_Mesage_is_S(void)
{
   if(UART1_recvdata[0]=='S' && UART1_Rec_Var.Array_Index==49)//for user set
	 {
		UART1_Rec_Flags.busy_flag=1;
		UART1_Rec_Flags.recv_check=0;
		UART1_Rec_Var.U1_CRC = UART1_crcFast((UART1_recvdata),49);
		UART1_recvdata[0]=0;
		if(UART1_Rec_Var.U1_CRC == UART1_recvdata[49])
		{
//			UART1_Use_Received_Data_S();
//			if(Othr.Recieved_Settings_Ok == 1)
//			{
				UART1_Msg_Send_Reqt_Flags.Msg_s = 1;//send ack
//			}
		}
		UART1_Rec_Flags.busy_flag=0;
	  }
}

void UART1_Update_Variables_if_Mesage_is_C(void)
{
   if(UART1_recvdata[0]=='C' && UART1_Rec_Var.Array_Index==21)//for user set
	 {
		 UART1_Rec_Flags.busy_flag=1;
		 UART1_Rec_Flags.recv_check=0;
		 UART1_Rec_Var.U1_CRC = UART1_crcFast((UART1_recvdata),21);
		 UART1_recvdata[0]=0;
		 if(UART1_Rec_Var.U1_CRC == UART1_recvdata[21])
			 {
				UART1_Use_Received_Data_C();
				UART1_Msg_Send_Reqt_Flags.Msg_c=1;//send ack
			 }
		  UART1_Rec_Flags.busy_flag=0;
	  }
}

void UART1_Update_Variables_if_Mesage_is_M(void)
{
   if(UART1_recvdata[0]=='M' && UART1_Rec_Var.Array_Index==1)//for user set
	 {
		 UART1_Rec_Flags.busy_flag=1;
		 UART1_Rec_Flags.recv_check=0;
		 UART1_Rec_Var.U1_CRC = UART1_crcFast((UART1_recvdata),1);
		 UART1_recvdata[0]=0;
		 if(UART1_Rec_Var.U1_CRC == UART1_recvdata[1])
			 {
//			UART1_Use_Received_Data_M();
			UART1_Msg_Send_Reqt_Flags.Msg_m=1;//send ack
			 }
		  UART1_Rec_Flags.busy_flag=0;
	  }
}

void UART1_Update_Variables_if_Mesage_is_R(void)
{
   if(UART1_recvdata[0]=='R' && UART1_Rec_Var.Array_Index==5)//for user set
	 {
		 UART1_Rec_Flags.busy_flag=1;
		 UART1_Rec_Flags.recv_check=0;
		 UART1_Rec_Var.U1_CRC = UART1_crcFast((UART1_recvdata),5);
		 UART1_recvdata[0]=0;
		 if(UART1_Rec_Var.U1_CRC == UART1_recvdata[5])
			 {
			UART1_Use_Received_Data_R();
			UART1_Msg_Send_Reqt_Flags.Msg_r=1;//send ack
			 }
		  UART1_Rec_Flags.busy_flag=0;
	  }
}

unsigned char UART1_crcFast(unsigned char message[],unsigned int nbyte)
{
   unsigned char  data,remainder =0;
   unsigned int  byte;

    for (byte = 0; byte < nbyte; ++byte)
    {
       data =(message[byte]) ^ (remainder);
       remainder = UART1_crcTable[data] ^ (message[byte] << 8);

    }
     return (remainder);
}

/*

void UART1_SendMsg(void)
{
   unsigned char size;
   UART1_TXPtr=&UART1_Txdata[0];
   size=sizeof(UART1_Txdata);
   while(size)
   {
      while (UART1_Transmit_Not_Complete);
      USART1->TDR = *UART1_TXPtr++;
      size--;
   }
}
*/

void UART1_Use_Received_Data_S(void)
{
	Othr.Recieved_Settings_Error = 0;//error is reset and below program sets it

	UART1_Recieved_Settings_Error_Check();//This program sets received setting error bit and resets received settings ok bit

	//Othr.Recieved_Settings_Ok = 1; // Only for testing later on we have to remove

	if(Othr.Recieved_Settings_Error == 0)
	{
		Norm_Avg_Limits.V_Batt_UL_H_Cut_Off  	= Temp_Settings.V_Batt_UL_H_Cut_Off;
		Norm_Avg_Limits.V_Batt_UL_L_Cut_In 		= Temp_Settings.V_Batt_UL_L_Cut_In;
		Norm_Avg_Limits.V_Batt_LL_H_Cut_In 		= Temp_Settings.V_Batt_LL_H_Cut_In;
		Norm_Avg_Limits.V_Batt_LL_L_Cut_Off 	= Temp_Settings.V_Batt_LL_L_Cut_Off;
		/////
		Norm_RMS_Limits.I_Inv_Pri_AC_100_Percent = Temp_Settings.I_Inv_Pri_AC_100_Percent;
		Norm_RMS_Limits.I_Inv_Pri_AC_110_Percent = Temp_Settings.I_Inv_Pri_AC_100_Percent + (Temp_Settings.I_Inv_Pri_AC_100_Percent>>3);
		Norm_RMS_Limits.I_Inv_Pri_AC_125_Percent = Temp_Settings.I_Inv_Pri_AC_100_Percent + (Temp_Settings.I_Inv_Pri_AC_100_Percent>>2);
		Norm_RMS_Limits.I_Inv_Pri_AC_150_Percent = Temp_Settings.I_Inv_Pri_AC_100_Percent + (Temp_Settings.I_Inv_Pri_AC_100_Percent>>1);

		Norm_ADC_Limits.I_Inv_Pri_AC_UL 		= (Temp_Settings.I_Inv_Pri_AC_100_Percent<<1) + (Temp_Settings.I_Inv_Pri_AC_100_Percent>>1); //X2.5 times RMS
		Norm_ADC_Limits.I_Inv_Pri_AC_UL			= (Norm_ADC_Limits.I_Inv_Pri_AC_UL)+ (Norm_ADC_Limits.I_Inv_Pri_AC_UL>>1); //peak is approx 1.5 *RMS
		if(Norm_ADC_Limits.I_Inv_Pri_AC_UL>3900)
		{
			Norm_ADC_Limits.I_Inv_Pri_AC_UL=3900;
		}

		Norm_ADC_Limits.I_Inv_Pri_AC_LL			=-(Norm_ADC_Limits.I_Inv_Pri_AC_UL);

		Othr.Relay_OFF_Delay_Time=Temp_Settings.Relay_OFF_Delay_Time; //multiples of 104us
		if(Temp_Settings.Relay_ON_Delay_Time < 192)
		{
			Othr.Relay_Activation_Delay = 192-Temp_Settings.Relay_ON_Delay_Time;
		}
		else
		{
			Othr.Relay_Activation_Delay = 0;
		}

		Inv_Cntrl.Inv_Export_Ipeak_limit 		= Temp_Settings.Inv_Ipeak_limit;
		Inv_Cntrl.Inv_Import_Ipeak_limit 		= Temp_Settings.Inv_Import_Ipeak_limit;
		Norm_Avg_Limits.V_PV_UL_H_Cut_Off 		= Temp_Settings.V_PV_UL_H_Cut_Off;
		Norm_Avg_Limits.V_PV_UL_L_Cut_In 		= Temp_Settings.V_PV_UL_L_Cut_In;
		Norm_Avg_Limits.V_PV_LL_H_Cut_In 		= Temp_Settings.V_PV_LL_H_Cut_In;
		Norm_Avg_Limits.V_PV_LL_L_Cut_Off 		= Temp_Settings.V_PV_LL_L_Cut_Off;
		Norm_Avg_Limits.I_PV_UL 				= Temp_Settings.I_PV_UL;
		Othr.PV_Power_Limit						= Temp_Settings.PV_Power_Limit;
		Norm_RMS_Limits.V_Mains_AC_UL_H_Cut_Off = Temp_Settings.V_Mains_AC_UL_H_Cut_Off;
		Norm_RMS_Limits.V_Mains_AC_UL_L_Cut_In 	= Temp_Settings.V_Mains_AC_UL_L_Cut_In;
		Norm_RMS_Limits.V_Mains_AC_LL_H_Cut_In 	= Temp_Settings.V_Mains_AC_LL_H_Cut_In;
		Norm_RMS_Limits.V_Mains_AC_LL_L_Cut_Off = Temp_Settings.V_Mains_AC_LL_L_Cut_Off;

		Inv_Cntrl.V_Mains_Failure_Band_UL 		= (Norm_RMS_Limits.V_Mains_AC_LL_L_Cut_Off*3)>>2;  //V_Mains_LL_Cut_Off*1.414*(1/2)
		Inv_Cntrl.V_Mains_Failure_Band_LL 		= -(Inv_Cntrl.V_Mains_Failure_Band_UL); //V_Mains_LL_Cut_Off*1.414*(1/2)

		Othr.Recieved_Settings_Ok 		= 1;//
		Othr.Recieved_Settings_Error 	= 1; //Set error intentionally so that this loop is not entered again

		// All temperorary variables are initialized to zero so that again and again this part of code doesn't run
		Temp_Settings.I_Inv_Pri_AC_100_Percent 	= 0;
		Temp_Settings.Relay_OFF_Delay_Time 		= 0;
		Temp_Settings.Relay_ON_Delay_Time 		= 0;

		Temp_Settings.I_PV_UL 					= 0;
		Temp_Settings.Inv_Ipeak_limit 			= 0;
		Temp_Settings.Inv_Import_Ipeak_limit 	= 0;
		Temp_Settings.Inv_Volt_Set 				= 0;
		Temp_Settings.V_Batt_LL_H_Cut_In 		= 0;
		Temp_Settings.V_Batt_LL_L_Cut_Off 		= 0;
		Temp_Settings.V_Batt_UL_H_Cut_Off 		= 0;
		Temp_Settings.V_Batt_UL_L_Cut_In 		= 0;

		Temp_Settings.V_Mains_AC_LL_H_Cut_In 	= 0;
		Temp_Settings.V_Mains_AC_LL_L_Cut_Off 	= 0;
		Temp_Settings.V_Mains_AC_UL_H_Cut_Off 	= 0;
		Temp_Settings.V_Mains_AC_UL_L_Cut_In 	= 0;
		Temp_Settings.V_PV_LL_H_Cut_In 			= 0;
		Temp_Settings.V_PV_LL_L_Cut_Off 		= 0;
		Temp_Settings.V_PV_UL_H_Cut_Off 		= 0;
		Temp_Settings.V_PV_UL_L_Cut_In 			= 0;
		Temp_Settings.Batt_Equalization_Reqst 	= 0;
		Temp_Settings.PV_Power_Limit 			= 0;

		}
}

void UART1_Recieved_Settings_Error_Check(void)
{
	__IO uint8_t i;
	i=1;
	Temp_Settings.Inv_Volt_Set 			= (uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=3;
	Temp_Settings.V_Batt_UL_H_Cut_Off 	=(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=5;
	Temp_Settings.V_Batt_UL_L_Cut_In 	=(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=7;
	Temp_Settings.V_Batt_LL_H_Cut_In 	=(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=9;
	Temp_Settings.V_Batt_LL_L_Cut_Off 	=(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=11;
	Temp_Settings.I_Inv_Pri_AC_100_Percent =(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=13;
	Temp_Settings.Relay_OFF_Delay_Time 	=(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=15;
	Temp_Settings.Relay_ON_Delay_Time 	=(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=17;
	Temp_Settings.Inv_Ipeak_limit 		=(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=19;
	Temp_Settings.Inv_Import_Ipeak_limit =(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=21;
	Temp_Settings.V_PV_UL_H_Cut_Off 	=(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=23;
	Temp_Settings.V_PV_UL_L_Cut_In 		=(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=25;
	Temp_Settings.V_PV_LL_H_Cut_In 		=(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=27;
	Temp_Settings.V_PV_LL_L_Cut_Off 	=(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=29;
	Temp_Settings.I_PV_UL 				=(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=31;
	Temp_Settings.V_Mains_AC_UL_H_Cut_Off =(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=33;
	Temp_Settings.V_Mains_AC_UL_L_Cut_In =(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=35;
	Temp_Settings.V_Mains_AC_LL_H_Cut_In =(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=37;
	Temp_Settings.V_Mains_AC_LL_L_Cut_Off =(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

	i=39;
	Temp_Settings.PV_Power_Limit =(uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);


	if((Temp_Settings.V_Batt_UL_H_Cut_Off<4096)&&(Temp_Settings.V_Batt_UL_H_Cut_Off>=Temp_Settings.V_Batt_UL_L_Cut_In))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.V_Batt_UL_L_Cut_In<Temp_Settings.V_Batt_UL_H_Cut_Off)&&(Temp_Settings.V_Batt_UL_L_Cut_In>=Temp_Settings.V_Batt_LL_H_Cut_In))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.V_Batt_LL_H_Cut_In<Temp_Settings.V_Batt_UL_L_Cut_In)&&(Temp_Settings.V_Batt_LL_H_Cut_In>=Temp_Settings.V_Batt_LL_L_Cut_Off))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.V_Batt_LL_L_Cut_Off<=Temp_Settings.V_Batt_LL_H_Cut_In)&&(Temp_Settings.V_Batt_LL_L_Cut_Off>0))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.I_Inv_Pri_AC_100_Percent<=1500)&&(Temp_Settings.I_Inv_Pri_AC_100_Percent>0))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.Relay_OFF_Delay_Time<576)&&(Temp_Settings.Relay_OFF_Delay_Time>0))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.Relay_ON_Delay_Time<576)&&(Temp_Settings.Relay_ON_Delay_Time>0))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.Inv_Ipeak_limit<Norm_ADC_Limits.I_Inv_Pri_AC_UL)&&(Temp_Settings.Inv_Ipeak_limit>0))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}
	if((Temp_Settings.Inv_Import_Ipeak_limit<Norm_ADC_Limits.I_Inv_Pri_AC_UL)&&(Temp_Settings.Inv_Import_Ipeak_limit>0))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}


	if((Temp_Settings.V_PV_UL_H_Cut_Off<4096)&&(Temp_Settings.V_PV_UL_H_Cut_Off>=Temp_Settings.V_PV_UL_L_Cut_In))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.V_PV_UL_L_Cut_In<Temp_Settings.V_PV_UL_H_Cut_Off)&&(Temp_Settings.V_PV_UL_L_Cut_In>=Temp_Settings.V_PV_LL_H_Cut_In))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.V_PV_LL_H_Cut_In<Temp_Settings.V_PV_UL_L_Cut_In)&&(Temp_Settings.V_PV_LL_H_Cut_In>=Temp_Settings.V_PV_LL_L_Cut_Off))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.V_PV_LL_L_Cut_Off<=Temp_Settings.V_PV_LL_H_Cut_In)&&(Temp_Settings.V_PV_LL_L_Cut_Off>0))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.I_PV_UL<=Norm_ADC_Limits.I_PV_UL))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.V_Mains_AC_UL_H_Cut_Off<4096)&&(Temp_Settings.V_Mains_AC_UL_H_Cut_Off>=Temp_Settings.V_Mains_AC_UL_L_Cut_In))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.V_Mains_AC_UL_L_Cut_In<Temp_Settings.V_Mains_AC_UL_H_Cut_Off)&&(Temp_Settings.V_Mains_AC_UL_L_Cut_In>=Temp_Settings.V_Mains_AC_LL_H_Cut_In))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.V_Mains_AC_LL_H_Cut_In<Temp_Settings.V_Mains_AC_UL_L_Cut_In)&&(Temp_Settings.V_Mains_AC_LL_H_Cut_In>=Temp_Settings.V_Mains_AC_LL_L_Cut_Off))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.V_Mains_AC_LL_L_Cut_Off<=Temp_Settings.V_Mains_AC_LL_H_Cut_In)&&(Temp_Settings.V_Mains_AC_LL_L_Cut_Off>0))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.PV_Power_Limit<=3600)&&(Temp_Settings.PV_Power_Limit>=0))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

}

void UART1_Use_Received_Data_C(void)
{

	__IO uint8_t i;

	Calibration_Temp.Error = 0;

	i = 1;
	Calibration_Temp.V_Inv = (uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);
	if(Calibration_Temp.V_Inv<1638)
	{
		Calibration_Temp.V_Inv = 1638;
		Calibration_Temp.Error = 1;
	}
	else if(Calibration_Temp.V_Inv>2457)
	{
		Calibration_Temp.V_Inv = 2457;
		Calibration_Temp.Error = 1;
	}

	i = 3;
	Calibration_Temp.V_Batt = (uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);
	if(Calibration_Temp.V_Batt<1638)
	{
		Calibration_Temp.V_Batt = 1638;
		Calibration_Temp.Error = 1;
	}
	else if(Calibration_Temp.V_Batt>2457)
	{
		Calibration_Temp.V_Batt = 2457;
		Calibration_Temp.Error = 1;
	}

	i = 5;
	Calibration_Temp.I_Inv = (uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);
	if(Calibration_Temp.I_Inv<1638)
	{
		Calibration_Temp.I_Inv = 1638;
		Calibration_Temp.Error = 1;
	}
	else if(Calibration_Temp.I_Inv>2457)
	{
		Calibration_Temp.I_Inv = 2457;
		Calibration_Temp.Error = 1;
	}

	i = 7;
	Calibration_Temp.I_Batt = (uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);
	if(Calibration_Temp.I_Batt<1638)
	{
		Calibration_Temp.I_Batt = 1638;
		Calibration_Temp.Error = 1;
	}
	else if(Calibration_Temp.I_Batt>2457)
	{
		Calibration_Temp.I_Batt = 2457;
		Calibration_Temp.Error = 1;
	}

	i = 9;
	Calibration_Temp.V_PV =  (uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);

//  uint16_t temp =  (uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);
//    if(temp!=0)
//    {
//    	Calibration_Temp.V_PV = temp;
//    }else
//    {
//    	Calibration_Temp.V_PV  = Calibration_Temp.V_PV;
//    }

	if(Calibration_Temp.V_PV<1638)
	{
		Calibration_Temp.V_PV = 1638;
		Calibration_Temp.Error = 1;
	}
	else if(Calibration_Temp.V_PV>2457)
	{
		Calibration_Temp.V_PV = 2457;
		Calibration_Temp.Error = 1;
	}

	i = 11;
	Calibration_Temp.I_PV = (uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);
	if(Calibration_Temp.I_PV<1638)
	{
		Calibration_Temp.I_PV = 1638;
		Calibration_Temp.Error = 1;
	}
	else if(Calibration_Temp.I_PV>2457)
	{
		Calibration_Temp.I_PV = 2457;
		Calibration_Temp.Error = 1;
	}

	i = 13;
	Calibration_Temp.V_Mains = (uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);
	if(Calibration_Temp.V_Mains<1638)
	{
		Calibration_Temp.V_Mains = 1638;
		Calibration_Temp.Error = 1;
	}
	else if(Calibration_Temp.V_Mains>2457)
	{
		Calibration_Temp.V_Mains = 2457;
		Calibration_Temp.Error = 1;
	}

	i = 15;
	Calibration_Temp.I_Mains_CT = (uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);
	if(Calibration_Temp.I_Mains_CT<1638)
	{
		Calibration_Temp.I_Mains_CT = 1638;
		Calibration_Temp.Error = 1;
	}
	else if(Calibration_Temp.I_Mains_CT>2457)
	{
		Calibration_Temp.I_Mains_CT = 2457;
		Calibration_Temp.Error = 1;
	}

	i = 17;
	Calibration_Temp.I_Inv_CT = (uint16_t)(UART1_recvdata[i])|((uint16_t)(UART1_recvdata[i+1])<<8);
	if(Calibration_Temp.I_Inv_CT<1638)
	{
		Calibration_Temp.I_Inv_CT = 1638;
		Calibration_Temp.Error = 1;
	}
	else if(Calibration_Temp.I_Inv_CT>2457)
	{
		Calibration_Temp.I_Inv_CT = 2457;
		Calibration_Temp.Error = 1;
	}


//	Calibration_Var = Calibration_Temp;
	if((Calibration_Temp.Error==0)&&(Switch_Is_ON))
	if((Calibration_Temp.Error==0))
	{
		Calibration_Var = Calibration_Temp;
		Calibration_Temp.OK = 1;
		Calibration_Var.OK = 1;
	}
	else
	{
		Calibration_Temp.OK = 0;
		Calibration_Var.OK = 0;
		Calibration_Temp.Error=1;
	}

}

void UART1_Use_Received_Data_M(void)
{

}

void UART1_Use_Received_Data_R(void)
{
	Metering.Export_Energy=0;
	Metering.Export_Energy_CT = 0;
	Metering.Export_KWH =0;
	Metering.Export_KWH_CT = 0;
	Metering.Import_Energy=0;
	Metering.Import_Energy_CT = 0;
	Metering.Import_KWH =0;
	Metering.Import_KWH_CT = 0;

	Metering.PV_KWH  = 0;
	Metering.PV_KWH_Sum  = 0;

	Metering.Batt_AH_Out=0;
	Metering.Batt_Ampere_Out_Sum = 0;

	Metering.Batt_AH_In = 0;
	Metering.Batt_Ampere_In_Sum = 0;

    UART1_recvdata[0]=0;
    UART1_recvdata[1]=0;
    UART1_recvdata[2]=0;
    UART1_recvdata[3]=0;
    UART1_recvdata[4]=0;
}

void UART1_Receive_ISR_Program(void)
{
   UART1_Rec_Var.recdata= UART1_Rec_Data_Register;

      if(UART1_Rec_Flags.busy_flag==0)
      {
         if(UART1_Rec_Var.recdata=='S' && UART1_Rec_Flags.recv_check==0)//for factory set if data received as T
         {
            UART1_Rec_Flags.recv_check=1;
            UART1_Rec_Var.Array_Index=0;
            UART1_recvdata[UART1_Rec_Var.Array_Index]=UART1_Rec_Var.recdata; //SAVE RECEIVE DATA IN FACTORY SET ARRAY
         }
         else if((UART1_Rec_Flags.recv_check==1) && UART1_Rec_Var.Array_Index<49 && UART1_recvdata[0]=='S')
         {
            UART1_Rec_Var.Array_Index++;
            UART1_recvdata[UART1_Rec_Var.Array_Index]=UART1_Rec_Var.recdata;
         }

         ///////////////////////////////////////////////////////////////////////////////////////////////
         else if(UART1_Rec_Var.recdata=='C' && UART1_Rec_Flags.recv_check==0)//for factory set if data received as T
         {
            UART1_Rec_Flags.recv_check=1;
            UART1_Rec_Var.Array_Index=0;
            UART1_recvdata[UART1_Rec_Var.Array_Index]=UART1_Rec_Var.recdata; //SAVE RECEIVE DATA IN FACTORY SET ARRAY
         }
         else if((UART1_Rec_Flags.recv_check==1) && UART1_Rec_Var.Array_Index<22 && UART1_recvdata[0]=='C')
         {
            UART1_Rec_Var.Array_Index++;
            UART1_recvdata[UART1_Rec_Var.Array_Index]=UART1_Rec_Var.recdata;
         }

         ///////////////////////////////////////////////////////////////////////////////////////////////

         else if(UART1_Rec_Var.recdata=='M' && UART1_Rec_Flags.recv_check==0)//for factory set if data received as T
         {
            UART1_Rec_Flags.recv_check=1;
            UART1_Rec_Var.Array_Index=0;
            UART1_recvdata[UART1_Rec_Var.Array_Index]=UART1_Rec_Var.recdata; //SAVE RECEIVE DATA IN FACTORY SET ARRAY
         }
         else if((UART1_Rec_Flags.recv_check==1) && UART1_Rec_Var.Array_Index<1 && UART1_recvdata[0]=='M')
         {
            UART1_Rec_Var.Array_Index++;
            UART1_recvdata[UART1_Rec_Var.Array_Index]=UART1_Rec_Var.recdata;
         }

         ///////////////////////////////////////////////////////////////////////////////////////////////


         else if(UART1_Rec_Var.recdata=='R' && UART1_Rec_Flags.recv_check==0)//for factory set if data received as T
         {
            UART1_Rec_Flags.recv_check=1;
            UART1_Rec_Var.Array_Index=0;
            UART1_recvdata[UART1_Rec_Var.Array_Index]=UART1_Rec_Var.recdata; //SAVE RECEIVE DATA IN FACTORY SET ARRAY
         }
         else if((UART1_Rec_Flags.recv_check==1) && UART1_Rec_Var.Array_Index<5 && UART1_recvdata[0]=='R')
         {
            UART1_Rec_Var.Array_Index++;
            UART1_recvdata[UART1_Rec_Var.Array_Index]=UART1_Rec_Var.recdata;
         }
      }
      UART1_Clear_Receive_Flag;
      USART1->ICR|=USART_ICR_ORECF;
    }

void UART1_Send_Function_Inside_Timer_Interrupt(void) ///put this code in timer interrupt which is called every 200uS or so
{
   if((UART1_Send_Flags.Send_Enable==1)&&(UART1_TRx_Complete))
    {
      if(UART1_Send_Var.Array_Index<UART1_Send_Var.Msg_Length)
         {
           UART1_Transmit_Data_register=UART1_Txdata[UART1_Send_Var.Array_Index];
           UART1_Send_Var.Array_Index= UART1_Send_Var.Array_Index+1;
         }
      else
         {    
           UART1_Send_Flags.Send_Enable=0;//now one can populate ned data into transmit register
         }
    }   
 }

void UART1_Request_Send_Message(void)
   {
     switch (UART1_Send_Var.Arbitration_Val)
       {
         case 1:

            if((UART1_Send_Flags.Send_Enable!=1))
             {
               UART1_Send_Var.Arbitration_Val=UART1_Send_Var.Arbitration_Val+1; //
                //arbitrate only if not busy with sending. if a send request is pending then 
                //it will send and go to next arbitration level in next loop cycle
                //this code must be at the beginning of this case, please do not change the location of this if statement.
             }
            if((UART1_Send_Flags.Send_Enable!=1)&&(UART1_Msg_Send_Reqt_Flags.Msg_s==1))
              {               
                  UART1_Populate_Txdata_Array_With_Message_s();
                  UART1_Send_Flags.Send_Enable=1;
              }
         break;
         
         
         case 2:

            if((UART1_Send_Flags.Send_Enable!=1))
             {
               UART1_Send_Var.Arbitration_Val=UART1_Send_Var.Arbitration_Val+1; //
                //arbitrate only if not busy with sending. if a send request is pending then 
                //it will send and go to next arbitration level in next loop cycle.
                //else it will go the next arbitration case in the next loop cycle without sending anything 
                //this code must be at the beginning of this case, please do not change the location of this if statement.
             }
            if((UART1_Send_Flags.Send_Enable!=1)&&(UART1_Msg_Send_Reqt_Flags.Msg_c==1))
              {
                  UART1_Populate_Txdata_Array_With_Message_c();
                  UART1_Send_Flags.Send_Enable=1;
              }
         break;

         case 3:

            if((UART1_Send_Flags.Send_Enable!=1))
             {
               UART1_Send_Var.Arbitration_Val=UART1_Send_Var.Arbitration_Val+1; //
                //arbitrate only if not busy with sending. if a send request is pending then
                //it will send and go to next arbitration level in next loop cycle.
                //else it will go the next arbitration case in the next loop cycle without sending anything
                //this code must be at the beginning of this case, please do not change the location of this if statement.
             }
            if((UART1_Send_Flags.Send_Enable!=1)&&(UART1_Msg_Send_Reqt_Flags.Msg_m==1))
              {
                  UART1_Populate_Txdata_Array_With_Message_m();
                  UART1_Send_Flags.Send_Enable=1;
              }
         break;

         case 4:

            if((UART1_Send_Flags.Send_Enable!=1))
             {
               UART1_Send_Var.Arbitration_Val=UART1_Send_Var.Arbitration_Val+1; //
                //arbitrate only if not busy with sending. if a send request is pending then
                //it will send and go to next arbitration level in next loop cycle.
                //else it will go the next arbitration case in the next loop cycle without sending anything
                //this code must be at the beginning of this case, please do not change the location of this if statement.
             }
            if((UART1_Send_Flags.Send_Enable!=1)&&(UART1_Msg_Send_Reqt_Flags.Msg_r==1))
              {               
                  UART1_Populate_Txdata_Array_With_Message_r();
                  UART1_Send_Flags.Send_Enable=1;
              }
         break;
         

         default:
                UART1_Send_Var.Arbitration_Val=1;
         break;
       }
  }

void UART1_Populate_Txdata_Array_With_Message_s(void)
{
//if(Othr.Recieved_Settings_Ok == 1)
	//{
		UART1_Txdata[0]				= 's';
		UART1_Send_Var.Msg_Length	= 2;//(0)for message and no CRC (total 1)
		UART1_Txdata[1] = UART1_crcFast((UART1_Txdata),(UART1_Send_Var.Msg_Length-1));

		UART1_Msg_Send_Reqt_Flags.Msg_s = 0;
		UART1_Send_Var.Array_Index 		= 0;
		UART1_Send_Flags.Send_Enable	= 1;
	//}
}

void UART1_Populate_Txdata_Array_With_Message_c(void)

   {
		UART1_Txdata[0]='c';

		UART1_Txdata[1]=(0x00FF & Calibration_Var.V_Inv);
		UART1_Txdata[2]=((0xFF00 & Calibration_Var.V_Inv)>>8);

		UART1_Txdata[3]=(0x00FF & Calibration_Var.V_Batt);
		UART1_Txdata[4]=((0xFF00 & Calibration_Var.V_Batt)>>8);

		UART1_Txdata[5]=(0x00FF & Calibration_Var.I_Inv);
		UART1_Txdata[6]=((0xFF00 & Calibration_Var.I_Inv)>>8);

		UART1_Txdata[7]=(0x00FF & Calibration_Var.I_Batt);
		UART1_Txdata[8]=((0xFF00 & Calibration_Var.I_Batt)>>8);

		UART1_Txdata[9]=(0x00FF & Calibration_Var.V_PV);
		UART1_Txdata[10]=((0xFF00 & Calibration_Var.V_PV)>>8);

		UART1_Txdata[11]=(0x00FF & Calibration_Var.I_PV);
		UART1_Txdata[12]=((0xFF00 & Calibration_Var.I_PV)>>8);

		UART1_Txdata[13]=(0x00FF & Calibration_Var.V_Mains);
		UART1_Txdata[14]=((0xFF00 & Calibration_Var.V_Mains)>>8);

		UART1_Txdata[15]=(0x00FF & Calibration_Var.I_Mains_CT);
		UART1_Txdata[16]=((0xFF00 & Calibration_Var.I_Mains_CT)>>8);

		UART1_Txdata[17]=(0x00FF & Calibration_Var.I_Inv_CT);
		UART1_Txdata[18]=((0xFF00 & Calibration_Var.I_Inv_CT)>>8);


		UART1_Send_Var.Msg_Length=22;//(0-26)for message and 27th location for CRC (total 28)
		UART1_Txdata[21] = UART1_crcFast((UART1_Txdata),(UART1_Send_Var.Msg_Length-1));

		UART1_Msg_Send_Reqt_Flags.Msg_c=0;
		UART1_Send_Var.Array_Index=0;
		UART1_Send_Flags.Send_Enable=1;

//		Calibration_Var.Copy_To_Flash_Request = 1;
//		Copy_Calib_To_Flash();

   }

void UART1_Populate_Txdata_Array_With_Message_m(void)
{
		int32_t X_UART1 = 0;

        UART1_Txdata[0] = 'm';

        UART1_Txdata[1] = (0x00FF & Norm_Avg.V_Batt);
        UART1_Txdata[2] = ((0xFF00 & Norm_Avg.V_Batt)>>8);

//        Norm_Avg.V_PV = 2550;
//        Norm_Avg.I_PV = 1990;
        UART1_Txdata[3] = (0x00FF & Norm_Avg.V_PV);
        UART1_Txdata[4] = ((0xFF00 & Norm_Avg.V_PV)>>8);
// I PV
        UART1_Txdata[5] = (0x00FF & Norm_Avg.I_PV);
        UART1_Txdata[6] = ((0xFF00 & Norm_Avg.I_PV)>>8);

        //PV_KW
        X_UART1 		= (Norm_Avg.I_PV * Norm_Avg.V_PV)>>12;   //change made
        UART1_Txdata[7] = (0x00FF & X_UART1);
        UART1_Txdata[8] = ((0xFF00 & X_UART1)>>8);

        //PV_KWH
        UART1_Txdata[9]	=(0x00FF & Metering.PV_KWH);
        UART1_Txdata[10]=((0xFF00 & Metering.PV_KWH)>>8);
// V mains
        UART1_Txdata[11]=(0x00FF & Norm_Rms.V_Mains);
        UART1_Txdata[12]=((0xFF00 & Norm_Rms.V_Mains)>>8);
// I inv
        UART1_Txdata[13]=(0x00FF & Norm_Rms.I_Inv_Pri);
        UART1_Txdata[14]=((0xFF00 & Norm_Rms.I_Inv_Pri)>>8);

        UART1_Txdata[15]=(0x00FF & Norm_Rms.I_Mains_CT);
        UART1_Txdata[16]=((0xFF00 & Norm_Rms.I_Mains_CT)>>8);
// F Grid
        UART1_Txdata[17]=(0x00FF & Inv_Cntrl.Time_Period_Grid);
        UART1_Txdata[18]=((0xFF00 & Inv_Cntrl.Time_Period_Grid)>>8);

//Grid KVA
        if(Norm_ADC.Mains_Power_Mean >= 0)
		{
		  X_UART1 		= (Norm_ADC.Mains_Power_Mean)>>12;
		  X_UART1		= (X_UART1*Calibration_Var.I_Mains_CT)>>11;
		  X_UART1		= (X_UART1*Calibration_Var.V_Mains)>>11;
		}
		else
		{
		  X_UART1 		= (-Norm_ADC.Mains_Power_Mean)>>12;
		  X_UART1		= (X_UART1*Calibration_Var.I_Mains_CT)>>11;
		  X_UART1		= (X_UART1*Calibration_Var.V_Mains)>>11;
		}

		UART1_Txdata[19]=(0x00FF & X_UART1);
		UART1_Txdata[20]=((0xFF00 & X_UART1)>>8);

        //////////////
        X_UART1			= Metering.Export_KWH;
        X_UART1			=(X_UART1*Calibration_Var.I_Inv)>>11;//i inv primary cal
        X_UART1			=(X_UART1*Calibration_Var.V_Mains)>>11;

        UART1_Txdata[21]=(0x00FF & X_UART1);
        UART1_Txdata[22]=((0xFF00 & X_UART1)>>8);

        //////////////
		X_UART1			= Metering.Import_KWH;
		X_UART1			=(X_UART1*Calibration_Var.I_Inv)>>11; //i inv primary cal
		X_UART1			=(X_UART1*Calibration_Var.V_Mains)>>11;

		UART1_Txdata[23]=(0x00FF & X_UART1); // to normalize it we are right shifting
		UART1_Txdata[24]=((0xFF00 & X_UART1)>>8);

//		UART1_Txdata[25]=(0x00FF & X_UART1); // to normalize it we are right shifting
//		UART1_Txdata[26]=((0xFF00 & X_UART1)>>8);

		/////////////
        Generate_Fault_Data_For_Metering();

        UART1_Txdata[27]=(0x00FF & Othr.Fault_Reg_1);
        UART1_Txdata[28]=((0xFF00 & Othr.Fault_Reg_1)>>8);

        /*
        Othr.Fault_Reg_2 =  ((uint16_t)Avg_Limit_Flags.V_Inv<<9)|((uint16_t)Avg_Limit_Flags.V_PV<<8)|((uint16_t)Avg_Limit_Flags.V_Batt<<7)|((uint16_t)Avg_Limit_Flags.Inv_Over_Temp<<6)|((uint16_t)Avg_Limit_Flags.I_PV<<5)|((uint16_t)Avg_Limit_Flags.I_Batt_Discharg<<4)|((uint16_t)Avg_Limit_Flags.I_Batt_Charg<<3)|((uint16_t)Avg_Limit_Flags.Batt_Over_Temp<<2)|((uint16_t)RMS_Limit_Flags.V_Mains<<1)|((uint16_t)RMS_Limit_Flags.I_Inv);
		UART1_Txdata[25]=(0x00FF & Othr.Fault_Reg_2);
        UART1_Txdata[26]=((0xFF00 & Othr.Fault_Reg_2)>>8);
        */
////only for temporaty debuging
        UART1_Txdata[29]=(0x00FF & Othr.Fault_Reg_2);
        UART1_Txdata[30]=((0xFF00 & Othr.Fault_Reg_2)>>8);

        /////////////
		UART1_Send_Var.Msg_Length=44;//(0-26)for message and 27th location for CRC (total 28)
		UART1_Txdata[43] = UART1_crcFast((UART1_Txdata),(UART1_Send_Var.Msg_Length-1));

		UART1_Msg_Send_Reqt_Flags.Msg_m	= 0;
		UART1_Send_Var.Array_Index		= 0;
		UART1_Send_Flags.Send_Enable	= 1;
   }

void UART1_Populate_Txdata_Array_With_Message_r(void)

   {
        UART1_Txdata[0]='r';
        UART1_Send_Var.Msg_Length=1;//(0)for message and no CRC (total 1)

        UART1_Msg_Send_Reqt_Flags.Msg_r=0;
        UART1_Send_Var.Array_Index=0;
        UART1_Send_Flags.Send_Enable=1;
   }

void UART1_Slave_Variables_Init(void)
{
    UART1_Rec_Flags.Not_Used1 = 0;
    UART1_Rec_Flags.Not_Used2 = 0;
    UART1_Rec_Flags.Not_Used3 = 0;
    UART1_Rec_Flags.Not_Used4 = 0;
    UART1_Rec_Flags.Not_Used5 = 0;
    UART1_Rec_Flags.Not_Used6 = 0;
    UART1_Rec_Flags.Not_Used7 = 0;
    UART1_Rec_Flags.Not_Used8 = 0;
    UART1_Rec_Flags.Not_Used9 = 0;
    UART1_Rec_Flags.Not_Used10 = 0;
    UART1_Rec_Flags.Not_Used11 = 0;
    UART1_Rec_Flags.Not_Used12 = 0;
    UART1_Rec_Flags.T_Parameters_Copied = 0;
    UART1_Rec_Flags.Mode_Error= 0;
    UART1_Rec_Flags.recv_check= 0;
    UART1_Rec_Flags.busy_flag = 0;


    UART1_Rec_Var.recdata = 0;
    UART1_Rec_Var.U1_CRC = 0;
    UART1_Rec_Var.Array_Index = 0;
    UART1_Rec_Var.Not_Used4 = 0;
    UART1_Rec_Var.Not_Used5 = 0;
    UART1_Rec_Var.Not_Used6 = 0;
    UART1_Rec_Var.Not_Used7 = 0;
    UART1_Rec_Var.Not_Used8 = 0;
    UART1_Rec_Var.Not_Used9 = 0;
    UART1_Rec_Var.Not_Used11 = 0;
    UART1_Rec_Var.Not_Used12 = 0;
    UART1_Rec_Var.Not_Used13 = 0;
    UART1_Rec_Var.Not_Used14 = 0;
    UART1_Rec_Var.Not_Used15 = 0;
    UART1_Rec_Var.Not_Used16 = 0;

    UART1_Send_Flags.Msg_Send_Fail = 0;
    UART1_Send_Flags.Waiting_for_Ack = 0;
    UART1_Send_Flags.Send_Enable = 0;
    UART1_Send_Flags.Ackw_Byte_Expected = 0;
    UART1_Send_Flags.Ackw_Data_Expected = 0;
    UART1_Send_Flags.Not_Used6 = 0;
    UART1_Send_Flags.Not_Used7 = 0;
    UART1_Send_Flags.Not_Used8 = 0;
    UART1_Send_Flags.Not_Used9 = 0;
    UART1_Send_Flags.Not_Used10 = 0;
    UART1_Send_Flags.Not_Used11 = 0;
    UART1_Send_Flags.Not_Used12 = 0;
    UART1_Send_Flags.Not_Used13 = 0;
    UART1_Send_Flags.Not_Used14 = 0;
    UART1_Send_Flags.Not_Used15 = 0;
    UART1_Send_Flags.Not_Used16 = 0;

    UART1_Send_Var.Ack_Wait_Time_cntr = 0;
    UART1_Send_Var.Ack_Fail_Num = 0;
    UART1_Send_Var.Array_Index = 0;
    UART1_Send_Var.Msg_Length = 0;
    UART1_Send_Var.Arbitration_Val = 0;
    UART1_Send_Var.Received_Ackw = 0;
    UART1_Send_Var.Expected_Ackw_Byte = 0;
    UART1_Send_Var.Count = 0;
    UART1_Send_Var.Not_Used9 = 0;
    UART1_Send_Var.Not_Used10 = 0;
    UART1_Send_Var.Not_Used11 = 0;
    UART1_Send_Var.Not_Used12 = 0;
    UART1_Send_Var.Not_Used13 = 0;
    UART1_Send_Var.Not_Used14 = 0;
    UART1_Send_Var.Not_Used15 = 0;
    UART1_Send_Var.Not_Used16 = 0;


    UART1_Msg_Send_Reqt_Flags.Msg_A = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_B = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_C = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_D = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_E = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_F = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_G = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_H = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_I = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_J = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_K = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_L = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_M = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_N = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_O = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_P = 0;

    UART1_Msg_Send_Reqt_Flags.Msg_c = 0;
    UART1_Msg_Send_Reqt_Flags.Msg_m = 0;
	UART1_Msg_Send_Reqt_Flags.Msg_r = 0;
	UART1_Msg_Send_Reqt_Flags.Msg_s = 0;

}
