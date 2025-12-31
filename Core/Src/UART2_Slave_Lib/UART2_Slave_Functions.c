
/*
 * UART_Communication_Functions.c
 *
 *  Created on: Apr 19, 2019
 *      Author: krishna
 */

#include "usart.h"
#include "New_Struct_Variables.h"
#include "UART2_Slave_Variables.h"
#include "UART2_Slave_Functions.h"
#include "definations.h"
#include "User_Defined_Functions.h"

// header files with variable declarations


/*
void UART1_Send_Data_if_Requested(void)
{
    if(UART1_recdata=='K')
    {

       UART2_Txdata[0]=M1S1_activation_State;

       UART2_Txdata[1]=(0x00FF & M1S1Steps);
       UART2_Txdata[2]=((0xFF00 & M1S1Steps)>>8);

       UART2_Txdata[3]=0x00FF & M1S1Speed;
       UART2_Txdata[4]=((0xFF00 & M1S1Speed)>>8);

       UART2_Txdata[5]=(0x00FF & M1S1Direction);
       UART2_Txdata[6]=((0xFF00 & M1S1Direction)>>8);

       UART2_Txdata[7]=(0x00FF & M1S1Steps);
       UART2_Txdata[8]=((0xFF00 & M1S1Steps)>>8);

       UART2_Txdata[9]=(0x00FF & M1S1Steps);
       UART2_Txdata[10]=((0xFF00 & M1S1Steps)>>8);

       UART2_Txdata[11]=0x00FF & M1S1Steps;
       UART2_Txdata[12]=((0xFF00 & M1S1Steps)>>8);

       UART2_Txdata[13]=(0x00FF & M1S1Steps);
       UART2_Txdata[14]=((0xFF00 & M1S1Steps)>>8);

       UART2_Txdata[15]=(0x00FF & M1S1Steps);
       UART2_Txdata[16]=((0xFF00 & M1S1Steps)>>8);

       UART2_Txdata[17]=(0x00FF & M1S1Steps);
       UART2_Txdata[18]=((0xFF00 & M1S1Steps)>>8);

       UART2_Txdata[19]=(0x00FF & M1S1Steps);
       UART2_Txdata[20]=((0xFF00 & M1S1Steps)>>8);

       UART2_Txdata[21]=(0x00FF & M1S1Steps);
       UART2_Txdata[22]=((0xFF00 & M1S1Steps)>>8);

       UART2_Txdata[23]=(0x00FF & M1S1Steps);
       UART2_Txdata[24]=((0xFF00 & M1S1Steps)>>8);

       UART2_Txdata[25]=(0x00FF & M1S1Steps);
       UART2_Txdata[26]=((0xFF00 & M1S1Steps)>>8);


       while(UART1_Transmit_Not_Complete);
       USART1->TDR ='k';
       //U2TXREG ='k';
       UART1_CRC=UART2_crcFast((UART2_Txdata),sizeof(UART2_Txdata));
       UART1_SendMsg();
       while (UART1_Transmit_Not_Complete);
       USART1->TDR = UART1_CRC;
       UART1_recdata=0;
    }

}
*/

void UART2_Store_Received_Data_In_Variables(void)
{
	UART2_Update_Variables_if_Mesage_is_S();//add functions for various messages
	UART2_Update_Variables_if_Mesage_is_C();
	UART2_Update_Variables_if_Mesage_is_M();
	UART2_Update_Variables_if_Mesage_is_R();
}

void UART2_Update_Variables_if_Mesage_is_S(void)
{
   if(UART2_recvdata[0]=='S' && UART2_Rec_Var.Array_Index==89)//for user set
	 {
		UART2_Rec_Flags.busy_flag=1;
		UART2_Rec_Flags.recv_check=0;
		UART2_Rec_Var.U1_CRC = UART2_crcFast((UART2_recvdata),89);
		UART2_recvdata[0]=0;
		if(UART2_Rec_Var.U1_CRC == UART2_recvdata[89])
		{
			UART2_Use_Received_Data_S();
			if(Othr.Recieved_Settings_Ok == 1)
			{
				UART2_Msg_Send_Reqt_Flags.Msg_s=1;//send ack
			}
		}
		UART2_Rec_Flags.busy_flag=0;
	  }
}

void UART2_Update_Variables_if_Mesage_is_C(void)
{
   if(UART2_recvdata[0]=='C' && UART2_Rec_Var.Array_Index==31)//for user set
	 {
		 UART2_Rec_Flags.busy_flag=1;
		 UART2_Rec_Flags.recv_check=0;
		 UART2_Rec_Var.U1_CRC = UART2_crcFast((UART2_recvdata),31);
		 UART2_recvdata[0]=0;
		 if(UART2_Rec_Var.U1_CRC == UART2_recvdata[31])
			 {
				UART2_Use_Received_Data_C();
				UART2_Msg_Send_Reqt_Flags.Msg_c=1;//send ack
			 }
		  UART2_Rec_Flags.busy_flag=0;
	  }
}

void UART2_Update_Variables_if_Mesage_is_M(void)
{
   if(UART2_recvdata[0]=='M' && UART2_Rec_Var.Array_Index==1)//for user set
	 {
		 UART2_Rec_Flags.busy_flag=1;
		 UART2_Rec_Flags.recv_check=0;
		 UART2_Rec_Var.U1_CRC = UART2_crcFast((UART2_recvdata),1);
		 UART2_recvdata[0]=0;
		 if(UART2_Rec_Var.U1_CRC == UART2_recvdata[1])
			 {
			UART2_Use_Received_Data_M();
			UART2_Msg_Send_Reqt_Flags.Msg_m=1;//send ack
			 }
		  UART2_Rec_Flags.busy_flag=0;
	  }
}

void UART2_Update_Variables_if_Mesage_is_R(void)
{
   if(UART2_recvdata[0]=='R' && UART2_Rec_Var.Array_Index==5)//for user set
	 {
		 UART2_Rec_Flags.busy_flag=1;
		 UART2_Rec_Flags.recv_check=0;
		 UART2_Rec_Var.U1_CRC = UART2_crcFast((UART2_recvdata),5);
		 UART2_recvdata[0]=0;
		 if(UART2_Rec_Var.U1_CRC == UART2_recvdata[5])
			 {
			UART2_Use_Received_Data_R();
			UART2_Msg_Send_Reqt_Flags.Msg_r=1;//send ack
			 }
		  UART2_Rec_Flags.busy_flag=0;
	  }
}

unsigned char UART2_crcFast(unsigned char message[],unsigned int nbyte)
{
   unsigned char  data,remainder =0;
   unsigned int  byte;

    for (byte = 0; byte < nbyte; ++byte)
    {
       data =(message[byte]) ^ (remainder);
       remainder = UART2_crcTable[data] ^ (message[byte] << 8);

    }
     return (remainder);
}

/*

void UART1_SendMsg(void)
{
   unsigned char size;
   UART1_TXPtr=&UART2_Txdata[0];
   size=sizeof(UART2_Txdata);
   while(size)
   {
      while (UART1_Transmit_Not_Complete);
      USART1->TDR = *UART1_TXPtr++;
      size--;
   }
}
*/

void UART2_Use_Received_Data_S(void)
{
	Othr.Recieved_Settings_Error = 0;//error is reset and below program sets it

	UART2_Recieved_Settings_Error_Check();//This program sets received setting error bit and resets received settings ok bit

	//Othr.Recieved_Settings_Ok = 1; // Only for testing later on we have to remove

	if(Othr.Recieved_Settings_Error == 0)
	{
		PV_Cntrl.V_Batt_ref = Temp_Settings.V_Batt_ref;
		Othr.V_Batt_Boost_Ref=Temp_Settings.V_Batt_Boost_ref;
		Othr.V_Batt_Equaliz_Ref=Temp_Settings.V_Batt_Equaliz_ref;
		Othr.V_Batt_Float_Ref=Temp_Settings.V_Batt_Float_ref;
		Norm_Avg_Limits.V_Batt_UL_H_Cut_Off  = Temp_Settings.V_Batt_UL_H_Cut_Off;
		Norm_Avg_Limits.V_Batt_UL_L_Cut_In = Temp_Settings.V_Batt_UL_L_Cut_In;
		Norm_Avg_Limits.V_Batt_LL_H_Cut_In = Temp_Settings.V_Batt_LL_H_Cut_In;
		Norm_Avg_Limits.V_Batt_LL_L_Cut_Off = Temp_Settings.V_Batt_LL_L_Cut_Off;
		/////
		Othr.I_Batt_Chg_Boost_Ref=Temp_Settings.I_Batt_Charg_UL;
		Othr.I_Batt_Chg_Boost_Cutoff_ref=Othr.I_Batt_Chg_Boost_Ref>>3;
		Othr.I_Batt_Chg_Float_Ref=Temp_Settings.I_Batt_Charg_UL;
		Othr.I_Batt_Chg_Equaliz_Ref=Temp_Settings.I_Batt_Charg_UL>>2;//charging current limit in equalization mode
		Othr.Batt_Equalization_Reqst=Temp_Settings.Batt_Equalization_Reqst;
		/////
		Norm_RMS_Limits.I_Inv_Pri_AC_100_Percent = Temp_Settings.I_Inv_Pri_AC_100_Percent;
		Norm_RMS_Limits.I_Inv_Pri_AC_110_Percent = Temp_Settings.I_Inv_Pri_AC_100_Percent + (Temp_Settings.I_Inv_Pri_AC_100_Percent>>3);
		Norm_RMS_Limits.I_Inv_Pri_AC_125_Percent = Temp_Settings.I_Inv_Pri_AC_100_Percent + (Temp_Settings.I_Inv_Pri_AC_100_Percent>>2);
		Norm_RMS_Limits.I_Inv_Pri_AC_150_Percent = Temp_Settings.I_Inv_Pri_AC_100_Percent + (Temp_Settings.I_Inv_Pri_AC_100_Percent>>1);


		///////////////////
		Norm_ADC_Limits.I_Inv_Pri_AC_UL= (Temp_Settings.I_Inv_Pri_AC_100_Percent<<1) + (Temp_Settings.I_Inv_Pri_AC_100_Percent>>1); //X2.5 times RMS
		Norm_ADC_Limits.I_Inv_Pri_AC_UL= (Norm_ADC_Limits.I_Inv_Pri_AC_UL)+ (Norm_ADC_Limits.I_Inv_Pri_AC_UL>>1); //peak is approx 1.5 *RMS
		if(Norm_ADC_Limits.I_Inv_Pri_AC_UL>3900)
		{
			Norm_ADC_Limits.I_Inv_Pri_AC_UL=3900;
		}

		Norm_ADC_Limits.I_Inv_Pri_AC_LL=-(Norm_ADC_Limits.I_Inv_Pri_AC_UL);
         //////////////////

		Othr.Relay_OFF_Delay_Time=Temp_Settings.Relay_OFF_Delay_Time; //multiples of 104us
		if(Temp_Settings.Relay_ON_Delay_Time < 192)
		{
			Othr.Relay_Activation_Delay = 192-Temp_Settings.Relay_ON_Delay_Time;
		}
		else if((Temp_Settings.Relay_ON_Delay_Time>=192)&&(Temp_Settings.Relay_ON_Delay_Time<384))
		{
			Othr.Relay_Activation_Delay = 384-Temp_Settings.Relay_ON_Delay_Time;
		}
		else if((Temp_Settings.Relay_ON_Delay_Time>=384)&&(Temp_Settings.Relay_ON_Delay_Time<576))
		{
			Othr.Relay_Activation_Delay = 576-Temp_Settings.Relay_ON_Delay_Time;
		}
		else
		{
			Othr.Relay_Activation_Delay = 0;
		}


		////////////////////////////////////

		Inv_Cntrl.Inv_Export_Ipeak_limit = Temp_Settings.Inv_Ipeak_limit;
		Inv_Cntrl.Inv_Import_Ipeak_limit = Temp_Settings.Inv_Import_Ipeak_limit;
		Norm_Avg_Limits.V_PV_UL_H_Cut_Off = Temp_Settings.V_PV_UL_H_Cut_Off;
		Norm_Avg_Limits.V_PV_UL_L_Cut_In = Temp_Settings.V_PV_UL_L_Cut_In;
		Norm_Avg_Limits.V_PV_LL_H_Cut_In = Temp_Settings.V_PV_LL_H_Cut_In;
		Norm_Avg_Limits.V_PV_LL_L_Cut_Off = Temp_Settings.V_PV_LL_L_Cut_Off;
		Norm_Avg_Limits.I_PV_UL = Temp_Settings.I_PV_UL;
		Othr.PV_Power_Limit=Temp_Settings.PV_Power_Limit;
		Norm_RMS_Limits.V_Mains_AC_UL_H_Cut_Off = Temp_Settings.V_Mains_AC_UL_H_Cut_Off;
		Norm_RMS_Limits.V_Mains_AC_UL_L_Cut_In = Temp_Settings.V_Mains_AC_UL_L_Cut_In;
		Norm_RMS_Limits.V_Mains_AC_LL_H_Cut_In = Temp_Settings.V_Mains_AC_LL_H_Cut_In;
		Norm_RMS_Limits.V_Mains_AC_LL_L_Cut_Off =Temp_Settings.V_Mains_AC_LL_L_Cut_Off;

		Inv_Cntrl.V_Mains_Failure_Band_UL = (Norm_RMS_Limits.V_Mains_AC_LL_L_Cut_Off*3)>>2;  //V_Mains_LL_Cut_Off*1.414*(1/2)
		Inv_Cntrl.V_Mains_Failure_Band_LL = -(Inv_Cntrl.V_Mains_Failure_Band_UL); //V_Mains_LL_Cut_Off*1.414*(1/2)

		Inv_Cntrl.V_Grid_Boost_Ref = Temp_Settings.V_Grid_Boost_Ref;
		Inv_Cntrl.V_Grid_Float_Ref = Temp_Settings.V_Grid_Float_Ref;
		Inv_Cntrl.Inverter_Rating = Temp_Settings.Inverter_Rating;
		Othr.Batt_Boost_Mode_Timer_Minutes_Limit = Temp_Settings.Batt_Boost_Mode_Timer_Minutes_Limit;
		Othr.Batt_Grid_Boost_Mode_Timer_Minutes_Limit = Othr.Batt_Boost_Mode_Timer_Minutes_Limit;
		Othr.V_Mains_Offset_Calib_Rqst = Temp_Settings.V_Mains_Offset_Calib_Rqst;

		//////////////////////////////////////////////
		if(Inv_Cntrl.Inverter_Rating == 3)
		{
			Inv_Cntrl.Wave_Shift_Count = 8;// Cannot be less than 6 due to anti - islanding algorithm
			Inv_Cntrl.Half_Wave_Symm_Count = 102;
			Inv_Cntrl.Half_Wave_Symm_Adjst = 103;
		}
		else if(Inv_Cntrl.Inverter_Rating == 5)
		{
			Inv_Cntrl.Wave_Shift_Count = 11; // Cannot be less than 6 due to anti - islanding algorithm
			Inv_Cntrl.Half_Wave_Symm_Count = 105;
			Inv_Cntrl.Half_Wave_Symm_Adjst = 106;
		}
		else if(Inv_Cntrl.Inverter_Rating == 10)
		{
			Inv_Cntrl.Wave_Shift_Count = 8;// Cannot be less than 6 due to anti - islanding algorithm
			Inv_Cntrl.Half_Wave_Symm_Count = 102;
			Inv_Cntrl.Half_Wave_Symm_Adjst = 103;
		}
		else
		{
			Inv_Cntrl.Wave_Shift_Count = 8;// Cannot be less than 6 due to anti - islanding algorithm
			Inv_Cntrl.Half_Wave_Symm_Count = 102;
			Inv_Cntrl.Half_Wave_Symm_Adjst = 103;
		}

//////////////////////////////////////////////
		if(Othr.Batt_Boost_Mode_Timer_Minutes_Limit < 5)
		{
			Othr.Batt_Boost_Mode_Timer_Minutes_Limit = 5;
			Othr.Batt_Grid_Boost_Mode_Timer_Minutes_Limit = 5;
		}

		Othr.Noload_Shutdown_Activate=Temp_Settings.Noload_Shutdown_Activate;
		Othr.I_Load_CT_NoLoad_Detect_RMS_Limit=Temp_Settings.I_Load_CT_NoLoad_Detect_RMS_Limit;
		/////////////////////////////////

		Othr.Recieved_Settings_Ok = 1;//
		Othr.Recieved_Settings_Error = 1; //Set error intentionally so that this loop is not entered again

		// All temperorary variables are initialized to zero so that again and again this part of code doesn't run
		Temp_Settings.I_Batt_Charg_UL = 0;
		Temp_Settings.I_Inv_Pri_AC_100_Percent = 0;
		Temp_Settings.Relay_OFF_Delay_Time = 0;
		Temp_Settings.Relay_ON_Delay_Time = 0;

		Temp_Settings.I_PV_UL = 0;
		Temp_Settings.Inv_Ipeak_limit = 0;
		Temp_Settings.Inv_Import_Ipeak_limit = 0;
		Temp_Settings.Inv_Volt_Set = 0;
		Temp_Settings.V_Batt_LL_H_Cut_In = 0;
		Temp_Settings.V_Batt_LL_L_Cut_Off = 0;
		Temp_Settings.V_Batt_UL_H_Cut_Off = 0;
		Temp_Settings.V_Batt_UL_L_Cut_In = 0;
		Temp_Settings.V_Batt_Boost_ref=0;
		Temp_Settings.V_Batt_Equaliz_ref=0;
		Temp_Settings.V_Batt_Float_ref=0;
		Temp_Settings.V_Batt_ref = 0;
		Temp_Settings.V_Mains_AC_LL_H_Cut_In = 0;
		Temp_Settings.V_Mains_AC_LL_L_Cut_Off = 0;
		Temp_Settings.V_Mains_AC_UL_H_Cut_Off = 0;
		Temp_Settings.V_Mains_AC_UL_L_Cut_In = 0;
		Temp_Settings.V_PV_LL_H_Cut_In = 0;
		Temp_Settings.V_PV_LL_L_Cut_Off = 0;
		Temp_Settings.V_PV_UL_H_Cut_Off = 0;
		Temp_Settings.V_PV_UL_L_Cut_In = 0;
		Temp_Settings.Batt_Equalization_Reqst=0;
		Temp_Settings.PV_Power_Limit=0;

		Temp_Settings.Mode = 0;
		Temp_Settings.Grid_Connect_BV = 0;
		Temp_Settings.Grid_Chg_Start_BV = 0;
		Temp_Settings.Grid_Chg_Stop_BV = 0;
		Temp_Settings.Grid_Dis_BV = 0;
		Temp_Settings.Grid_Dis_Batt_Current = 0;
		Temp_Settings.Charging_Current_Through_Grid = 0;
		Temp_Settings.V_Grid_Boost_Ref = 0;
		Temp_Settings.V_Grid_Float_Ref = 0;
		Temp_Settings.Inverter_Rating = 0;
		Temp_Settings.Battery_Type = 0;
		Temp_Settings.Noload_Shutdown_Activate=0;
		Temp_Settings.I_Load_CT_NoLoad_Detect_RMS_Limit=0;
		}
}

void UART2_Recieved_Settings_Error_Check(void)
{
	__IO uint8_t i;
	i=1;
	Temp_Settings.Inv_Volt_Set = (uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=3;

	i=5;

	i=7;
	Temp_Settings.V_Batt_Float_ref=(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);
	Temp_Settings.V_Batt_ref=Temp_Settings.V_Batt_Float_ref;
	i=9;
	Temp_Settings.V_Batt_UL_H_Cut_Off =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=11;
	Temp_Settings.V_Batt_UL_L_Cut_In =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=13;
	Temp_Settings.V_Batt_LL_H_Cut_In =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=15;
	Temp_Settings.V_Batt_LL_L_Cut_Off =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=17;
	Temp_Settings.I_Batt_Charg_UL =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=19;
	Temp_Settings.I_Inv_Pri_AC_100_Percent =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=21;
	Temp_Settings.Relay_OFF_Delay_Time =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=23;
	Temp_Settings.Relay_ON_Delay_Time =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=25;
	Temp_Settings.V_Mains_Offset_Calib_Rqst =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=27;
	Temp_Settings.Inv_Ipeak_limit =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=29;
	Temp_Settings.Inv_Import_Ipeak_limit =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=31;
	Temp_Settings.V_PV_UL_H_Cut_Off =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=33;
	Temp_Settings.V_PV_UL_L_Cut_In =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=35;
	Temp_Settings.V_PV_LL_H_Cut_In =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=37;
	Temp_Settings.V_PV_LL_L_Cut_Off =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=39;
	Temp_Settings.I_PV_UL =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=41;
	Temp_Settings.V_Mains_AC_UL_H_Cut_Off =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=43;
	Temp_Settings.V_Mains_AC_UL_L_Cut_In =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=45;
	Temp_Settings.V_Mains_AC_LL_H_Cut_In =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=47;
	Temp_Settings.V_Mains_AC_LL_L_Cut_Off =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=49;
	Temp_Settings.V_Batt_Boost_ref =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=51;
	Temp_Settings.V_Batt_Equaliz_ref =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=53;
	Temp_Settings.Batt_Equalization_Reqst =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=55;
	Temp_Settings.PV_Power_Limit =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=57;
	Temp_Settings.Mode =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=59;
	Temp_Settings.Grid_Connect_BV =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=61;
	Temp_Settings.Grid_Chg_Start_BV =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=63;
	Temp_Settings.Grid_Chg_Stop_BV =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=65;
	Temp_Settings.Grid_Dis_BV =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=67;
	Temp_Settings.Grid_Dis_Batt_Current =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=69;
	Temp_Settings.Charging_Current_Through_Grid =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=71;
	Temp_Settings.V_Grid_Boost_Ref =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=73;
	Temp_Settings.V_Grid_Float_Ref =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=75;
	Temp_Settings.Inverter_Rating =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=77;
	Temp_Settings.Battery_Type =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=79;
	Temp_Settings.Batt_Boost_Mode_Timer_Minutes_Limit =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=81;
	Temp_Settings.Noload_Shutdown_Activate =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	i=83;
	Temp_Settings.I_Load_CT_NoLoad_Detect_RMS_Limit =(uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);

	if((Temp_Settings.V_Batt_Float_ref<Temp_Settings.V_Batt_UL_L_Cut_In)&&(Temp_Settings.V_Batt_Float_ref>=Temp_Settings.V_Batt_LL_H_Cut_In))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.V_Batt_Boost_ref<Temp_Settings.V_Batt_UL_L_Cut_In)&&(Temp_Settings.V_Batt_Boost_ref>=Temp_Settings.V_Batt_LL_H_Cut_In))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.V_Batt_Equaliz_ref<Temp_Settings.V_Batt_UL_L_Cut_In)&&(Temp_Settings.V_Batt_Equaliz_ref>=Temp_Settings.V_Batt_LL_H_Cut_In))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}



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

	if((Temp_Settings.V_Mains_Offset_Calib_Rqst>=0)&&(Temp_Settings.V_Mains_Offset_Calib_Rqst<=1))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
//			Othr.Recieved_Settings_Error = 1;
//			Othr.Recieved_Settings_Ok = 0;
		}
//	if((Temp_Settings.I_Inv_Pri_AC_110_Percent<=Temp_Settings.I_Inv_Pri_AC_125_Percent)&&(Temp_Settings.I_Inv_Pri_AC_110_Percent>Temp_Settings.I_Inv_Pri_AC_100_Percent))
//		{
//		//Othr.Recieved_Settings_Error = 0;
//		}
//	else
//		{
//			Othr.Recieved_Settings_Error = 1;
//			Othr.Recieved_Settings_Ok = 0;
//		}

//	if((Temp_Settings.I_Inv_Pri_AC_125_Percent<=Temp_Settings.I_Inv_Pri_AC_150_Percent)&&(Temp_Settings.I_Inv_Pri_AC_125_Percent>Temp_Settings.I_Inv_Pri_AC_110_Percent))
//		{
//		//Othr.Recieved_Settings_Error = 0;
//		}
//	else
//		{
//			Othr.Recieved_Settings_Error = 1;
//			Othr.Recieved_Settings_Ok = 0;
//		}

//	if((Temp_Settings.I_Inv_Pri_AC_150_Percent<=4096)&&(Temp_Settings.I_Inv_Pri_AC_150_Percent>Temp_Settings.I_Inv_Pri_AC_125_Percent))
//		{
//		//Othr.Recieved_Settings_Error = 0;
//		}
//	else
//		{
//			Othr.Recieved_Settings_Error = 1;
//			Othr.Recieved_Settings_Ok = 0;
//		}

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

	if((Temp_Settings.Batt_Equalization_Reqst==1)||(Temp_Settings.Batt_Equalization_Reqst==0))
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


	if((Temp_Settings.Mode<=4)&&(Temp_Settings.Mode>0))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.Grid_Connect_BV<=Temp_Settings.V_Batt_UL_H_Cut_Off)&&(Temp_Settings.Grid_Connect_BV>Temp_Settings.V_Batt_LL_L_Cut_Off))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.Grid_Chg_Start_BV<=Temp_Settings.V_Batt_UL_H_Cut_Off)&&(Temp_Settings.Grid_Chg_Start_BV>Temp_Settings.V_Batt_LL_L_Cut_Off))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.Grid_Chg_Stop_BV<=Temp_Settings.V_Batt_UL_H_Cut_Off)&&(Temp_Settings.Grid_Chg_Stop_BV>Temp_Settings.V_Batt_LL_L_Cut_Off))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.Grid_Dis_BV<=Temp_Settings.V_Batt_UL_H_Cut_Off)&&(Temp_Settings.Grid_Dis_BV>Temp_Settings.V_Batt_LL_L_Cut_Off))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

//	if((Temp_Settings.Grid_Dis_Batt_Current<=Temp_Settings.I_Batt_Charg_UL)&&(Temp_Settings.Grid_Dis_Batt_Current>0))
	if((Temp_Settings.Grid_Dis_Batt_Current<=4096)&&(Temp_Settings.Grid_Dis_Batt_Current>=0))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

//	if((Temp_Settings.Charging_Current_Through_Grid<=Temp_Settings.I_Batt_Charg_UL)&&(Temp_Settings.Charging_Current_Through_Grid>0))
	if((Temp_Settings.Charging_Current_Through_Grid<=4096)&&(Temp_Settings.Charging_Current_Through_Grid>=0))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}


	if((Temp_Settings.V_Grid_Boost_Ref<=Temp_Settings.V_Batt_UL_H_Cut_Off)&&(Temp_Settings.V_Grid_Boost_Ref>Temp_Settings.V_Batt_LL_L_Cut_Off))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.V_Grid_Float_Ref<=Temp_Settings.V_Grid_Boost_Ref)&&(Temp_Settings.V_Grid_Float_Ref>Temp_Settings.V_Batt_LL_L_Cut_Off))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.Inverter_Rating>0)&&(Temp_Settings.Inverter_Rating<=15))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if((Temp_Settings.Battery_Type<=1)&&(Temp_Settings.Battery_Type>=0))
		{
		//Othr.Recieved_Settings_Error = 0;
		}
	else
		{
			Othr.Recieved_Settings_Error = 1;
			Othr.Recieved_Settings_Ok = 0;
		}

	if(Temp_Settings.Batt_Boost_Mode_Timer_Minutes_Limit<=120)
	{
	//Othr.Recieved_Settings_Error = 0;
	}
	else
	{
		Othr.Recieved_Settings_Error = 1;
		Othr.Recieved_Settings_Ok = 0;
	}

	if(Temp_Settings.Noload_Shutdown_Activate<2)
	{
	//Othr.Recieved_Settings_Error = 0;
	}
	else
	{
		Othr.Recieved_Settings_Error = 1;
		Othr.Recieved_Settings_Ok = 0;
	}

	if(Temp_Settings.I_Load_CT_NoLoad_Detect_RMS_Limit<=4096)
	{
	//Othr.Recieved_Settings_Error = 0;
	}
	else
	{
		Othr.Recieved_Settings_Error = 1;
		Othr.Recieved_Settings_Ok = 0;
	}

}

void UART2_Use_Received_Data_C(void)
{

	__IO uint8_t i;

	Calibration_Temp.Error = 0;

	i = 1;
	Calibration_Temp.V_Inv = (uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);
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
	Calibration_Temp.V_Batt = (uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);
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
	Calibration_Temp.I_Inv = (uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);
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
	Calibration_Temp.I_Batt = (uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);
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
	Calibration_Temp.V_PV = (uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);
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
	Calibration_Temp.I_PV = (uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);
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
	Calibration_Temp.V_Mains = (uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);
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
	Calibration_Temp.I_Mains_CT = (uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);
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
	Calibration_Temp.I_Inv_CT = (uint16_t)(UART2_recvdata[i])|((uint16_t)(UART2_recvdata[i+1])<<8);
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



	if((Calibration_Temp.Error==0)&&(Switch_Is_ON))
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

void UART2_Use_Received_Data_M(void)
{

}

void UART2_Use_Received_Data_R(void)
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

    UART2_recvdata[0]=0;
    UART2_recvdata[1]=0;
    UART2_recvdata[2]=0;
    UART2_recvdata[3]=0;
    UART2_recvdata[4]=0;
}

void UART2_Receive_ISR_Program(void)
{
   UART2_Rec_Var.recdata= UART2_Rec_Data_Register;

      if(UART2_Rec_Flags.busy_flag==0)
      {
         if(UART2_Rec_Var.recdata=='S' && UART2_Rec_Flags.recv_check==0)//for factory set if data received as T
         {
            UART2_Rec_Flags.recv_check=1;
            UART2_Rec_Var.Array_Index=0;
            UART2_recvdata[UART2_Rec_Var.Array_Index]=UART2_Rec_Var.recdata; //SAVE RECEIVE DATA IN FACTORY SET ARRAY
         }
         else if((UART2_Rec_Flags.recv_check==1) && UART2_Rec_Var.Array_Index<89 && UART2_recvdata[0]=='S')
         {
            UART2_Rec_Var.Array_Index++;
            UART2_recvdata[UART2_Rec_Var.Array_Index]=UART2_Rec_Var.recdata;
         }
         
         ///////////////////////////////////////////////////////////////////////////////////////////////
         else if(UART2_Rec_Var.recdata=='C' && UART2_Rec_Flags.recv_check==0)//for factory set if data received as T
         {
            UART2_Rec_Flags.recv_check=1;
            UART2_Rec_Var.Array_Index=0;
            UART2_recvdata[UART2_Rec_Var.Array_Index]=UART2_Rec_Var.recdata; //SAVE RECEIVE DATA IN FACTORY SET ARRAY
         }
         else if((UART2_Rec_Flags.recv_check==1) && UART2_Rec_Var.Array_Index<31 && UART2_recvdata[0]=='C')
         {
            UART2_Rec_Var.Array_Index++;
            UART2_recvdata[UART2_Rec_Var.Array_Index]=UART2_Rec_Var.recdata;
         }
         
         ///////////////////////////////////////////////////////////////////////////////////////////////
         
         else if(UART2_Rec_Var.recdata=='M' && UART2_Rec_Flags.recv_check==0)//for factory set if data received as T
         {
            UART2_Rec_Flags.recv_check=1;
            UART2_Rec_Var.Array_Index=0;
            UART2_recvdata[UART2_Rec_Var.Array_Index]=UART2_Rec_Var.recdata; //SAVE RECEIVE DATA IN FACTORY SET ARRAY
         }
         else if((UART2_Rec_Flags.recv_check==1) && UART2_Rec_Var.Array_Index<1 && UART2_recvdata[0]=='M')
         {
            UART2_Rec_Var.Array_Index++;
            UART2_recvdata[UART2_Rec_Var.Array_Index]=UART2_Rec_Var.recdata;
         }
         
         ///////////////////////////////////////////////////////////////////////////////////////////////
         
         
         else if(UART2_Rec_Var.recdata=='R' && UART2_Rec_Flags.recv_check==0)//for factory set if data received as T
         {
            UART2_Rec_Flags.recv_check=1;
            UART2_Rec_Var.Array_Index=0;
            UART2_recvdata[UART2_Rec_Var.Array_Index]=UART2_Rec_Var.recdata; //SAVE RECEIVE DATA IN FACTORY SET ARRAY
         }
         else if((UART2_Rec_Flags.recv_check==1) && UART2_Rec_Var.Array_Index<5 && UART2_recvdata[0]=='R')
         {
            UART2_Rec_Var.Array_Index++;
            UART2_recvdata[UART2_Rec_Var.Array_Index]=UART2_Rec_Var.recdata;
         }
      }
      UART2_Clear_Receive_Flag;
      USART2->ICR|=USART_ICR_ORECF;
    }

void UART2_Send_Function_Inside_Timer_Interrupt(void) ///put this code in timer interrupt which is called every 200uS or so
{
   if((UART2_Send_Flags.Send_Enable==1)&&(UART2_TRx_Complete))
    {
      if(UART2_Send_Var.Array_Index<UART2_Send_Var.Msg_Length)
         {
           UART2_Transmit_Data_register=UART2_Txdata[UART2_Send_Var.Array_Index];
           UART2_Send_Var.Array_Index= UART2_Send_Var.Array_Index+1;
         }
      else
         {    
           UART2_Send_Flags.Send_Enable=0;//now one can populate ned data into transmit register
         }
    }   
 }

void UART2_Request_Send_Message(void)
   {
     switch (UART2_Send_Var.Arbitration_Val)
       {
         case 1:

            if((UART2_Send_Flags.Send_Enable!=1))
             {
               UART2_Send_Var.Arbitration_Val=UART2_Send_Var.Arbitration_Val+1; //
                //arbitrate only if not busy with sending. if a send request is pending then 
                //it will send and go to next arbitration level in next loop cycle
                //this code must be at the beginning of this case, please do not change the location of this if statement.
             }
            if((UART2_Send_Flags.Send_Enable!=1)&&(UART2_Msg_Send_Reqt_Flags.Msg_s==1))
              {               
                  UART2_Populate_Txdata_Array_With_Message_s();
                  UART2_Send_Flags.Send_Enable=1;
              }
         break;
         
         
         case 2:

            if((UART2_Send_Flags.Send_Enable!=1))
             {
               UART2_Send_Var.Arbitration_Val=UART2_Send_Var.Arbitration_Val+1; //
                //arbitrate only if not busy with sending. if a send request is pending then 
                //it will send and go to next arbitration level in next loop cycle.
                //else it will go the next arbitration case in the next loop cycle without sending anything 
                //this code must be at the beginning of this case, please do not change the location of this if statement.
             }
            if((UART2_Send_Flags.Send_Enable!=1)&&(UART2_Msg_Send_Reqt_Flags.Msg_c==1))
              {
                  UART2_Populate_Txdata_Array_With_Message_c();
                  UART2_Send_Flags.Send_Enable=1;
              }
         break;

         case 3:

            if((UART2_Send_Flags.Send_Enable!=1))
             {
               UART2_Send_Var.Arbitration_Val=UART2_Send_Var.Arbitration_Val+1; //
                //arbitrate only if not busy with sending. if a send request is pending then
                //it will send and go to next arbitration level in next loop cycle.
                //else it will go the next arbitration case in the next loop cycle without sending anything
                //this code must be at the beginning of this case, please do not change the location of this if statement.
             }
            if((UART2_Send_Flags.Send_Enable!=1)&&(UART2_Msg_Send_Reqt_Flags.Msg_m==1))
              {
                  UART2_Populate_Txdata_Array_With_Message_m();
                  UART2_Send_Flags.Send_Enable=1;
              }
         break;

         case 4:

            if((UART2_Send_Flags.Send_Enable!=1))
             {
               UART2_Send_Var.Arbitration_Val=UART2_Send_Var.Arbitration_Val+1; //
                //arbitrate only if not busy with sending. if a send request is pending then
                //it will send and go to next arbitration level in next loop cycle.
                //else it will go the next arbitration case in the next loop cycle without sending anything
                //this code must be at the beginning of this case, please do not change the location of this if statement.
             }
            if((UART2_Send_Flags.Send_Enable!=1)&&(UART2_Msg_Send_Reqt_Flags.Msg_r==1))
              {               
                  UART2_Populate_Txdata_Array_With_Message_r();
                  UART2_Send_Flags.Send_Enable=1;
              }
         break;
         

         default:
                UART2_Send_Var.Arbitration_Val=1;
         break;
       }
  }

void UART2_Populate_Txdata_Array_With_Message_s(void)
   {
	//if(Othr.Recieved_Settings_Ok == 1)
		//{
			UART2_Txdata[0]='s';
			UART2_Send_Var.Msg_Length=1;//(0)for message and no CRC (total 1)

			UART2_Msg_Send_Reqt_Flags.Msg_s=0;
			UART2_Send_Var.Array_Index=0;
			UART2_Send_Flags.Send_Enable=1;
		//}
    }

void UART2_Populate_Txdata_Array_With_Message_c(void)

   {
		UART2_Txdata[0]='c';

		UART2_Txdata[1]=(0x00FF & Calibration_Var.V_Inv);
		UART2_Txdata[2]=((0xFF00 & Calibration_Var.V_Inv)>>8);

		UART2_Txdata[3]=(0x00FF & Calibration_Var.V_Batt);
		UART2_Txdata[4]=((0xFF00 & Calibration_Var.V_Batt)>>8);

		UART2_Txdata[5]=(0x00FF & Calibration_Var.I_Inv);
		UART2_Txdata[6]=((0xFF00 & Calibration_Var.I_Inv)>>8);

		UART2_Txdata[7]=(0x00FF & Calibration_Var.I_Batt);
		UART2_Txdata[8]=((0xFF00 & Calibration_Var.I_Batt)>>8);

		UART2_Txdata[9]=(0x00FF & Calibration_Var.V_PV);
		UART2_Txdata[10]=((0xFF00 & Calibration_Var.V_PV)>>8);

		UART2_Txdata[11]=(0x00FF & Calibration_Var.I_PV);
		UART2_Txdata[12]=((0xFF00 & Calibration_Var.I_PV)>>8);

		UART2_Txdata[13]=(0x00FF & Calibration_Var.V_Mains);
		UART2_Txdata[14]=((0xFF00 & Calibration_Var.V_Mains)>>8);

		UART2_Txdata[15]=(0x00FF & Calibration_Var.I_Mains_CT);
		UART2_Txdata[16]=((0xFF00 & Calibration_Var.I_Mains_CT)>>8);

		UART2_Txdata[17]=(0x00FF & Calibration_Var.I_Inv_CT);
		UART2_Txdata[18]=((0xFF00 & Calibration_Var.I_Inv_CT)>>8);

		UART2_Txdata[19]=0;
		UART2_Txdata[20]=0;

		UART2_Send_Var.Msg_Length=32;//(0-26)for message and 27th location for CRC (total 28)
		UART2_Txdata[31] = UART2_crcFast((UART2_Txdata),(UART2_Send_Var.Msg_Length-1));

		UART2_Msg_Send_Reqt_Flags.Msg_c=0;
		UART2_Send_Var.Array_Index=0;
		UART2_Send_Flags.Send_Enable=1;

   }

void UART2_Populate_Txdata_Array_With_Message_m(void)
{
		int32_t X_UART2 = 0;

	/*


        UART2_Txdata[0] = 'm';

        UART2_Txdata[1]=0xDC;//(0x00FF & Norm_Avg.V_Batt);
        UART2_Txdata[2]=0x5;//((0xFF00 & Norm_Avg.V_Batt)>>8);

        if(Norm_Avg.I_Batt < 0)
        {
			UART2_Txdata[3]=0xA4;//(0x00FF & (-Norm_Avg.I_Batt));
			UART2_Txdata[4]=0x6;//((0xFF00 & (-Norm_Avg.I_Batt))>>8);

	        UART2_Txdata[5]=0x20;//0;
	        UART2_Txdata[6]=0x3;//0;
        }

        if(Norm_Avg.I_Batt > 0)
        {
        	UART2_Txdata[3]=0xA4;//0;
			UART2_Txdata[4]=0x6;//0;

			UART2_Txdata[5]=0x20;//(0x00FF & Norm_Avg.I_Batt);
			UART2_Txdata[6]=0x3;//((0xFF00 & Norm_Avg.I_Batt)>>8);
        }

        UART2_Txdata[7]=0x6C;//(0x00FF & Norm_Avg.V_PV);
        UART2_Txdata[8]=0x7;//((0xFF00 & Norm_Avg.V_PV)>>8);

		UART2_Txdata[9]=0x34;//(0x00FF & Norm_Avg.I_PV);
        UART2_Txdata[10]=0x8;//((0xFF00 & Norm_Avg.I_PV)>>8);

        UART2_Txdata[11]=0xFC;//(0x00FF & Metering.PV_KWH);
        UART2_Txdata[12]=0x8;//((0xFF00 & Metering.PV_KWH)>>8);

        UART2_Txdata[13]=0xC4;//(0x00FF & Norm_Avg.V_Inv);
        UART2_Txdata[14]=0x9;//((0xFF00 & Norm_Avg.V_Inv)>>8);

        UART2_Txdata[15]=0x8C;//(0x00FF & Norm_Rms.I_Inv_Pri);
        UART2_Txdata[16]=0xA;//((0xFF00 & Norm_Rms.I_Inv_Pri)>>8);

        UART2_Txdata[17]=0x54;//(0x00FF & Norm_Rms.V_Mains);
        UART2_Txdata[18]=0xB;//((0xFF00 & Norm_Rms.V_Mains)>>8);

        UART2_Txdata[19]=0x1C;//(0x00FF & Norm_Rms.I_Inv_Pri);
        UART2_Txdata[20]=0xC;//((0xFF00 & Norm_Rms.I_Inv_Pri)>>8);

        UART2_Txdata[21]=0xE2;//(0x00FF & Inv_Cntrl.Time_Period_Grid);
        UART2_Txdata[22]=0x4;//((0xFF00 & Inv_Cntrl.Time_Period_Grid)>>8);

        UART2_Txdata[23]=0xE8;//(0x00FF & Metering.Apparent_Power);
        UART2_Txdata[24]=0x3;//((0xFF00 & Metering.Apparent_Power)>>8);

        UART2_Txdata[25]=0xB0;//(0x00FF & Metering.Export_KWH);
        UART2_Txdata[26]=0x4;//((0xFF00 & Metering.Export_KWH)>>8);

        Othr.Fault_Reg_1 =  ((uint16_t)Avg_Limit_Flags.V_PV<<1)|((uint16_t)Avg_Limit_Flags.V_Batt);
        Othr.Fault_Reg_1 |=  ((uint16_t)Fast_Limit_Flags.Freq<<3)|((uint16_t)RMS_Limit_Flags.V_Mains<<2);
        Othr.Fault_Reg_1 |=  ((uint16_t)RMS_Limit_Flags.I_Inv<<5)|((uint16_t)Avg_Limit_Flags.V_Inv<<4);
        UART2_Txdata[27]=0xAA;//(0x00FF & Othr.Fault_Reg_1);
        UART2_Txdata[28]=0xA0;//((0xFF00 & Othr.Fault_Reg_1)>>8);
	 */

        UART2_Txdata[0] = 'm';

        UART2_Txdata[1]=(0x00FF & Norm_Avg.V_Batt);
        UART2_Txdata[2]=((0xFF00 & Norm_Avg.V_Batt)>>8);

        if(Norm_Avg.I_Batt < 0)
        {
      	  	UART2_Txdata[3]=(0x00FF & (-Norm_Avg.I_Batt));
      	  	UART2_Txdata[4]=((0xFF00 & (-Norm_Avg.I_Batt))>>8);

				UART2_Txdata[5]=0;
				UART2_Txdata[6]=0;
        }

        if(Norm_Avg.I_Batt >= 0)
        {
				UART2_Txdata[3]=0;
				UART2_Txdata[4]=0;

				UART2_Txdata[5]=(0x00FF & Norm_Avg.I_Batt);
				UART2_Txdata[6]=((0xFF00 & Norm_Avg.I_Batt)>>8);
        }

        UART2_Txdata[7]=(0x00FF & Norm_Avg.V_PV);
        UART2_Txdata[8]=((0xFF00 & Norm_Avg.V_PV)>>8);

        UART2_Txdata[9]=(0x00FF & Norm_Avg.I_PV);
        UART2_Txdata[10]=((0xFF00 & Norm_Avg.I_PV)>>8);

        UART2_Txdata[11]=(0x00FF & Metering.PV_KWH);
        UART2_Txdata[12]=((0xFF00 & Metering.PV_KWH)>>8);

        UART2_Txdata[13]=(0x00FF & Norm_Rms.V_Inv);
        UART2_Txdata[14]=((0xFF00 & Norm_Rms.V_Inv)>>8);

		  UART2_Txdata[15]=(0x00FF & Norm_Rms.I_Inv_Pri);
		  UART2_Txdata[16]=((0xFF00 & Norm_Rms.I_Inv_Pri)>>8);

        UART2_Txdata[17]=(0x00FF & Norm_Rms.V_Mains);
        UART2_Txdata[18]=((0xFF00 & Norm_Rms.V_Mains)>>8);


        if(V_Mains_Inside_RMS_Limits_Cutin) // if V mains RMS is in limits then send the data
        {
      	  UART2_Txdata[21]=(0x00FF & Inv_Cntrl.Time_Period_Grid_Shifted);
      	  UART2_Txdata[22]=((0xFF00 & Inv_Cntrl.Time_Period_Grid_Shifted)>>8);
        }
        else
        {
      	  UART2_Txdata[21] = 0;
      	  UART2_Txdata[22] = 0;
        }

        //////////////
        X_UART2=Metering.Import_KWH;
        X_UART2=(X_UART2*Calibration_Var.I_Inv)>>11; //i inv primary cal
        X_UART2=(X_UART2*Calibration_Var.V_Mains)>>11;

        UART2_Txdata[23]=(0x00FF & X_UART2); // to normalize it we are right shifting
        UART2_Txdata[24]=((0xFF00 & X_UART2)>>8);
        //////////////

        //////////////
        X_UART2=Metering.Export_KWH;
        X_UART2=(X_UART2*Calibration_Var.I_Inv)>>11;//i inv primary cal
        X_UART2=(X_UART2*Calibration_Var.V_Mains)>>11;

        UART2_Txdata[25]=(0x00FF & X_UART2);
        UART2_Txdata[26]=((0xFF00 & X_UART2)>>8);
        //////////////

        Generate_Fault_Data_For_Metering();


//        Othr.Fault_Reg_1 =  ((uint16_t)Avg_Limit_Flags.V_PV<<1)|((uint16_t)Avg_Limit_Flags.V_Batt);
//        Othr.Fault_Reg_1 |=  ((uint16_t)Fast_Limit_Flags.Freq<<3)|((uint16_t)RMS_Limit_Flags.V_Mains<<2);
//        Othr.Fault_Reg_1 |=  ((uint16_t)RMS_Limit_Flags.I_Inv<<5)|((uint16_t)Avg_Limit_Flags.V_Inv<<4);
        UART2_Txdata[27]=(0x00FF & Othr.Fault_Reg_1);
        UART2_Txdata[28]=((0xFF00 & Othr.Fault_Reg_1)>>8);

        /*
        Othr.Fault_Reg_2 =  ((uint16_t)Avg_Limit_Flags.V_Inv<<9)|((uint16_t)Avg_Limit_Flags.V_PV<<8)|((uint16_t)Avg_Limit_Flags.V_Batt<<7)|((uint16_t)Avg_Limit_Flags.Inv_Over_Temp<<6)|((uint16_t)Avg_Limit_Flags.I_PV<<5)|((uint16_t)Avg_Limit_Flags.I_Batt_Discharg<<4)|((uint16_t)Avg_Limit_Flags.I_Batt_Charg<<3)|((uint16_t)Avg_Limit_Flags.Batt_Over_Temp<<2)|((uint16_t)RMS_Limit_Flags.V_Mains<<1)|((uint16_t)RMS_Limit_Flags.I_Inv);
		UART2_Txdata[25]=(0x00FF & Othr.Fault_Reg_2);
        UART2_Txdata[26]=((0xFF00 & Othr.Fault_Reg_2)>>8);
        */
////only for temporaty debuging
        UART2_Txdata[29]=(0x00FF & Othr.Fault_Reg_2);
        UART2_Txdata[30]=((0xFF00 & Othr.Fault_Reg_2)>>8);

        UART2_Txdata[31]=(0x00FF & Norm_Rms.I_Mains_CT );
        UART2_Txdata[32]=((0xFF00 & Norm_Rms.I_Mains_CT )>>8);



        if(Inv_Cntrl.State==2)
        {
        	if(Norm_ADC.Inv_Power_Mean >= 0)
			{
			  X_UART2 = Norm_ADC.Inv_Power_Mean>>12;
			  X_UART2=(X_UART2*Calibration_Var.I_Inv_CT)>>11;
			  X_UART2=(X_UART2*Calibration_Var.V_Inv)>>11;
			}
			else
			{
			  X_UART2 = (-Norm_ADC.Inv_Power_Mean)>>12;
			  X_UART2=(X_UART2*Calibration_Var.I_Inv_CT)>>11;
			  X_UART2=(X_UART2*Calibration_Var.V_Inv)>>11;

			}

        }

        else
        {
        	if(Norm_ADC.Inv_Power_Mean >= 0)
			{
			  X_UART2 = Norm_ADC.Inv_Power_Mean>>12;
			  X_UART2=(X_UART2*Calibration_Var.I_Inv_CT)>>11;
			  X_UART2=(X_UART2*Calibration_Var.V_Inv)>>11;
			}
			else
			{
			  X_UART2 = (-Norm_ADC.Inv_Power_Mean)>>12;
			  X_UART2=(X_UART2*Calibration_Var.I_Inv_CT)>>11;
			  X_UART2=(X_UART2*Calibration_Var.V_Inv)>>11;

			}

//			if(Norm_ADC.Mains_Power_Mean >= 0)
//			{
//			  X_UART2 = Norm_ADC.Mains_Power_Mean>>12;
//			  X_UART2=(X_UART2*Calibration_Var.I_Inv)>>11;
//			  X_UART2=(X_UART2*Calibration_Var.V_Mains)>>11;
//			}
//			else
//			{
//			  X_UART2 = (-Norm_ADC.Mains_Power_Mean)>>12;
//			  X_UART2=(X_UART2*Calibration_Var.I_Inv)>>11;
//			  X_UART2=(X_UART2*Calibration_Var.V_Mains)>>11;
//			}
        }
        UART2_Txdata[33]=(0x00FF & X_UART2);
        UART2_Txdata[34]=((0xFF00 & X_UART2)>>8);

        if(Norm_ADC.Mains_Power_CT_Mean >= 0)
        {
      	  X_UART2 = Norm_ADC.Mains_Power_CT_Mean>>12;
      	  X_UART2=(X_UART2*Calibration_Var.I_Mains_CT)>>11;
      	  X_UART2=(X_UART2*Calibration_Var.V_Mains)>>11;

        }
        else
        {
      	  X_UART2 = (-Norm_ADC.Mains_Power_CT_Mean)>>12;
      	  X_UART2=(X_UART2*Calibration_Var.I_Mains_CT)>>11;
      	  X_UART2=(X_UART2*Calibration_Var.V_Mains)>>11;

        }

        UART2_Txdata[35]=(0x00FF & X_UART2);
        UART2_Txdata[36]=((0xFF00 & X_UART2)>>8);




        ////////////
    	  X_UART2 = Metering.Import_KWH_CT;
    	  X_UART2=(X_UART2*Calibration_Var.I_Mains_CT)>>11;
    	  X_UART2=(X_UART2*Calibration_Var.V_Mains)>>11;

        UART2_Txdata[37]=(0x00FF & X_UART2);
        UART2_Txdata[38]=((0xFF00 & X_UART2)>>8);
        ////////////
        ////////////
    	  X_UART2 = Metering.Export_KWH_CT;
    	  X_UART2=(X_UART2*Calibration_Var.I_Mains_CT)>>11;
    	  X_UART2=(X_UART2*Calibration_Var.V_Mains)>>11;

        UART2_Txdata[39]=(0x00FF &  X_UART2);
        UART2_Txdata[40]=((0xFF00 & X_UART2)>>8);
        ////////////
//        UART2_Txdata[41]=(0x00FF &  Othr.Batt_Boost_Mode_Timer_Minutes);
//        UART2_Txdata[42]=((0xFF00 & Othr.Batt_Boost_Mode_Timer_Minutes)>>8);
//
//        UART2_Txdata[43]=(0x00FF &  Othr.Batt_Grid_Boost_Mode_Timer_Minutes);
//        UART2_Txdata[44]=((0xFF00 & Othr.Batt_Grid_Boost_Mode_Timer_Minutes)>>8);

		  UART2_Txdata[49]=(0x00FF & Debug_Fast.Overall );
		  UART2_Txdata[50]=((0xFF00 & Debug_Fast.Overall )>>8);

		  UART2_Txdata[51]=(0x00FF & Debug_Avg.Overall );
		  UART2_Txdata[52]=((0xFF00 & Debug_Avg.Overall )>>8);

		  UART2_Txdata[53]=(0x00FF & Debug_Rms.Overall );
		  UART2_Txdata[54]=((0xFF00 & Debug_Rms.Overall )>>8);

		  UART2_Send_Var.Msg_Length=56;//(0-26)for message and 27th location for CRC (total 28)
		  UART2_Txdata[55] = UART2_crcFast((UART2_Txdata),(UART2_Send_Var.Msg_Length-1));

			UART2_Msg_Send_Reqt_Flags.Msg_m=0;
			UART2_Send_Var.Array_Index=0;
			UART2_Send_Flags.Send_Enable=1;
   }

void UART2_Populate_Txdata_Array_With_Message_r(void)

   {
        UART2_Txdata[0]='r';
        UART2_Send_Var.Msg_Length=1;//(0)for message and no CRC (total 1)
        
        UART2_Msg_Send_Reqt_Flags.Msg_r=0;
        UART2_Send_Var.Array_Index=0;
        UART2_Send_Flags.Send_Enable=1;
   }

void UART2_Slave_Variables_Init(void)
{
    UART2_Rec_Flags.Not_Used1 = 0;
    UART2_Rec_Flags.Not_Used2 = 0;
    UART2_Rec_Flags.Not_Used3 = 0;
    UART2_Rec_Flags.Not_Used4 = 0;
    UART2_Rec_Flags.Not_Used5 = 0;
    UART2_Rec_Flags.Not_Used6 = 0;
    UART2_Rec_Flags.Not_Used7 = 0;
    UART2_Rec_Flags.Not_Used8 = 0;
    UART2_Rec_Flags.Not_Used9 = 0;
    UART2_Rec_Flags.Not_Used10 = 0;
    UART2_Rec_Flags.Not_Used11 = 0;
    UART2_Rec_Flags.Not_Used12 = 0;
    UART2_Rec_Flags.T_Parameters_Copied = 0;
    UART2_Rec_Flags.Mode_Error= 0;
    UART2_Rec_Flags.recv_check= 0;
    UART2_Rec_Flags.busy_flag = 0;


    UART2_Rec_Var.recdata = 0;
    UART2_Rec_Var.U1_CRC = 0;
    UART2_Rec_Var.Array_Index = 0;
    UART2_Rec_Var.Not_Used4 = 0;
    UART2_Rec_Var.Not_Used5 = 0;
    UART2_Rec_Var.Not_Used6 = 0;
    UART2_Rec_Var.Not_Used7 = 0;
    UART2_Rec_Var.Not_Used8 = 0;
    UART2_Rec_Var.Not_Used9 = 0;
    UART2_Rec_Var.Not_Used11 = 0;
    UART2_Rec_Var.Not_Used12 = 0;
    UART2_Rec_Var.Not_Used13 = 0;
    UART2_Rec_Var.Not_Used14 = 0;
    UART2_Rec_Var.Not_Used15 = 0;
    UART2_Rec_Var.Not_Used16 = 0;

    UART2_Send_Flags.Msg_Send_Fail = 0;
    UART2_Send_Flags.Waiting_for_Ack = 0;
    UART2_Send_Flags.Send_Enable = 0;
    UART2_Send_Flags.Ackw_Byte_Expected = 0;
    UART2_Send_Flags.Ackw_Data_Expected = 0;
    UART2_Send_Flags.Not_Used6 = 0;
    UART2_Send_Flags.Not_Used7 = 0;
    UART2_Send_Flags.Not_Used8 = 0;
    UART2_Send_Flags.Not_Used9 = 0;
    UART2_Send_Flags.Not_Used10 = 0;
    UART2_Send_Flags.Not_Used11 = 0;
    UART2_Send_Flags.Not_Used12 = 0;
    UART2_Send_Flags.Not_Used13 = 0;
    UART2_Send_Flags.Not_Used14 = 0;
    UART2_Send_Flags.Not_Used15 = 0;
    UART2_Send_Flags.Not_Used16 = 0;

    UART2_Send_Var.Ack_Wait_Time_cntr = 0;
    UART2_Send_Var.Ack_Fail_Num = 0;
    UART2_Send_Var.Array_Index = 0;
    UART2_Send_Var.Msg_Length = 0;
    UART2_Send_Var.Arbitration_Val = 0;
    UART2_Send_Var.Received_Ackw = 0;
    UART2_Send_Var.Expected_Ackw_Byte = 0;
    UART2_Send_Var.Count = 0;
    UART2_Send_Var.Not_Used9 = 0;
    UART2_Send_Var.Not_Used10 = 0;
    UART2_Send_Var.Not_Used11 = 0;
    UART2_Send_Var.Not_Used12 = 0;
    UART2_Send_Var.Not_Used13 = 0;
    UART2_Send_Var.Not_Used14 = 0;
    UART2_Send_Var.Not_Used15 = 0;
    UART2_Send_Var.Not_Used16 = 0;


    UART2_Msg_Send_Reqt_Flags.Msg_A = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_B = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_C = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_D = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_E = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_F = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_G = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_H = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_I = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_J = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_K = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_L = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_M = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_N = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_O = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_P = 0;

    UART2_Msg_Send_Reqt_Flags.Msg_c = 0;
    UART2_Msg_Send_Reqt_Flags.Msg_m = 0;
	UART2_Msg_Send_Reqt_Flags.Msg_r = 0;
	UART2_Msg_Send_Reqt_Flags.Msg_s = 0;

}
