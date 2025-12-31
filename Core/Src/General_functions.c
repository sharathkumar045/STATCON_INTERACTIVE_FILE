/*
 * General_functions.c
 *
 *  Created on: 05-Apr-2019
 *      Author: ankur
 */

#include"stm32f0xx.h"
#include"main.h"
#include "definations.h"

#include "User_Defined_Functions.h"
#include "New_Struct_Variables.h"

#include "UART2_Slave_Variables.h"
#include "UART2_Slave_Functions.h"

#include "UART1_Slave_Variables.h"
#include "UART1_Slave_Functions.h"

#include "UART_Debugging_Variables.h"
#include "UART_Debugging_Functions.h"

void TIM1_OEN_DT_CCEN_IEN_STRT()
{
	LL_TIM_OC_DisablePreload(TIM1, LL_TIM_CHANNEL_CH2);
	LL_TIM_OC_DisablePreload(TIM1, LL_TIM_CHANNEL_CH3);

	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;
	TIM1->CCER |= 0x0000;// To Enable Inverter PWM write |= 0x0550
	TIM1->SR &= ~(TIM_SR_UIF); // clear the update interrupt flag
	TIM1->DIER |= TIM_DIER_UIE;  //Update interrupt enable
	TIM1->CR1 |= TIM_CR1_CEN; // counter enable

}

void ADC_DMA_CH1_CONFIG()
{
	DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));
	DMA1_Channel1->CMAR = (uint32_t)(&(ADC_Value));
	DMA1_Channel1->CNDTR = 11;
	DMA1_Channel1->CCR |= 0x0001;
}

void TIM3_CAPEN_IEN_STRT()
{
//	TIM3->CCER |= TIM_CCER_CC1E;//Capture enable
//	TIM3->DIER |= TIM_DIER_CC1IE | TIM_DIER_CC1DE; //DMA enable and capture CH1 interrupt enable
	TIM3->CR1 |= TIM_CR1_CEN;//Counter enable
}

void CAPTURE_DMA_CH4_CONFIG()
{
	DMA1_Channel4->CPAR = (uint32_t) (&(TIM3->CCR1));
	DMA1_Channel4->CMAR = (uint32_t)(&(Inv_Cntrl.Time_Period_Grid));
	DMA1_Channel4->CNDTR = 1;
	DMA1_Channel4->CCR |= DMA_CCR_EN;
}

void TIM14_CH1_PWM_Config()
{
	LL_TIM_OC_DisablePreload(TIM14, LL_TIM_CHANNEL_CH1);

	TIM14->CCER |= TIM_CCER_CC1E;
	TIM14->CCR1 = 300; //160V this is used for PV ref generation, the set voltage of PV will be 30V less than actual set voltage
	TIM14->CR1 |= TIM_CR1_CEN;
}

void Inverter_OFF()
{
	if(TIM1->BDTR & TIM_BDTR_MOE)
		{
			TIM1->BDTR&= ~TIM_BDTR_MOE;   // MOE disable
			Inv_Cntrl.Inv_Off_to_Mains_Relay_Off_Delay_Flag=1;
		}

	if((TIM1->CCR2)||(TIM1->CCR3)||(TIM1->CCER&(~0xfaaf)))
		{
			TIM1->CCR2 = 0;
			TIM1->CCR3 = 0;
			TIM1->CCER&= 0xfaaf;
			Inv_Cntrl.Inv_Off_to_Mains_Relay_Off_Delay_Flag=1;
		}
	else
		{

		}
	if(Is_Mains_Relay_ON)
	{
		Inv_Cntrl.Inv_Off_to_Mains_Relay_Off_Delay_Flag=1;
	}
}

void Switch_Off_Mains_Relay()
{
	if((Cntr.Inv_Off_to_Mains_Relay_Off_Delay>=Half_Second_Count)&&(Is_Inv_Off))
	{
		Mains_Relay_Off;
		Cntr.Inv_Off_to_Mains_Relay_Off_Delay = 0;
		Inv_Cntrl.Inv_Off_to_Mains_Relay_Off_Delay_Flag=0;
	}
}

void Switch_On_Mains_Relay()
{
	Mains_Relay_On;
	//Mains_Relay_On_to_Inv_On_Delay_Flag = 1;
}

void Inverter_ON()
{
	//if((Cntr.Mains_Relay_On_to_Inv_On_Delay>=1000)&&(Is_Mains_Relay_ON))
	//{
	TIM1->CCER|=0x0550;  // MOE bit not enabled, only channels are enabled
	//Cntr.Mains_Relay_On_to_Inv_On_Delay=0;
	//}
}

void MPPT_OFF()
{
	TIM14->CCER &= ~TIM_CCER_CC1E;
	TIM14->CCR1 = 0;
}

void MPPT_ON()
{
	TIM14->CCER |= TIM_CCER_CC1E;
	//TIM14->BDTR |= TIM_BDTR_MOE; no bdtr register in timer 14
}

void ADC_Avg_And_RMS_Cal()
{
	/***************************Average calculation*********************************/

	 Norm_Avg.I_PV_Sum 		= Norm_Avg.I_PV_Sum + ((I_PV_ADC - Inv_Cntrl.I_Inv_Pri_Calib_Offset)<<1);         // using I_Add_Mppt to store sum of all current values of 32 values
	 Norm_Avg.V_PV_Sum 		= Norm_Avg.V_PV_Sum + V_PV_ADC;
	 Norm_Avg.V_Batt_Sum 	= Norm_Avg.V_Batt_Sum + V_DC_Link_ADC;

	 Cntr.ADC_Avg 			= Cntr.ADC_Avg+ 1;



	 if(Cntr.ADC_Avg>Othr.Sine_Length)
	 {
		 Norm_Avg.V_PV 		= ((Norm_Avg.V_PV_Sum*(Norm_Avg.Multiplier))>>18);
		 Norm_Avg.V_PV = 2550;
		 Norm_Avg.V_PV 		= (Norm_Avg.V_PV * Calibration_Var.V_PV)>>11;

		 Norm_Avg.I_PV 		= ((Norm_Avg.I_PV_Sum*(Norm_Avg.Multiplier))>>18);            // this value can be used in the while function for reading current values
		 Norm_Avg.I_PV = 1990;
		 Norm_Avg.I_PV 		= (Norm_Avg.I_PV * Calibration_Var.I_PV)>>11;

		 Norm_Avg.V_Batt 	= ((Norm_Avg.V_Batt_Sum*(Norm_Avg.Multiplier))>>18);
		 Norm_Avg.V_Batt 	= (Norm_Avg.V_Batt * Calibration_Var.V_Batt)>>11;

		 if(Norm_Avg.V_PV > 4095)
		 {
			 Norm_Avg.V_PV = 4095;
		 }
		 if(Norm_Avg.I_PV > 4095)
		 {
			 Norm_Avg.I_PV = 4095;
		 }
		 if(Norm_Avg.V_Batt > 4095)
		 {
			 Norm_Avg.V_Batt = 4095;
		 }

		 Norm_Avg.I_PV_Sum=0;
		 Norm_Avg.V_PV_Sum=0;
		 Norm_Avg.V_Batt_Sum=0;

		 Cntr.ADC_Avg=1;

	 }

	 /***************************RMS calculation*********************************/

	 Norm_ADC.V_Mains_AC 			= -((Mains_Volt_ADC-Inv_Cntrl.V_Mains_Calib_Offset)<<1);
	 Norm_ADC.I_Inv_Pri_AC 			= ((I_Inv_pri_ADC - Inv_Cntrl.I_Inv_Pri_Calib_Offset )<<1); // shifting the ADC values according to offset (500/4096)
	 Norm_ADC.I_Mains_CT 			= ((I_Mains_CT_ADC - Inv_Cntrl.I_Inv_CT_Calib_Offset )<<1);

	 Norm_ADC.Mains_Power_Sum 		= ( Norm_ADC.Mains_Power_Sum +(Norm_ADC.V_Mains_AC* Norm_ADC.I_Inv_Pri_AC));

	 Norm_Rms.V_Mains_Sq_Sum 		= (Norm_Rms.V_Mains_Sq_Sum + (Norm_ADC.V_Mains_AC*Norm_ADC.V_Mains_AC));     // using Grid_V_Add we store sum of all voltage square values of  191 voltages
	 Norm_Rms.I_Inv_Pri_Sq_Sum 		= (Norm_Rms.I_Inv_Pri_Sq_Sum + (Norm_ADC.I_Inv_Pri_AC*Norm_ADC.I_Inv_Pri_AC));
	 Norm_Rms.I_Mains_CT_Sq_Sum 	= (Norm_Rms.I_Mains_CT_Sq_Sum + (Norm_ADC.I_Mains_CT*Norm_ADC.I_Mains_CT));

	 Cntr.Mean_Sq_Sum 				= Cntr.Mean_Sq_Sum+1;

	 if(Cntr.Mean_Sq_Sum>Othr.Sine_Length)
	 {
		 Norm_Rms.V_Mains_Mean_Sq 	= (((uint64_t)Norm_Rms.V_Mains_Sq_Sum*Norm_Avg.Multiplier))>>18;           // division by 192 has been shifted
		 Norm_Rms.I_Inv_Pri_Mean_Sq = (((uint64_t)Norm_Rms.I_Inv_Pri_Sq_Sum*Norm_Avg.Multiplier)>>18);
		 Norm_Rms.I_Mains_CT_Mean_Sq = (((uint64_t)Norm_Rms.I_Mains_CT_Sq_Sum*Norm_Avg.Multiplier)>>18);

		 if(Norm_ADC.Mains_Power_Sum >= 0)
		 {
			 Norm_ADC.Mains_Power_Mean= ((uint64_t)(Norm_ADC.Mains_Power_Sum*Norm_Avg.Multiplier))>>18; // for calculating the actual power consumed(KW)
		 }
		 else
		 {
			 Norm_ADC.Mains_Power_Sum = -Norm_ADC.Mains_Power_Sum;
			 Norm_ADC.Mains_Power_Mean= -(((uint64_t)(Norm_ADC.Mains_Power_Sum*Norm_Avg.Multiplier))>>18); // for calculating the actual power consumed(KW)
		 }

//		 if(Norm_ADC.Mains_Power_CT_Sum >= 0)
//		 {
//			 Norm_ADC.Mains_Power_CT_Mean= ((uint64_t)(Norm_ADC.Mains_Power_CT_Sum*Norm_Avg.Multiplier))>>18; // for calculating the actual power consumed(KW)
//		 }
//		 else
//		 {
//			 Norm_ADC.Mains_Power_CT_Sum = -Norm_ADC.Mains_Power_CT_Sum;
//			 Norm_ADC.Mains_Power_CT_Mean= -(((uint64_t)(Norm_ADC.Mains_Power_CT_Sum*Norm_Avg.Multiplier))>>18); // for calculating the actual power consumed(KW)
//		 }

		 Norm_ADC.Mains_Power_Sum= 0;
//		 Norm_ADC.Mains_Power_CT_Sum = 0;
         Norm_ADC.Inv_Power_Sum=0;

		 Norm_Rms.V_Mains_Sq_Sum= 0;
		 Norm_Rms.I_Inv_Pri_Sq_Sum = 0;
		 Norm_Rms.I_Mains_CT_Sq_Sum = 0;

		 Inv_Cntrl.RMS_Sqrt_Calc_Request=1;
		 Cntr.Mean_Sq_Sum=1;
	}
}

void PV_Duty_Dec_Code()
{
	if(PV_Cntrl.Duty_Dec==1)
	{
//		PV_Cntrl.Delta_Duty = PV_Cntrl.Delta_Duty_Neg_Step;
		PV_Cntrl.Duty_Dec=0;
		PV_Cntrl.Duty_Inc=0;

		if(MPPT_PWM_Channel>=PV_Cntrl.Delta_Duty)               //  checking if TIM1_CCR1 has not gone below a user defined value
			{
				MPPT_PWM_Channel=((MPPT_PWM_Channel)-PV_Cntrl.Delta_Duty);
			}
		else
			{
				MPPT_PWM_Channel=0;
			}
		PV_Cntrl.Duty_Sweep_Mode=0;        // since we have decremented duty cycle and intend to futher decrease the duty cycle
	}
}

void PV_Duty_Inc_Code()
{
	if(PV_Cntrl.Duty_Inc==1)
	{
//		PV_Cntrl.Delta_Duty = PV_Cntrl.Delta_Duty_Pos_Step;
		if((MPPT_PWM_Channel)<=((MPPT_Time_Period) - PV_Cntrl.Delta_Duty))
			{
				MPPT_PWM_Channel = ((MPPT_PWM_Channel)+PV_Cntrl.Delta_Duty);
				PV_Cntrl.Duty_Inc=0;
			}
		else
			{
				MPPT_PWM_Channel = MPPT_Time_Period;
			}
		PV_Cntrl.Duty_Sweep_Mode=1;       // since we have incremented duty cycle and intend to futher increase the duty cycle
	}
}

void Update_Sine_Freq_To_Grid_Freq()
{
	if(Inv_Cntrl.Disable_Grid_Freq_Update_and_Freq_Diag == 0)
	{

		Inv_Cntrl.Time_Period_Grid_Shifted = (Inv_Cntrl.Time_Period_Grid>>5)-1;
		TIM1->ARR = Inv_Cntrl.Time_Period_Grid_Shifted;   // setting the TIM1->ARR register according to the timeperiod grid for input capture
	}
}

void Anti_Islanding_Update_Sine_Freq_To_Grid_Freq()
{
	if(Inv_Cntrl.Disable_Grid_Freq_Update_and_Freq_Diag == 0)
	{
		Inv_Cntrl.Time_Period_Grid_Shifted = (Inv_Cntrl.Time_Period_Grid>>5)-1;
		if((Inv_Cntrl.Time_Period_Grid_Shifted<1500)&&(Inv_Cntrl.Time_Period_Grid_Shifted>900))//this is always true
		{
			TIM1->ARR = Inv_Cntrl.Time_Period_Grid_Shifted + Delta_Time_Period; //1250 implies 20ms so 10 implies 0.16ms this is for AI
		}
	}
}

void Cntrs_Incrmnt_Fn_for_100us_ISR()
{

	Othr.System_Off_Timer=Othr.System_Off_Timer+1;      //during development we put this off timer and share the hex file
	if(Othr.System_Off_Timer >= Othr.System_Off_Timer_Limit)
	{
		Othr.System_Off_Timer = 0;
		Othr.System_Off_Timer_Limit_Cycle_Count = Othr.System_Off_Timer_Limit_Cycle_Count+1;
		if(Othr.System_Off_Timer_Limit_Cycle_Count >= 7)
		{
			Othr.System_Off_Timer_Limit_Cycle_Count = 7;
		}
	}

	if(Othr.System_Off_Timer_Limit_Cycle_Count >= 7)
	{
		Inv_Cntrl.State = 0;   // go to state 0 and stay there
		Inv_Cntrl.First_Time_State_Entry = 1;
		Inverter_OFF();
		MPPT_OFF();
		PV_Cntrl.State = 0;
		PV_Cntrl.First_Time_State_Entry = 1;
	}

	Inv_Cntrl.Count = Inv_Cntrl.Count+1;      		// increment the count value
	if(Inv_Cntrl.Count > Othr.Sine_Length) 					// if count is greater than 192 then it is reset to 1
	{
		Inv_Cntrl.Count = 1;
	}

	Cntr.Mppt_Loop = Cntr.Mppt_Loop+1;         		//this count is used for executing MPPT algorithm after a fixed delay depending on Mppt_Loop_Time_Sec
	if(Cntr.Mppt_Loop > Two_Hundred_ms_Count)
	{
		Cntr.Mppt_Loop = Two_Hundred_ms_Count;           // we are limiting Cntr.Mppt_Loop so that it does not overflow
	}

	if(Inv_Cntrl.Start_Ov_Temp_Cnt_Request == 1)  // This counter is used for the delay for temperature signal
	{
		Cntr.Over_Temp  = Cntr.Over_Temp +1;
		if(Cntr.Over_Temp > Ten_Second_Count)
		{
			 Cntr.Over_Temp = Ten_Second_Count;
		}
	}
	else
	{
		Cntr.Over_Temp = 0;
	}

	if(Inv_Cntrl.Inv_Off_to_Mains_Relay_Off_Delay_Flag==1) // This counter is used for giving delay to relay after switching off the inverter
	{
		Cntr.Inv_Off_to_Mains_Relay_Off_Delay = Cntr.Inv_Off_to_Mains_Relay_Off_Delay+1;
		if(Cntr.Inv_Off_to_Mains_Relay_Off_Delay >= Half_Second_Count)
		{
			Cntr.Inv_Off_to_Mains_Relay_Off_Delay = Half_Second_Count;
			Inv_Cntrl.Inv_Off_to_Mains_Relay_Off_Delay_Flag = 0;
		}
	}
	else
	{
		Cntr.Inv_Off_to_Mains_Relay_Off_Delay = 0;
	}

	if(Inv_Cntrl.Disable_Grid_Freq_Update_and_Freq_Diag == 1)  // This is for staying upto transients are suppressed after switching on relay
	{
		Cntr.ZCD_Diag_Delay = Cntr.ZCD_Diag_Delay+1; 							 // delay counter for ZCD delay
		if(Cntr.ZCD_Diag_Delay >= Ten_Second_Count)   					 //  1 second delay
		{
			Cntr.ZCD_Diag_Delay = Ten_Second_Count;
		}
	}
	else
	{
		Cntr.ZCD_Diag_Delay = 0;
	}


	UART1_Send_Var.Count = UART1_Send_Var.Count+1;        // This is for send data through UART every 1ms
	if(UART1_Send_Var.Count >= 10)
	{
		UART1_Send_Var.Count = 10;
	}
	UART2_Send_Var.Count = UART2_Send_Var.Count+1;        // This is for send data through UART every 1ms
	if(UART2_Send_Var.Count >= 10)
	{
		UART2_Send_Var.Count = 10;
	}

	Cntr.ZCD_Error = Cntr.ZCD_Error + 1;  										// this counter is used for detection of failure of grid
	if(Cntr.ZCD_Error >= 585)
	{
		Cntr.ZCD_Error = 585;
	}

	if(Inv_Cntrl.Diag_Delay == 1)                        // All diagnosis are delayed upto transients are suppressed
	{
		Cntr.Diag_delay = Cntr.Diag_delay + 1;
		if(Cntr.Diag_delay > 1920)
		{
			Cntr.Diag_delay = 0;
			Inv_Cntrl.Diag_Delay = 0;
		}
	}
	else
	{
		Cntr.Diag_delay = 0;
	}


	if(Othr.Inv_Short_Ckt_Count!=0)
	{
		Cntr.Inv_Shortckt_Count_Reset_Counter = Cntr.Inv_Shortckt_Count_Reset_Counter+1;
		if(Cntr.Inv_Shortckt_Count_Reset_Counter >= Fifteen_Minute_Count)
		{
			Othr.Inv_Short_Ckt_Count = 0;
			Cntr.Inv_Shortckt_Count_Reset_Counter = 0;
		}
	}


	if(Othr.Inv_Fast_I_limit_Count!=0)
	{
		Cntr.Inv_Fast_I_limit_Count_Reset_Counter = Cntr.Inv_Fast_I_limit_Count_Reset_Counter+1;
		if(Cntr.Inv_Fast_I_limit_Count_Reset_Counter >= Fifteen_Minute_Count)
		{
			Othr.Inv_Fast_I_limit_Count = 0;
			Cntr.Inv_Fast_I_limit_Count_Reset_Counter = 0;
		}
	}

	if(Inv_Cntrl.State == 1)
	{
		Inv_Cntrl.State_1_Counter = Inv_Cntrl.State_1_Counter+1;
		if(Inv_Cntrl.State_1_Counter >= Ten_Second_Count)
		{
			Inv_Cntrl.State_1_Counter = Ten_Second_Count;
		}
	}
	else
	{
		Inv_Cntrl.State_1_Counter = 0;
	}

	if((Inv_Cntrl.State==1)&&(Inv_Cntrl.First_Time_State_Entry == 0)&&(Inv_Cntrl.Count == 1))
	{
		Inv_Cntrl.Inv_Export_Ipeak_limit = Inv_Cntrl.Inv_Export_Ipeak_limit + 2;
		if(Inv_Cntrl.Inv_Export_Ipeak_limit >= I_Export_Limit_10A_Peak)
		{
			Inv_Cntrl.Inv_Export_Ipeak_limit = I_Export_Limit_10A_Peak;
		}

		Inv_Cntrl.Inv_Import_Ipeak_limit = Inv_Cntrl.Inv_Import_Ipeak_limit + 2;
		if(Inv_Cntrl.Inv_Import_Ipeak_limit >= I_Import_Limit_10A_Peak)
		{
			Inv_Cntrl.Inv_Import_Ipeak_limit = I_Import_Limit_10A_Peak;
		}
	}
	else if(Inv_Cntrl.State == 0)
	{
		Inv_Cntrl.Inv_Import_Ipeak_limit = 0;
		Inv_Cntrl.Inv_Export_Ipeak_limit = 0;
	}

	Zero_Crossing_Detection_Counter_Fn();
}

void Zero_Crossing_Detection_Counter_Fn()
{
//	Othr.ZCD_Counter = Othr.ZCD_Counter + 1;
	if(ZCD_Pin_is_Low)
	{
		Othr.ZCD_Present_State = 1;
//		GPIOC->ODR |= GPIO_ODR_5;
	}
	else
	{
		Othr.ZCD_Present_State = 0;
//		GPIOC->ODR &= ~GPIO_ODR_5;
	}

	if((Othr.ZCD_Prev_State == 1)&&(Othr.ZCD_Present_State == 0))
	{
		Othr.ZCD_Value_From_Timer = TIM3->CNT;

		TIM3->CNT = 0;

		Cntr.ZCD_Error 		= 0;
//		if((Othr.ZCD_Counter > 180)&&(Othr.ZCD_Counter < 210))
//		{
//			Othr.ZCD_Value 		= Othr.ZCD_Counter;
//			Cntr.ZCD_Error 		= 0;
//			Inv_Cntrl.Count 	= 4;
//			Inv_Cntrl.ZCD 		= 0;
//		}
//		Othr.ZCD_Counter 	= 0;

		if((Othr.ZCD_Value_From_Timer > 9000)&&(Othr.ZCD_Value_From_Timer < 11111)) //if the freq between 45Hz and 55Hz, for 50Hz it is 10000
		{
			TIM1->ARR 					= (Othr.ZCD_Value_From_Timer>>3);
			Inv_Cntrl.Time_Period_Grid 	= (Othr.ZCD_Value_From_Timer<<2);
			Inv_Cntrl.Count 			= 4;
			Cntr.ZCD_Error 				= 0;
			Inv_Cntrl.ZCD 				= 0; // if this is one then it is used for anti-islanding code
		}
	}

//	if(Othr.ZCD_Counter >= 2000)
//	{
//		Othr.ZCD_Counter = 0;
//	}

	Othr.ZCD_Prev_State = Othr.ZCD_Present_State;

//	Inv_Cntrl.Time_Period_Grid = ((Othr.ZCD_Value * 208));
//	if(Freq_In_limits)
//	{
//		Inv_Cntrl.Time_Period_Grid_Shifted = Inv_Cntrl.Time_Period_Grid>>5;
//		TIM1->ARR = Inv_Cntrl.Time_Period_Grid_Shifted;// + Delta_Time_Period;
//	}
}

void Batt_V_Err_Integ_Saturator_Code()
{

	if(Inv_Cntrl.V_Batt_Err_Integ > (Inv_Cntrl.Inv_Export_Ipeak_limit<<12))
	{
		Inv_Cntrl.V_Batt_Err_Integ = (Inv_Cntrl.Inv_Export_Ipeak_limit<<12);
	}

	if(Inv_Cntrl.V_Batt_Err_Integ < -((Inv_Cntrl.Inv_Import_Ipeak_limit<<12)))
	{
		Inv_Cntrl.V_Batt_Err_Integ = -(Inv_Cntrl.Inv_Import_Ipeak_limit<<12);
	}

	Inv_Cntrl.V_Batt_Err_Prop_Integ = Inv_Cntrl.V_Loop_Proportional_OP + (Inv_Cntrl.V_Batt_Err_Integ>>12);

	if(Inv_Cntrl.V_Batt_Err_Prop_Integ > Inv_Cntrl.Inv_Export_Ipeak_limit)
	{
		Inv_Cntrl.V_Batt_Err_Prop_Integ = Inv_Cntrl.Inv_Export_Ipeak_limit;
	}

	if(Inv_Cntrl.V_Batt_Err_Prop_Integ < -(Inv_Cntrl.Inv_Import_Ipeak_limit))
	{
		Inv_Cntrl.V_Batt_Err_Prop_Integ = -Inv_Cntrl.Inv_Import_Ipeak_limit;
	}
}

void V_Generate_Saturator_Code()
{
	if (Inv_Cntrl.V_Generate>=625)
	{
		Inv_Cntrl.V_Generate = 625;
	}

	if (Inv_Cntrl.V_Generate<-625)
	{
		Inv_Cntrl.V_Generate = -625;
	}
}

void V_Delta_Generate_Saturator_Code()
{
	if (Inv_Cntrl.V_Delta_Generate>=1170)
	{
		Inv_Cntrl.V_Delta_Generate = 1170;
	}

	else if (Inv_Cntrl.V_Delta_Generate<-1170)
	{
		Inv_Cntrl.V_Delta_Generate = -1170;
	}
}

void Fast_Diagnostics()
{
	if (V_Batt_Outof_Fast_Limits)
	{
		if(Inv_Cntrl.State == 1)
		{
			Debug_Fast.A |= 0x1U;
		}

		Debug_Fast.B |= 0x1U;

		Fast_Limit_Flags.V_Batt = 1;
		Avg_Limit_Flags.V_Batt = 1;

		Inverter_OFF();
		Inv_Cntrl.State = 0;
		Inv_Cntrl.First_Time_State_Entry=1;

		MPPT_OFF();
		PV_Cntrl.State = 0;
		PV_Cntrl.First_Time_State_Entry=1;

	}
	else
	{
		Debug_Fast.B &= ~(0x1U);
	}

	if(I_Inv_Pri_AC_Outof_Fast_Limits)
	{
		if(Inv_Cntrl.State == 1)
		{
			Debug_Fast.A |= (0x1U<<1);
			Debug_Fast.D = Norm_ADC.I_Inv_Pri_AC;
		}

		Debug_Fast.B |= (0x1U<<1);
		Inverter_OFF();
		Inv_Cntrl.State = 0;
		Inv_Cntrl.First_Time_State_Entry=1;
	}
	else
	{
		Debug_Fast.B &= ~(0x1U<<1);
	}

	if(Cntr.ZCD_Error>=585) //585 refers to 3 cycles
	{
		if(Inv_Cntrl.State == 1)
		{
			Debug_Fast.A |= (0x1U<<2);
		}

		if(Inv_Cntrl.State == 1)
		{
			Othr.Inv_Off_Latch=1;
		}

		Debug_Fast.B |= (0x1U<<2);
		Inv_Cntrl.Time_Period_Grid =Inv_Cntrl.Time_Period_Grid_UL+10;// we use this for generating freq flags
		Fast_Limit_Flags.Freq = 1;

		Inverter_OFF();
		Inv_Cntrl.State = 0;
		Inv_Cntrl.First_Time_State_Entry=1;

		MPPT_OFF();
		PV_Cntrl.State = 0;
		PV_Cntrl.First_Time_State_Entry=1;
	}
	else
	{
		Debug_Fast.B &= ~(0x1U<<2);
	}

	if((Freq_Out_Of_limits)&&(Inv_Cntrl.Disable_Grid_Freq_Update_and_Freq_Diag==0))
	{
//		if(Inv_Cntrl.State == 1)
//		{
//			Othr.Inv_Off_Latch=1;
//		}

		if(Inv_Cntrl.State == 1)
		{
			Debug_Fast.A |= (0x1U<<3);
		}
		Debug_Fast.B |= (0x1U<<3);
		Inverter_OFF();
		Inv_Cntrl.State = 0;
		Inv_Cntrl.First_Time_State_Entry=1;

		MPPT_OFF();
		PV_Cntrl.State = 0;
		PV_Cntrl.First_Time_State_Entry=1;

		Fast_Limit_Flags.Freq = 1;
		Debug_Fast.C = 1;
	}
	else
	{
		Debug_Fast.B &= ~(0x1U<<3);
	}

	if((Norm_ADC.V_Mains_AC>Norm_ADC_Limits.V_Mains_AC_UL)||(Norm_ADC.V_Mains_AC<Norm_ADC_Limits.V_Mains_AC_LL))  // if the voltage upper limit is crossed then switch off the inverter immediately
	{
		if(Inv_Cntrl.State == 1)
		{
			Debug_Fast.A |= (0x1U<<4);
		}
		Debug_Fast.B |= (0x1U<<4);
		Fast_Limit_Flags.V_Mains = 1;

		Inverter_OFF();
		Inv_Cntrl.State = 0;
		Inv_Cntrl.First_Time_State_Entry=1;

		MPPT_OFF();
		PV_Cntrl.State = 0;
		PV_Cntrl.First_Time_State_Entry=1;
	}
	else
	{
		Debug_Fast.B &= ~(0x1U<<4);
	}

	if((Switch_Is_OFF))  // if the switch is turned off then system is shut down
	{
		if(Inv_Cntrl.State == 1)
		{
			Debug_Fast.A |= (0x1U<<5);
		}
		Debug_Fast.B |= (0x1U<<5);
		Inverter_OFF();
		Inv_Cntrl.State=0;   // go to state 0 and stay there
		Inv_Cntrl.First_Time_State_Entry=1;

		TIM3->CCR2 = 0;

		Othr.Previous_Switch_State = 0;
	}
	else
	{
		Debug_Fast.B &= ~(0x1U<<5);
	}

	if(Switch_Is_ON)
	{
		if(Othr.Previous_Switch_State==0)
		{
			Othr.Inv_Off_Latch=0;

			Fast_Limit_Flags.DC_insulation = 0;
		}
		Othr.Previous_Switch_State=1;
	}

//	if(Driver_Supply_is_Not_Ok)  // if the drivers aren't getting power supply
//	{
//		if(Inv_Cntrl.State == 1)
//		{
//			Debug_Fast.A |= (0x1U<<6);
//		}
//		Debug_Fast.B |= (0x1U<<6);
//		Inverter_OFF();
//		Inv_Cntrl.State=0;   // go to state 0 and stay there
//		Inv_Cntrl.First_Time_State_Entry=1;
//
//		TIM3->CCR2 = 0;
//
//		Othr.Previous_Switch_State = 0;
//	}
//	else
//	{
//		Debug_Fast.B &= ~(0x1U<<6);
//	}
//
//	if(Driver_Supply_Ok)
//	{
//		if(Othr.Previous_Switch_State==0)
//		{
//			Othr.Inv_Off_Latch=0;
////			Inverter_Relay_OFF;
//		}
//		Othr.Previous_Switch_State=1;
//	}
}

void Avg_Diagnostics()
{
	/********************   Common diagnosis for all states  *********************/
//
//	if(V_PV_Outof_Avg_Limits_Cutoff)
//	{
//		Fast_Limit_Flags.V_PV = 1;
//		Avg_Limit_Flags.V_PV = 1;
//
//		MPPT_OFF();
//		PV_Cntrl.State = 0;
//		PV_Cntrl.First_Time_State_Entry=1;
//
//		Debug_Fast.A |= (0x1U<<6);
//	}

//	if(Inv_Cntrl.State == 0)
//	{
//		if((DC_Insulation_ADC > 1450) && (Fast_Limit_Flags.DC_insulation == 0))
//		{
//			Inverter_OFF();
//			Inv_Cntrl.State = 0;
//			Inv_Cntrl.First_Time_State_Entry=1;
//
//			MPPT_OFF();
//			PV_Cntrl.State = 0;
//			PV_Cntrl.First_Time_State_Entry=1;
//		}
//		if(Fast_Limit_Flags.DC_insulation)
//		{
//			Inverter_OFF();
//			Inv_Cntrl.State = 0;
//			Inv_Cntrl.First_Time_State_Entry=1;
//		}
//	}
//	else
//	{
//		if(Inv_Cntrl.State_1_Counter >= Ten_Second_Count)
//		{
//			if((DC_Insulation_ADC < 413)||(DC_Insulation_ADC > 1450))
//			{
//				Inverter_OFF();
//				Inv_Cntrl.State = 0;
//				Inv_Cntrl.First_Time_State_Entry=1;
//
//				MPPT_OFF();
//				PV_Cntrl.State = 0;
//				PV_Cntrl.First_Time_State_Entry=1;
//
//				Fast_Limit_Flags.DC_insulation = 1;
//			}
//		}
//	}
}

void RMS_Diagnostics()
{
	/********************   Diagnosis for State 1   *********************/

	if(V_Mains_Outof_RMS_Limits_Cutoff)
	{
		if(Inv_Cntrl.State == 1)
		{
			Debug_Fast.A |= (0x1U<<8);
		}
		Debug_Fast.B |= (0x1U<<8);
		RMS_Limit_Flags.V_Mains=1;

		Inverter_OFF();
		Inv_Cntrl.State = 0;
		Inv_Cntrl.First_Time_State_Entry=1;

		MPPT_OFF();
		PV_Cntrl.State = 0;
		PV_Cntrl.First_Time_State_Entry=1;
	}
	else
	{
		Debug_Fast.B &= ~(0x1U<<8);
	}

	if(I_Inv_Pri_Outof_RMS_Limits_UL)
	{
		if(Inv_Cntrl.State == 1)
		{
			Debug_Fast.A |= (0x1U<<9);
		}
		Debug_Fast.B |= (0x1U<<9);
		Inverter_OFF();
		Inv_Cntrl.State = 0;
		Inv_Cntrl.First_Time_State_Entry=1;
	}
	else
	{
		Debug_Fast.B &= ~(0x1U<<9);
	}

}

void Grid_To_Inverter_State_Transition_Fn()
{
	Mains_Relay_Off;  // Switch off the mains relay
	Inverter_OFF();
//	Inv_Cntrl.In_Transition_to_Inv_Mode = 1;
//	Inv_Cntrl.I_Peak_Ref  = 0 ;
//	Cntr.Grid_To_Inv_Off_Delay = Cntr.Grid_To_Inv_Off_Delay + 1;
//	if(Cntr.Grid_To_Inv_Off_Delay >= 164)
//	{
	Inv_Cntrl.State = 2;
	Inv_Cntrl.Direct_State_Entry =1;

	Fast_Limit_Flags.V_Mains = 1;
	RMS_Limit_Flags.V_Mains = 1;
	Fast_Limit_Flags.Freq = 1;
	Inv_Cntrl.Diag_Delay = 1;

	Cntr.Grid_To_Inv_Off_Delay = 0;
//		Inv_Cntrl.In_Transition_to_Inv_Mode = 0;
	Inv_Cntrl.Synchronizing_Var = 0;
//	}
}

void Check_if_V_Mains_Out_of_Bounds()
{
	if((Inv_Cntrl.Count>40)&&(Inv_Cntrl.Count<60))
	{
		if((Norm_ADC.V_Mains_AC<Sine_Wave_170V[Inv_Cntrl.Count-1]) || (Norm_ADC.V_Mains_AC>Sine_Wave_270V[Inv_Cntrl.Count-1]))
		{
			Fast_Limit_Flags.Grid_Failure = 1;
		}
	}
	if((Inv_Cntrl.Count>130)&&(Inv_Cntrl.Count<150))
	{
		if((Norm_ADC.V_Mains_AC>Sine_Wave_170V[Inv_Cntrl.Count-1]) || (Norm_ADC.V_Mains_AC<Sine_Wave_270V[Inv_Cntrl.Count-1]))
		{
			Fast_Limit_Flags.Grid_Failure = 1;
		}
	}
}

void Energy_Calculation()
{
	if(Norm_ADC.Mains_Power_Mean>=0)
	{
		Metering.Export_Energy = Metering.Export_Energy + Norm_ADC.Mains_Power_Mean;
	}
	else
	{
		Metering.Import_Energy = Metering.Import_Energy - Norm_ADC.Mains_Power_Mean;
	}

 	/*
 	 * KWHr =WHr/1000
 	 * 			= Wmin/(60*1000)
 	 * 			= Wsec/(60*60*1000)
 	 * 			= Wmsec/(60*60*1000*1000)
 	 *
 	 * KWHr = (E * x *y *20)/(60*60*1000*1000)
 	 *
 	 * E for 1KWHr = (60*60*1000*1000)/(x * y *20)
 	 *
 	 * x -> Mains Voltage_Sensing_Ratio = 0.119
 	 * y -> Mains Current_Sensing_Ratio = 0.023
 	 * 20 -> this is because of 20milli seconds
 	 *
 	 * As per request from STATCON(Niranjan) we will be sending 0.01 KWH
 	 *
 	 * therefore
 	 * Point_01_E_for_1KWHr = E / 100
 	 *
 	 */
	while(Metering.Export_Energy > Point_01_E_for_1KWHr)
	{
		Metering.Export_Energy = Metering.Export_Energy - Point_01_E_for_1KWHr;
		Metering.Export_KWH = Metering.Export_KWH +1;
	}

	while(Metering.Import_Energy > Point_01_E_for_1KWHr)
	{
		Metering.Import_Energy = Metering.Import_Energy - Point_01_E_for_1KWHr;
		Metering.Import_KWH = Metering.Import_KWH +1;
	}
}

void Energy_Calculation_With_Mains_CT()
{
	if(Norm_ADC.Mains_Power_CT_Mean>=0)
	{
		Metering.Export_Energy_CT = Metering.Export_Energy_CT + Norm_ADC.Mains_Power_CT_Mean;
	}
	else
	{
		Metering.Import_Energy_CT = Metering.Import_Energy_CT - Norm_ADC.Mains_Power_CT_Mean;
	}

 	/*
 	 * KWHr =WHr/1000
 	 * 			= Wmin/(60*1000)
 	 * 			= Wsec/(60*60*1000)
 	 * 			= Wmsec/(60*60*1000*1000)
 	 *
 	 * KWHr = (E * x *y *20)/(60*60*1000*1000)
 	 *
 	 * E for 1KWHr = (60*60*1000*1000)/(x * y *20)
 	 *
 	 * x -> Mains Voltage_Sensing_Ratio
 	 * y -> Mains Current_Sensing_Ratio
 	 * 20 -> this is because of 20milli seconds
 	 *
 	 * As per request from STATCON(Niranjan) we will be sending 0.01 KWH
 	 *
 	 * therefore
 	 *
 	 * Point_01_E_for_1KWHr_Mains_CT = E / 100
 	 */
	while(Metering.Export_Energy_CT > Point_01_E_for_1KWHr_Mains_CT)
	{
		Metering.Export_Energy_CT = Metering.Export_Energy_CT - Point_01_E_for_1KWHr_Mains_CT;
		Metering.Export_KWH_CT = Metering.Export_KWH_CT +1;
	}

	while(Metering.Import_Energy_CT > Point_01_E_for_1KWHr_Mains_CT)
	{
		Metering.Import_Energy_CT = Metering.Import_Energy_CT - Point_01_E_for_1KWHr_Mains_CT;
		Metering.Import_KWH_CT = Metering.Import_KWH_CT +1;
	}

}

void Energy_Calculation_With_Mains_CT_For_GSM_Card()
{
	if(Norm_ADC.Mains_Power_CT_Mean>=0)
	{
		Metering_GSM_Card.Export_Energy_CT = Metering_GSM_Card.Export_Energy_CT + Norm_ADC.Mains_Power_CT_Mean;
	}
	else
	{
		Metering_GSM_Card.Import_Energy_CT = Metering_GSM_Card.Import_Energy_CT - Norm_ADC.Mains_Power_CT_Mean;
	}

 	/*
 	 * KWHr =WHr/1000
 	 * 			= Wmin/(60*1000)
 	 * 			= Wsec/(60*60*1000)
 	 * 			= Wmsec/(60*60*1000*1000)
 	 *
 	 * KWHr = (E * x *y *20)/(60*60*1000*1000)
 	 *
 	 * E for 1KWHr = (60*60*1000*1000)/(x * y *20)
 	 *
 	 * x -> Mains Voltage_Sensing_Ratio
 	 * y -> Mains Current_Sensing_Ratio
 	 * 20 -> this is because of 20milli seconds
 	 *
 	 * As per request from STATCON(Niranjan) we will be sending 0.01 KWH
 	 *
 	 * therefore
 	 * 0.01 KWHr = (E * x *y *20)/(60*60*1000*1000)*(1/100)
 	 *
 	 */
	while(Metering.Export_Energy_CT > Point_01_E_for_1KWHr_Mains_CT)
	{
		Metering_GSM_Card.Export_Energy_CT = Metering_GSM_Card.Export_Energy_CT - Point_01_E_for_1KWHr_Mains_CT;
		Metering_GSM_Card.Export_KWH_CT = Metering_GSM_Card.Export_KWH_CT +1;
	}

	while(Metering.Import_Energy_CT > Point_01_E_for_1KWHr_Mains_CT)
	{
		Metering_GSM_Card.Import_Energy_CT = Metering_GSM_Card.Import_Energy_CT - Point_01_E_for_1KWHr_Mains_CT;
		Metering_GSM_Card.Import_KWH_CT = Metering_GSM_Card.Import_KWH_CT +1;
	}

}

void Energy_Calculation_For_GSM_Card()
{
	if(Norm_ADC.Mains_Power_Mean>=0)
	{
		Metering_GSM_Card.Export_Energy = Metering_GSM_Card.Export_Energy + Norm_ADC.Mains_Power_Mean;
	}
	else
	{
		Metering_GSM_Card.Import_Energy = Metering_GSM_Card.Import_Energy - Norm_ADC.Mains_Power_Mean;
	}

 	/*
 	 * KWHr =WHr/1000
 	 * 			= Wmin/(60*1000)
 	 * 			= Wsec/(60*60*1000)
 	 * 			= Wmsec/(60*60*1000*1000)
 	 *
 	 * KWHr = (E * x *y *20)/(60*60*1000*1000)
 	 *
 	 * E for 1KWHr = (60*60*1000*1000)/(x * y *20)
 	 *
 	 * x -> Mains Voltage_Sensing_Ratio
 	 * y -> Mains Current_Sensing_Ratio
 	 * 20 -> this is because of 20milli seconds
 	 *
 	 */
		while(Metering_GSM_Card.Export_Energy > Point_01_E_for_1KWHr)
		{
			Metering_GSM_Card.Export_Energy = Metering_GSM_Card.Export_Energy - Point_01_E_for_1KWHr;
			Metering_GSM_Card.Export_KWH = Metering_GSM_Card.Export_KWH +1;
		}

		while(Metering_GSM_Card.Import_Energy > Point_01_E_for_1KWHr)
		{
			Metering_GSM_Card.Import_Energy = Metering_GSM_Card.Import_Energy - Point_01_E_for_1KWHr;
			Metering_GSM_Card.Import_KWH = Metering_GSM_Card.Import_KWH +1;
		}
}

void AH_Calculation()
{
	/*
	 * 1AH = Ampere_min/60
	 * 		= Ampere_Sec/(60*60)
	 * 		= Ampere_millisec/(60*60*1000)
	 * 		= Ampere_Avg*20 /(60*60*1000)
	 * 		= ADC*x*20/(60*60*1000)
	 *
	 *A for 1AH = (60*60*1000)/(20*x)
	 *
	 * x -> battery current sensing ratio = 0.024414
	 *
	 */
	if(Norm_Avg.I_Batt >=0)
	{
		Metering.Batt_Ampere_Out_Sum = Metering.Batt_Ampere_Out_Sum + Norm_Avg.I_Batt;
		while(Metering.Batt_Ampere_Out_Sum>Value_for_1AHr)
		{
			Metering.Batt_Ampere_Out_Sum = Metering.Batt_Ampere_Out_Sum - 1;
			Metering.Batt_AH_Out = Metering.Batt_AH_Out +1;
		}
	}
	else
	{
		Metering.Batt_Ampere_In_Sum = Metering.Batt_Ampere_In_Sum - Norm_Avg.I_Batt;
		while(Metering.Batt_Ampere_In_Sum>Value_for_1AHr)
		{
			Metering.Batt_Ampere_In_Sum = Metering.Batt_Ampere_In_Sum - 1;
			Metering.Batt_AH_In = Metering.Batt_AH_In +1;
		}
	}
}

void AH_Calculation_For_GSM_Card()
{
	/*
	 * 1AH = Ampere_min/60
	 * 		= Ampere_Sec/(60*60)
	 * 		= Ampere_millisec/(60*60*1000)
	 * 		= Ampere_Avg*20 /(60*60*1000)
	 * 		= ADC*x*20/(60*60*1000)
	 *
	 *A for 1AH = (60*60*1000)/(20*x)
	 *
	 * x -> battery current sensing ratio = 0.024414
	 *
	 */
	if(Norm_Avg.I_Batt >=0)
	{
		Metering_GSM_Card.Batt_Ampere_Out_Sum = Metering_GSM_Card.Batt_Ampere_Out_Sum + Norm_Avg.I_Batt;
		while(Metering_GSM_Card.Batt_Ampere_Out_Sum>Value_for_1AHr)
		{
			Metering_GSM_Card.Batt_Ampere_Out_Sum = Metering_GSM_Card.Batt_Ampere_Out_Sum - 1;
			Metering_GSM_Card.Batt_AH_Out = Metering_GSM_Card.Batt_AH_Out +1;
		}
	}
	else
	{
		Metering_GSM_Card.Batt_Ampere_In_Sum = Metering_GSM_Card.Batt_Ampere_In_Sum - Norm_Avg.I_Batt;
		while(Metering_GSM_Card.Batt_Ampere_In_Sum>Value_for_1AHr)
		{
			Metering_GSM_Card.Batt_Ampere_In_Sum = Metering_GSM_Card.Batt_Ampere_In_Sum - 1;
			Metering_GSM_Card.Batt_AH_In = Metering_GSM_Card.Batt_AH_In +1;
		}
	}
}


void PV_KWhr_Calculation()
{
	Metering.PV_KWH_Sum = Metering.PV_KWH_Sum + (Norm_Avg.I_PV * Norm_Avg.V_PV);
 	/*
 	 * KWHr =WHr/1000
 	 * 			= Wmin/(60*1000)
 	 * 			= Wsec/(60*60*1000)
 	 * 			= Wmsec/(60*60*1000*1000)
 	 *
 	 * KWHr = (E * x *y *20)/(60*60*1000*1000)
 	 *
 	 * E for 1KWHr = (60*60*1000*1000)/(x * y *20)
 	 *
 	 * x -> PV Voltage_Sensing_Ratio = 0.18228217
 	 * y -> PV Current_Sensing_Ratio = 0.023
 	 * 20 -> this is because of 20milli seconds
 	 *
 	 * E for 1KWHr = (60*60*1000*1000)/(x * y *20)
 	  As per request from STATCON(Niranjan) we will be sending 0.01 KWH
 	 *
 	 * therefore
 	 * 0.01 KWHr = (60*60*1000*1000)/(x * y *20)*(1/100)
 	 */
	if(Metering.PV_KWH_Sum > P_01_PV_E_For_1KWHr)
	{
		Metering.PV_KWH_Sum = Metering.PV_KWH_Sum - P_01_PV_E_For_1KWHr;
		Metering.PV_KWH = Metering.PV_KWH + 1;
	}

}


void PV_KWhr_Calculation_For_GSM_Card()
{
	Metering_GSM_Card.PV_KWH_Sum = Metering_GSM_Card.PV_KWH_Sum + (Norm_Avg.I_PV * Norm_Avg.V_PV);
 	/*
 	 * KWHr =WHr/1000
 	 * 			= Wmin/(60*1000)
 	 * 			= Wsec/(60*60*1000)
 	 * 			= Wmsec/(60*60*1000*1000)
 	 *
 	 * KWHr = (E * x *y *20)/(60*60*1000*1000)
 	 *
 	 * E for 1KWHr = (60*60*1000*1000)/(x * y *20)
 	 *
 	 * x -> PV Voltage_Sensing_Ratio = 0.01664595
 	 * y -> PV Current_Sensing_Ratio = 0.05935785
 	 * 20 -> this is because of 20milli seconds
 	 *
 	 */
	if(Metering_GSM_Card.PV_KWH_Sum > P_01_PV_E_For_1KWHr)
	{
		Metering_GSM_Card.PV_KWH_Sum = Metering_GSM_Card.PV_KWH_Sum - P_01_PV_E_For_1KWHr;
		Metering_GSM_Card.PV_KWH = Metering_GSM_Card.PV_KWH + 1;
	}

}

void Variable_Init()
{
	Norm_ADC.I_Inv_Pri_AC 		= 0;
	Norm_ADC.V_Mains_AC 		= 0;
	Norm_ADC.Mains_Power_Sum	= 0;
	Norm_ADC.Mains_Power_CT_Sum	= 0;
	Norm_ADC.Inv_Power_Sum		= 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Norm_Avg.I_Batt 			= 0;
	Norm_Avg.I_Batt_Sum 		= 0;
	Norm_Avg.I_PV 				= 0;
	Norm_Avg.I_PV_Sum 			= 0;
	Norm_Avg.V_Batt 			= 0;
	Norm_Avg.V_Batt_Sum 		= 0;
	Norm_Avg.V_PV 				= 0;
	Norm_Avg.V_PV_Sum 			= 0;
	Norm_Avg.Multiplier 		= 1365; //(1/192)<<18
	Norm_Avg.Power_Multiplier 	= 2730; // (1/96)<< 18
	Norm_Avg.Batt_Temperature 	= 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Norm_Rms.I_Inv_Pri 			= 0;
	Norm_Rms.I_Inv_Pri_Mean_Sq 	= 0;
	Norm_Rms.I_Inv_Pri_Sq_Sum 	= 0;
	Norm_Rms.V_Mains 			= 0;
	Norm_Rms.V_Mains_Mean_Sq 	= 0;
	Norm_Rms.V_Mains_Sq_Sum 	= 0;
	Norm_Rms.I_Mains_CT 		= 0;
	Norm_Rms.I_Mains_CT_Mean_Sq = 0;
	Norm_Rms.I_Mains_CT_Sq_Sum 	= 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Cntr.ADC_Avg 				= 0;
	Cntr.Inv_EOS0_1  			= 0;
	Cntr.Inv_Soft_Start 		= 0;
	Cntr.Mean_Sq_Sum 			= 0;
	Cntr.Mppt_Loop 				= 0;
	Cntr.Over_Temp 				= 0;
	Cntr.PV_EOS0 				= 0;
	Cntr.Inv_Off_to_Mains_Relay_Off_Delay = 0;
	Cntr.ZCD_Error 				= 0;
	Cntr.Watt_Hour 				= 0;
	Cntr.Watt_Min 				= 0;
	Cntr.Watt_Sec 				= 0;
	Cntr.Watt_mSec  			= 0;
	Cntr.Calibration_Avg 		= 0;
	Cntr.Grid_To_Inv_Off_Delay  = 0;
	Cntr.Tfr_Saturation_Detect  = 0;
	Cntr.PV_Absent 				= 0;
	Cntr.Inv_Shortckt_Count_Reset_Counter=0;
	Cntr.Relay_Activation_Delay = 0;
	Cntr.Grid_Failure_Detection = 0;

	Cntr.Noload_Detect 			= 0;
	Cntr.Noload_Turnoff_to_Turnon_Delay = 0;
	Cntr.Calibration_Rqst 		= 0;

	Cntr.V_Mains_Offset_Calib_Avg = 0;
	Cntr.V_Mains_Offset_Calib_Rqst = 0;

	Cntr.PV_Ref_Soft 			= 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Norm_ADC_Limits.I_PV_UL 				= 3905; //2403=40A
	Norm_ADC_Limits.V_Batt_LL 				= 1027; //250V
	Norm_ADC_Limits.V_Batt_UL 				= 1960; //480V
	Norm_ADC_Limits.V_PV_LL 				= 0;
	Norm_ADC_Limits.V_PV_UL 				= 4000; //150V
	Norm_ADC_Limits.V_Mains_AC_LL 			= -3500;//415
	Norm_ADC_Limits.V_Mains_AC_UL 			= 3500; //415
	Norm_ADC_Limits.I_Inv_Pri_AC_LL 		= -2000;//22A peak
	Norm_ADC_Limits.I_Inv_Pri_AC_UL 		= 2000; //22A peak
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Norm_Avg_Limits.I_PV_UL 				= 1622; //27A
	Norm_Avg_Limits.V_Batt_LL_H_Cut_In 		= 1190; //290V
	Norm_Avg_Limits.V_Batt_LL_L_Cut_Off 	= 1150; //280V
	Norm_Avg_Limits.V_Batt_UL_H_Cut_Off 	= 1850; //450V
	Norm_Avg_Limits.V_Batt_UL_L_Cut_In 		= 1808; //440V

	Norm_Avg_Limits.V_PV_LL_H_Cut_In 		= 603;  //110V
	Norm_Avg_Limits.V_PV_LL_L_Cut_Off 		= 550;  //100V
	Norm_Avg_Limits.V_PV_UL_H_Cut_Off 		= 2621; //460V
	Norm_Avg_Limits.V_PV_UL_L_Cut_In 		= 2507; //440V

	Norm_Avg_Limits.Batt_Temp_LL_H_Cut_In 	= 0;
	Norm_Avg_Limits.Batt_Temp_LL_L_Cut_Off 	= 0;
	Norm_Avg_Limits.Batt_Temp_UL_H_Cut_Off 	= 4096;
	Norm_Avg_Limits.Batt_Temp_UL_L_Cut_In 	= 4096;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Norm_RMS_Limits.I_Inv_Pri_AC_LL 		= 0;
	Norm_RMS_Limits.I_Inv_Pri_AC_UL 		= 1075; //12.5A RMS //set to 1075 for 100A RMS

	Norm_RMS_Limits.V_Mains_AC_LL_H_Cut_In 	= 1530;//180 V
	Norm_RMS_Limits.V_Mains_AC_LL_L_Cut_Off = 1448;//170 V
	Norm_RMS_Limits.V_Mains_AC_UL_H_Cut_Off = 2358;//280 V
	Norm_RMS_Limits.V_Mains_AC_UL_L_Cut_In 	= 2275;//270 V

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Fast_Limit_Flags.Freq 					= 1;
	Fast_Limit_Flags.I_Inv 					= 1;
	Fast_Limit_Flags.I_PV 					= 1;
	Fast_Limit_Flags.V_Batt 				= 1;
	Fast_Limit_Flags.V_Mains 				= 1;
	Fast_Limit_Flags.V_PV 					= 1;
	Fast_Limit_Flags.Grid_Failure 			= 1;
	Fast_Limit_Flags.V_Batt_For_PV 			= 1;
	Fast_Limit_Flags.Short_Circuit 			= 1;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	RMS_Limit_Flags.I_Inv 					= 1;
	RMS_Limit_Flags.V_Mains 				= 1;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Avg_Limit_Flags.Batt_Over_Temp 			= 1;
	Avg_Limit_Flags.I_PV 					= 1;
	Avg_Limit_Flags.Inv_Over_Temp 			= 1;
	Avg_Limit_Flags.V_Batt 					= 1;
	Avg_Limit_Flags.V_PV 					= 1;
	Avg_Limit_Flags.V_Batt_For_PV 			= 1;
	Avg_Limit_Flags.V_Mains_Offset_Calib_Failure = 1;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Inv_Cntrl.Count 						= 1;
	Inv_Cntrl.Duty_Cycle_1 					= 0;
	Inv_Cntrl.Duty_Cycle_2 					= 0;
	Inv_Cntrl.EOS0_Cntr_Request 			= 0;
	Inv_Cntrl.I_Peak_Ref 					= 0;
	Inv_Cntrl.I_Error_Inv 					= 0;
	Inv_Cntrl.I_Ref 						= 0;
	Inv_Cntrl.Ki_V_Batt 					= 0 ;
	Inv_Cntrl.Kp_I_Inv_Loop 				= 16384;    //1.5<<12 = 6144
	Inv_Cntrl.PerUnit 						= 4096;
	Inv_Cntrl.RMS_Sqrt_Calc_Request 		= 0;
	Inv_Cntrl.Soft_Start_Var 				= 0;
	Inv_Cntrl.Start_Ov_Temp_Cnt_Request 	= 0;
	Inv_Cntrl.State 						= 0;
	Inv_Cntrl.Time_Period_Grid 				= 0;
	Inv_Cntrl.Time_Period_Grid_Shifted 		= 0;
	Inv_Cntrl.V_Batt_Err 					= 0;
	Inv_Cntrl.V_Batt_Err_Integ 				= 0;
	Inv_Cntrl.V_Batt_ref 					= 1920;     //2765; //55V    //set to 2262 for 45V
	Inv_Cntrl.V_Delta_Generate 				= 0;
	Inv_Cntrl.V_Generate 					= 0;
	Inv_Cntrl.ZCD 							= 0;
	Inv_Cntrl.First_Time_State_Entry 		= 1;
	Inv_Cntrl.Inv_Export_Ipeak_limit 		= 2317;//2151; //set to 600 for 56.56 A peak(40A RMS)
	Inv_Cntrl.Inv_Import_Ipeak_limit 		= 2317;//2151; //set to 600 for 56.56 A peak(40A RMS)
	Inv_Cntrl.Time_Period_Grid_LL 			= 36363; //55Hz
	Inv_Cntrl.Time_Period_Grid_UL 			= 44444; //45Hz
	Inv_Cntrl.Inv_Off_to_Mains_Relay_Off_Delay_Flag = 0;
	Inv_Cntrl.V_Mains_FF 					= 0;
	Inv_Cntrl.V_Mains_Adjstr 				= 0;
	Inv_Cntrl.V_Batt_For_Mains_Adjst 		= 0;
 	Inv_Cntrl.Inverter_ON_Delay 			= 0;
	Inv_Cntrl.Inverter_ON_Delay_Rqst 		= 0;
	Inv_Cntrl.Over_Load_Cntr 				= 0;
	Inv_Cntrl.Over_Load_Cntr_Rqst 			= 0;
	Inv_Cntrl.Grid_Disconnected_Flag 		= 0;
	Inv_Cntrl.Direct_State_Entry 			= 0;
	Inv_Cntrl.I_Inv_Pri_Calib_Offset 		= 2010;
	Inv_Cntrl.I_Inv_Pri_Calib_Offset_Sum 	= 0;
	Inv_Cntrl.Calib_Successful_Flag 		= 0;

	Inv_Cntrl.Disable_Grid_Freq_Update_and_Freq_Diag=0;  // newly added
	Inv_Cntrl.In_Transition_to_Inv_Mode 	= 0;

	Inv_Cntrl.Integral_For_Sat_Detect 		= 0;
	Inv_Cntrl.Local_Load_Compensation 		= 0;

	Inv_Cntrl.Inverter_Rating 				= 3; // #KW

	Inv_Cntrl.Wave_Shift_Count 				= 5;
	Inv_Cntrl.Half_Wave_Symm_Count 			= 100;
	Inv_Cntrl.Half_Wave_Symm_Adjst 			= 100;
	Inv_Cntrl.PV_Absent 					= 0;
	Inv_Cntrl.Relay_Activation_Delay_Rqst 	= 0;
	Inv_Cntrl.V_Mains_Failure_Band_LL 		= -1130; // -116V (V_Mains_LL_Cut_Off*1.414/2)
	Inv_Cntrl.V_Mains_Failure_Band_UL 		= 1130; // 116V
	Inv_Cntrl.Inv_Off_Dueto_Low_V_Batt 		= 0;

	Inv_Cntrl.V_Mains_Calib_Offset 			= 1950;
	Inv_Cntrl.V_Mains_Calib_Offset_Sum 		= 0;

	Inv_Cntrl.Kp_V_DC_Link_Loop 			= 43008;//7<<12 = 28672
	Inv_Cntrl.Ki_V_DC_Link_Loop 			= 4096;//1<<12 = 4096

	Inv_Cntrl.Divide_1_By_5P3 				= 761; // 1/5.36->0.186 ->0.001000110111 <<12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//othr settings have to be below Norm_Avg_Setting initialization

	Othr.Batt_Temp50 						= 4096;
	Othr.Batt_Temp80 						= 4096;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Othr.Sine_Length 						= 192;
	Othr.Half_Sine_Length 					= 96;
	Othr.Fault_Reg_1 						= 0;
	Othr.Recieved_Settings_Error 			= 1;
	Othr.Recieved_Settings_Ok 				= 0;

	Othr.System_Off_Timer 					= 0;
	Othr.System_Off_Timer_Limit 			= 60000000;//X 100us
	Othr.System_Off_Timer_Limit_Cycle_Count = 0;

	Othr.V_Batt_Boost_Ref 					= 2614;     //2765; //55V    //set to 2262 for 45V;
	Othr.V_Batt_Equaliz_Ref 				= 2614;     //2765; //55V    //set to 2262 for 45V;
	Othr.V_Batt_Float_Ref 					= 2614;     //2765; //55V    //set to 2262 for 45V;
	Othr.Batt_Chg_Mode 						= 2;
	Othr.Batt_Chg_Prev_Mode 				= 1;

	Othr.Inv_Off_Latch 						= 0;
	Othr.Overload_110_percent_Count 		= 0;
	Othr.Overload_125_percent_Count 		= 0;
	Othr.Overload_150_percent_Count 		= 0;
	Othr.Overload_Count_Reset_Timer 		= 0;
	Othr.Overload_Count_Reset_Timer_Limit 	= 36000000;
	Othr.V_RMS_Inv_OV_Shutdown_Count 		= 0;
	Othr.Batt_Temp_Comp_Factor 				= 4095;
	Othr.Batt_Temp_Outof_Bounds_Flag 		= 0;

	Othr.GPRS_On_Off_Command 				= 1;

	Othr.Relay_OFF_Delay_Time 				= 200;
//		Othr.Relay_ON_Delay_Time=100;
	Othr.Relay_Activation_Delay 			= 100;
	Othr.Inv_Short_Ckt_Count 				= 0;
	Othr.Inv_Fast_I_limit_Count 			= 0;

	Othr.Noload_Shutdown_Activate 			= 0;
	Othr.I_Load_CT_NoLoad_Detect_RMS_Limit 	= 0;

	Othr.Noload_Detect_Cntr_Rqst 			= 0;
	Othr.Noload_Turnoff_to_Turnon_Delay_Cntr_Rqst = 0;
	Othr.Noload_Shutdown_Flag 				= 0;
	Othr.Previous_PowerON_Status_VbattS2 	= 0;

	Othr.V_Mains_Offset_Calib_Rqst 			= 0;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	PV_Cntrl.Delta_Duty 					= 5;// 10 implies 0.5%
	PV_Cntrl.Duty_Cycle 					= 0;
	PV_Cntrl.Duty_Dec 						= 0;
	PV_Cntrl.Duty_Inc 						= 0;
	PV_Cntrl.Duty_Sweep_Mode 				= 1;
	PV_Cntrl.Power_New 						= 0;
	PV_Cntrl.Power_Old 						= 0;
	PV_Cntrl.State 							= 0;
	PV_Cntrl.I_PV_Soft_Limit_Flag 			= 0;
	PV_Cntrl.Mppt_Loop_Time 				= 48000;  //in terms of timer interrupt count
	PV_Cntrl.V_PV_Soft_Limit 				= 2386;  //exceeding this limit will make the duty cycle decrease in PV loop
	PV_Cntrl.I_PV_Soft_Limit				= 2403;
	PV_Cntrl.First_Time_State_Entry 		= 1;
	PV_Cntrl.V_Batt_ref 					= 2941;//58.5V     //2765; //55V    //set to 2262 for 45V

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Metering.Batt_AH_In 					= 0;
	Metering.Batt_AH_Out 					= 0;
	Metering.Batt_Ampere_In_Sum 			= 0;
	Metering.Batt_Ampere_Out_Sum 			= 0;
	Metering.Export_Energy 					= 0;
	Metering.Export_KWH 					= 0;
	Metering.Import_Energy 					= 0;
	Metering.Import_KWH 					= 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Debug.A = 0;
	Debug.B = 0;
	Debug.C = 0;
	Debug.D = 0;
	Debug.E = 0;
	Debug.F = 0;
	Debug.G = 0;
	Debug.H = 0;
	Debug.I = 0;
	Debug.J = 0;

	Debug.I_Batt_ADC_Dummy_ADC 		= 0;
	Debug.Batt_Temp_Dummy_ADC 		= 0;
	Debug.I_Inv_CT_ADC_Dummy_ADC 	= 2048;
	Debug.I_Inv_pri_ADC_Dummy_ADC	= 2048;
	Debug.I_Mains_CT_ADC_Dummy_ADC	= 2048;
	Debug.Mains_Volt_ADC_Dummy_ADC	= 3148;
	Debug.V_Batt_ADC_Dummy_ADC		= 2815;
	Debug.V_Inv_ADC_Dummy_ADC		= 2048;
	Debug.V_PV_ADC_Dummy_ADC		= 2359;
	Debug.I_PV_ADC_Dummy_ADC		= 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Calibration_Temp.I_Batt 		= 2048;
	Calibration_Temp.I_Inv 			= 2048;
	Calibration_Temp.I_PV 			= 2048;
	Calibration_Temp.V_Batt 		= 1912;// Due to request from statcon (kushal) 2048*56/60
	Calibration_Temp.V_Inv 			= 2048;
	Calibration_Temp.V_Mains 		= 2048;
	Calibration_Temp.V_PV 			= 2048;
	Calibration_Temp.I_Mains_CT 	= 2048;
	Calibration_Temp.I_Inv_CT 		= 2048;

	Calibration_Var.I_Batt 			= 2050;
	Calibration_Var.I_Inv 			= 2051;
	Calibration_Var.I_PV 			= 2052;
	Calibration_Var.V_Batt 			= 1912;
	Calibration_Var.V_Inv 			= 2053;
	Calibration_Var.V_Mains 		= 2054;
	Calibration_Var.V_PV 			= 2055;
	Calibration_Var.I_Mains_CT 		= 2048;
	Calibration_Var.I_Inv_CT 		= 2048;

	Calibration_Var.Copy_To_Flash_Rqst_V_Mains_Offset=0;


}

void Copy_Calib_To_Flash()
{
	if(Calibration_Var.Copy_To_Flash_Request==1)
	{
		Flash_Erase();
		Flash_Write();
		Calibration_Var.Copy_To_Flash_Request=0;
	}

}

void Copy_Calib_From_Flash()
{
	Flash_Read();
}



void Batt_Boost_Float_Changeover()
{
  //irrespective of the mode, if the battery is discharged a bit then the charging cycle needs to start over again
	if (Norm_Avg.V_Batt<=(Othr.V_Batt_Float_Ref -(Othr.V_Batt_Float_Ref>>4)))
	{
		Othr.Batt_Chg_Mode=2;
	}

	if(Othr.Batt_Chg_Mode==2)
	{
		if(Norm_Avg.V_Batt>=(Othr.V_Batt_Boost_Ref-(Othr.V_Batt_Boost_Ref>>6)))
		{
			Othr.Batt_Boost_Mode_Timer_Count_Reqst = 1;
			if(Othr.Batt_Boost_Mode_Timer_Minutes >= Othr.Batt_Boost_Mode_Timer_Minutes_Limit)
			{
				if(Othr.Batt_Equalization_Reqst==1)
				{
					Othr.Batt_Chg_Mode=3;
					Othr.Batt_Equalization_Reqst=0;
				}
				else
				{
					Othr.Batt_Chg_Mode=1;
				}
				Othr.Batt_Chg_EOS_Timer=0;
				Othr.Batt_Chg_EOS_Timer_Count_Reqst=0;

				Othr.Batt_Boost_Mode_Timer_Count_Reqst = 0;
				Othr.Batt_Boost_Mode_Timer_Minutes = 0;
				Othr.Batt_Boost_Mode_Timer_Seconds = 0;
			}

			if(Norm_Avg.I_Batt>=(-(Othr.I_Batt_Chg_Boost_Cutoff_ref)))
			{
				Othr.Batt_Chg_EOS_Timer_Count_Reqst=1;
				if(Othr.Batt_Chg_EOS_Timer>Othr.Batt_Chg_EOS_Timer_Limit)
				{
					if(Othr.Batt_Equalization_Reqst==1)
					{
						Othr.Batt_Chg_Mode=3;
						Othr.Batt_Equalization_Reqst=0;
					}
					else
					{
						Othr.Batt_Chg_Mode=1;
					}
					Othr.Batt_Chg_EOS_Timer=0;
					Othr.Batt_Chg_EOS_Timer_Count_Reqst=0;

					Othr.Batt_Boost_Mode_Timer_Count_Reqst = 0;
					Othr.Batt_Boost_Mode_Timer_Minutes = 0;
					Othr.Batt_Boost_Mode_Timer_Seconds = 0;

				}
			}
			else
			{
				Othr.Batt_Chg_EOS_Timer=0;
				Othr.Batt_Chg_EOS_Timer_Count_Reqst=0;
			}
		}
		else
		{
			Othr.Batt_Boost_Mode_Timer_Count_Reqst = 0;
			Othr.Batt_Boost_Mode_Timer_Minutes = 0;
			Othr.Batt_Boost_Mode_Timer_Seconds = 0;

			Othr.Batt_Chg_EOS_Timer=0;
			Othr.Batt_Chg_EOS_Timer_Count_Reqst=0;
		}

	}

	if(Othr.Batt_Chg_Mode==3)
		{
		///
		if(Norm_Avg.V_Batt>=(Othr.V_Batt_Boost_Ref))
			{
					Othr.Batt_Equilization_Mode_Timer_Count_Reqst=1;
					if((Othr.Batt_Equilization_Mode_Timer_Minutes) >= (Othr.Batt_Equilization_Mode_Timer_Minutes_Limit))
					{
						Othr.Batt_Equilization_Mode_Timer_Count_Reqst=0;
						Othr.Batt_Equilization_Mode_Timer_Minutes=0;
						Othr.Batt_Equilization_Mode_Timer_Seconds=0;
						Othr.Batt_Chg_Mode=1;
					}
			}
		else
			{
				Othr.Batt_Equilization_Mode_Timer_Count_Reqst=0;
				Othr.Batt_Equilization_Mode_Timer_Minutes=0;
				Othr.Batt_Equilization_Mode_Timer_Seconds=0;
			}

		///
		if(Norm_Avg.V_Batt>=(Othr.V_Batt_Equaliz_Ref-(Othr.V_Batt_Equaliz_Ref>>6)))
		{
			Othr.Batt_Chg_EOS_Timer_Count_Reqst=1;
			if(Othr.Batt_Chg_EOS_Timer>Othr.Batt_Chg_EOS_Timer_Limit)
				{
					Othr.Batt_Chg_EOS_Timer=0;
					Othr.Batt_Chg_EOS_Timer_Count_Reqst=0;
					Othr.Batt_Chg_Mode=1;
					PV_Cntrl.V_Batt_ref=Othr.V_Batt_Float_Ref;
				}
		}
		else
		    {
					Othr.Batt_Chg_EOS_Timer=0;
					Othr.Batt_Chg_EOS_Timer_Count_Reqst=0;
		    }

		  }

	if(Othr.Batt_Chg_Mode==1)
			{
				PV_Cntrl.V_Batt_ref= ((uint32_t)(Othr.Batt_Temp_Comp_Factor * Othr.V_Batt_Float_Ref))>>12;
			}
	else if(Othr.Batt_Chg_Mode==2)
			{
				PV_Cntrl.V_Batt_ref=((uint32_t)(Othr.Batt_Temp_Comp_Factor * Othr.V_Batt_Boost_Ref))>>12;
			}
	else if(Othr.Batt_Chg_Mode==3)
			{
				PV_Cntrl.V_Batt_ref=((uint32_t)(Othr.Batt_Temp_Comp_Factor * Othr.V_Batt_Equaliz_Ref))>>12;
			}


	////
	if((Othr.Batt_Chg_Prev_Mode==1)&&(Othr.Batt_Chg_Mode==2))
	{
		Othr.Batt_Charge_Discharge_Cycle_Count=Othr.Batt_Charge_Discharge_Cycle_Count+1;
	}
	if(Othr.Batt_Charge_Discharge_Cycle_Count>=Othr.Batt_Charge_Discharge_Cycle_Count_Limit)
	{
		Othr.Batt_Equalization_Reqst=1;
		Othr.Batt_Charge_Discharge_Cycle_Count=0;
	}
	Othr.Batt_Chg_Prev_Mode=Othr.Batt_Chg_Mode;

	///

	////
}

void Grid_Batt_Boost_Float_Changeover(void)
{
  //irrespective of the mode, if the battery is discharged a bit then the charging cycle needs to start over again

	if(Othr.Grid_Batt_Chg_Mode==2)
	{
		if(Norm_Avg.V_Batt>=(Inv_Cntrl.V_Grid_Boost_Ref-(Inv_Cntrl.V_Grid_Boost_Ref>>6)))
		{
			Othr.Batt_Grid_Boost_Mode_Timer_Count_Reqst = 1;
			if((Inv_Cntrl.I_Peak_Ref >=(-(Inv_Cntrl.Inv_Import_Ipeak_limit >> 2)))||(Othr.Batt_Grid_Boost_Mode_Timer_Minutes>=Othr.Batt_Grid_Boost_Mode_Timer_Minutes_Limit)) //
			{
					Othr.Grid_Batt_Chg_Mode=1;

					Othr.Batt_Grid_Boost_Mode_Timer_Count_Reqst = 0;
					Othr.Batt_Grid_Boost_Mode_Timer_Minutes = 0;
					Othr.Batt_Grid_Boost_Mode_Timer_Seconds = 0;
			}
		}
		else
		{
			Othr.Batt_Grid_Boost_Mode_Timer_Count_Reqst = 0;
			Othr.Batt_Grid_Boost_Mode_Timer_Minutes = 0;
			Othr.Batt_Grid_Boost_Mode_Timer_Seconds = 0;
		}

		if(Othr.Batt_Chg_Mode == 1)
		{
			Othr.Grid_Batt_Chg_Mode =1;
		}
	}

	if(Othr.Grid_Batt_Chg_Mode==1)
	{
		Inv_Cntrl.V_Batt_ref= ((uint32_t)(Othr.Batt_Temp_Comp_Factor * Inv_Cntrl.V_Grid_Float_Ref))>>12;
	}
	else if(Othr.Grid_Batt_Chg_Mode==2)
	{
		Inv_Cntrl.V_Batt_ref=((uint32_t)(Othr.Batt_Temp_Comp_Factor * Inv_Cntrl.V_Grid_Boost_Ref))>>12;
	}

}


void Batt_Temp_Comp_Factor_Calculator()
{


	//Batt_Temp = 200;
 if((Inv_Temp_ADC > 176)&&(Inv_Temp_ADC < 3084))//140mV and 2500mV
	{

	 if(Inv_Temp_ADC<27*44)
	 	{
	 	Othr.Batt_Temp_Comp_Factor = 4096 + ((((uint32_t)(1188-Inv_Temp_ADC))*((uint32_t)36605))>>18);

	 		// (1/44)*(3/2000)=0.0000341

	 		//(0.0000341*4095)<<18 = 36605


	 	}
	 	else
	 	{
	 		Othr.Batt_Temp_Comp_Factor =4096 - ((((uint32_t)(Inv_Temp_ADC-1188))*((uint32_t)36605))>>18);
	 	}
	}

	else
	{
		Othr.Batt_Temp_Comp_Factor=4096;
		Othr.Batt_Temp_Outof_Bounds_Flag=1;
	}

 if((Othr.Batt_Temp_Comp_Factor>4400)||(Othr.Batt_Temp_Comp_Factor<3600))
 {
	 Othr.Batt_Temp_Comp_Factor=4096;
	 Othr.Batt_Temp_Outof_Bounds_Flag=1;
 }


}


void Generate_Fault_Data_For_Metering()
{
	// Flags_fault.pv_uv
	if(Norm_Avg.V_PV<Norm_Avg_Limits.V_PV_LL_H_Cut_In)
	{
	Othr.Fault_Reg_1|=0x1U;
	}
	else
	{
	Othr.Fault_Reg_1 &=~0x1U;
	}

	//  Flags_fault.pv_ov
	if(Norm_Avg.V_PV>Norm_Avg_Limits.V_PV_UL_L_Cut_In)
	{
	Othr.Fault_Reg_1 |=(0x1U<<1);
	}
	else
	{
		Othr.Fault_Reg_1 &=~(0x1U<<1);
	}

	//  F_Flags.System_switched_off

	if(!Switch_Is_ON)
	{
	 Othr.Fault_Reg_1 |=(0x1U<<2);
	}
	else
	{
	Othr.Fault_Reg_1 &=~(0x1U<<2);
	}


	//  F_Flags.Sleep_mode
	if(!Switch_Is_ON)
	{
	 Othr.Fault_Reg_1 |=(0x1U<<3);
	}
	else
	{
	Othr.Fault_Reg_1 &=~(0x1U<<3);
	}

	//  Flags.boost_flag /Float Flag
	if(Othr.Batt_Chg_Mode==1)
	{
	Othr.Fault_Reg_1 |=(0x1U<<4);
	}
	else
	{
		Othr.Fault_Reg_1 &=~(0x1U<<4);
	}


	if(Othr.Batt_Chg_Mode==2)
	{
	Othr.Fault_Reg_1 |=(0x1U<<5);
	}
	else
	{
		Othr.Fault_Reg_1 &=~(0x1U<<5);
	}


	if(Othr.Batt_Chg_Mode==3)
	{
	Othr.Fault_Reg_1 |=(0x1U<<6);
	}
	else
	{
		Othr.Fault_Reg_1 &=~(0x1U<<6);
	}


/*


	//  Flags_fault.Inv_on_indication

	  	if(Inv_Cntrl.State == 2)
	  	{
	  		Othr.Fault_Reg_1 |=(1<<7);
	  	}
	   else
			{
				Othr.Fault_Reg_1 &=~(1<<7);
			}

*/


	//  Flags_fault.mains_OV
	if(Norm_Rms.V_Mains>Norm_RMS_Limits.V_Mains_AC_UL_L_Cut_In)
	{
	Othr.Fault_Reg_1 |=(0x1U<<8);
	}
	else
	{
		Othr.Fault_Reg_1 &=~(0x1U<<8);
	}

	//  F_Flags.mains_UF
	if(Norm_Rms.V_Mains<Norm_RMS_Limits.V_Mains_AC_LL_H_Cut_In)
	{
	Othr.Fault_Reg_1 |=(0x1U<<9);
	}
	else
	{
		Othr.Fault_Reg_1 &=~(0x1U<<9);
	}


	//  F_Flags.batt_uv_alarm



	//  F_F_Flags.System_UNDER_Ovrload

	if(Othr.Fault_Indication_Time_Extended_I_Inv)
	{
	Othr.Fault_Reg_1 |=(0x1U<<11);
	}
	else
	{
		Othr.Fault_Reg_1 &=~(0x1U<<11);
	}

	//  F_Flags.PV_CHG_OL
	if(I_PV_Outof_Avg_Limits_UL)
	{
	Othr.Fault_Reg_1 |=(0x1U<<12);
	}
	else
	{
		Othr.Fault_Reg_1 &=~(0x1U<<12);
	}

	//  Flags_fault.temp_c_fail

	//  Flags_fault.Batt_Temp_error
	if((Inv_Temp_ADC <= 176)||(Inv_Temp_ADC  >= 3084))//<4 deg or >70 deg
	{
	Othr.Fault_Reg_1 |=(0x1U<<14);
	}
	else
	{
		Othr.Fault_Reg_1 &=~(0x1U<<14);
	}

	  //Flags_fault.batt_ov
	if(Norm_Avg.V_Batt>Norm_Avg_Limits.V_Batt_UL_L_Cut_In)
	{
	Othr.Fault_Reg_2 |=(0x1U);
	}
	else
	{
		Othr.Fault_Reg_2 &=~(0x1U);
	}


	//  Flags_fault.batt_uv
	if(Norm_Avg.V_Batt<Norm_Avg_Limits.V_Batt_LL_H_Cut_In)
	{
	Othr.Fault_Reg_2 |=(0x1U<<1);
	}
	else
	{
		Othr.Fault_Reg_2 &= ~(0x1U<<1);
	}

	  //Flags.op_ov

	//Flags.op_uv


		//Grid_Export_Flag
	if((Inv_Cntrl.State == 1)&&(Norm_ADC.Mains_Power_CT_Mean>=0))//Norm_ADC.Mains_Power_CT_Mean>>12
	{
		Othr.Fault_Reg_2 |=(0x1U<<4);
	}
	else
	{
		Othr.Fault_Reg_2 &=~(0x1U<<4);
	}

		//Grid_Import_Flag
	if(((Inv_Cntrl.State == 1)||(Inv_Cntrl.State == 3))&&(Norm_ADC.Mains_Power_CT_Mean<0))
	{
		Othr.Fault_Reg_2 |=(0x1U<<5);
	}
	else
	{
		Othr.Fault_Reg_2 &=~(0x1U<<5);
	}

	//Flags.trip
	if(Inv_Cntrl.State == 0)
	{
		Othr.Fault_Reg_2 |=(0x1U<<6);
	}
	else
	{
		Othr.Fault_Reg_2 &=~(0x1U<<6);
	}

//	Grid_Mode_on
	if(Inv_Cntrl.State == 1)
	{
		Othr.Fault_Reg_2 |=(0x1U<<7);
	}
	else
	{
		Othr.Fault_Reg_2 &=~(0x1U<<7);
	}
	//	Inv_Mode_on

	if(Inv_Cntrl.State == 2)
	{
		Othr.Fault_Reg_2 |=(0x1U<<8);
	}
	else
	{
		Othr.Fault_Reg_2 &=~(0x1U<<8);
	}

	//Grid_Export_Flag
	if((Inv_Cntrl.State == 1)&&(Inv_Cntrl.I_Peak_Ref>=0))
	{
		Othr.Fault_Reg_2 |=(0x1U<<9);
	}
	else
	{
		Othr.Fault_Reg_2 &=~(0x1U<<9);
	}

	//Grid_Import_Flag
	if((Inv_Cntrl.State == 1)&&(Inv_Cntrl.I_Peak_Ref<0))
	{
		Othr.Fault_Reg_2 |=(0x1U<<10);
	}
	else
	{
		Othr.Fault_Reg_2 &=~(0x1U<<10);
	}

	if(Othr.Noload_Shutdown_Flag)
	{
		Othr.Fault_Reg_2 |=(0x1U<<11);
	}
	else
	{
		Othr.Fault_Reg_2 &=~(0x1U<<11);
	}

	if(Othr.GPRS_On_Off_Command==0)//if inv off signal from GPRS
	{
		Othr.Fault_Reg_2 |=(0x1U<<12);
	}
	else
	{
		Othr.Fault_Reg_2 &=~(0x1U<<12);
	}
	if(Othr.Fault_Indication_Time_Extended_Short_Circuit==1)//if inv off signal from GPRS
	{
		Othr.Fault_Reg_2 |=(0x1U<<13);
	}
	else
	{
		Othr.Fault_Reg_2 &=~(0x1U<<13);
	}
	if((Othr.Fault_Indication_Time_Extended_Fast_I_Inv ==1)||(Othr.Fault_Indication_Time_Extended_I_Inv == 1))//if inv off signal from GPRS
	{
		Othr.Fault_Reg_2 |=(0x1U<<14);
	}
	else
	{
		Othr.Fault_Reg_2 &=~(0x1U<<14);
	}

		//////////////////////////////////Debug_Information
		Debug_Fast.Overall=0;
		Debug_Fast.Overall |= (Debug_Fast.A)|(Debug_Fast.B<<1)|(Debug_Fast.C<<2)|(Debug_Fast.D<<3)|(Debug_Fast.E<<4);
		Debug_Fast.Overall |= (Debug_Fast.F<<5)|(Debug_Fast.G<<6)|(Debug_Fast.H<<7)|(Debug_Fast.I<<8)|(Debug_Fast.J<<9);
		Debug_Fast.Overall |= (Debug_Fast.K<<10)|(Debug_Fast.L<<11)|(Debug_Fast.M<<12)|(Debug_Fast.M<<13)|(Debug_Fast.N<<14);

		Debug_Avg.Overall=0;
		Debug_Avg.Overall |= (Debug_Avg.A)|(Debug_Avg.B<<1)|(Debug_Avg.C<<2)|(Debug_Avg.D<<3)|(Debug_Avg.E<<4);
		Debug_Avg.Overall |= (Debug_Avg.F<<5)|(Debug_Avg.G<<6)|(Debug_Avg.H<<7)|(Debug_Avg.I<<8)|(Debug_Avg.J<<9);
		Debug_Avg.Overall |= (Debug_Avg.K<<10)|(Debug_Avg.L<<11)|(Debug_Avg.M<<12)|(Debug_Avg.M<<13)|(Debug_Avg.N<<14);


		Debug_Rms.Overall=0;
		Debug_Rms.Overall |= (Debug_Rms.A)|(Debug_Rms.B<<1)|(Debug_Rms.C<<2)|(Debug_Rms.D<<3)|(Debug_Rms.E<<4);
		Debug_Rms.Overall |= (Debug_Rms.F<<5)|(Debug_Rms.G<<6)|(Debug_Rms.H<<7)|(Debug_Rms.I<<8)|(Debug_Rms.J<<9);
		Debug_Rms.Overall |= (Debug_Rms.K<<10)|(Debug_Rms.L<<11)|(Debug_Rms.M<<12)|(Debug_Rms.M<<13)|(Debug_Rms.N<<14);

		Debug_State.Overall=0;
		Debug_State.Overall |= (Debug_State.A)|(Debug_State.B<<1)|(Debug_State.C<<2)|(Debug_State.D<<3)|(Debug_State.E<<4);
		Debug_State.Overall |= (Debug_State.F<<5)|(Debug_State.G<<6)|(Debug_State.H<<7)|(Debug_State.I<<8)|(Debug_State.J<<9);
		Debug_State.Overall |= (Debug_State.K<<10)|(Debug_State.L<<11)|(Debug_State.M<<12)|(Debug_State.M<<13)|(Debug_State.N<<14);
}

void Flash_Erase()
{
	 FLASH->CR |= FLASH_CR_PER; /* (1) */
	 FLASH->AR = 0x0800F800; /* (2) */
	 FLASH->CR |= FLASH_CR_STRT; /* (3) */
	 while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (4) */
	 {
	 /* For robust implementation, add here time-out management */
	 }
	 if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (5) */
	 {
	 FLASH->SR = FLASH_SR_EOP; /* (6)*/
	 }
	 else
	 {
	 /* Manage the error cases */
	 }
	 FLASH->CR &= ~FLASH_CR_PER; /* (7) */
}

void Flash_Erase_1()
{
	 FLASH->CR |= FLASH_CR_PER; /* (1) */
	 FLASH->AR = 0x0800FC00; /* (2) */
	 FLASH->CR |= FLASH_CR_STRT; /* (3) */
	 while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (4) */
	 {
	 /* For robust implementation, add here time-out management */
	 }
	 if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (5) */
	 {
	 FLASH->SR = FLASH_SR_EOP; /* (6)*/
	 }
	 else
	 {
	 /* Manage the error cases */
	 }
	 FLASH->CR &= ~FLASH_CR_PER; /* (7) */
}

void Flash_Write()
{

			 while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (1) */
			  {
			  /* For robust implementation, add here time-out management */
			  }
			  if ((FLASH->CR & FLASH_CR_LOCK) != 0) /* (2) */
			  {
			  FLASH->KEYR = 0x45670123; /* (3) */
			  FLASH->KEYR = 0xCDEF89AB;
			  }

			  FLASH->CR |= FLASH_CR_PG; /* (1) */

			  FLASH->SR |= FLASH_SR_PGERR;

			  uint8_t i=0;




//			  for(i=0;i<5;i++)
//			  {
//				 // if((Tipu_Sultan[i])!=(Calibration_Temporary[i]))
//				  //{
//				  //Pp = Debug_Flash_Write;
//				  *(Debug_Flash_Write + (i<<1))= Calibration_Temporary[i]; /* (2) */
//
//
//					  //*(__IO uint16_t*)(0x8006848 + i) = Calibration_Temporary[i]; /* (2) */
//					  while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (3) */
//					  { /* For robust implementation, add here time-out management */
//					  }
//
//					  if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (4) */
//					  {
//					  FLASH->SR = FLASH_SR_EOP; /* (5) */
//					  }
//					  else
//					  {
//					  /* Manage the error cases */
//					  }
//				  //}
//
//			  }

			  *(Debug_Flash_Write + (i++<<1))= 	Calibration_Var.I_Batt; /* (2) */
				//*(__IO uint16_t*)(0x8006848 + i) = Calibration_Temporary[i]; /* (2) */
				while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (3) */
				{ /* For robust implementation, add here time-out management */
				}
				if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (4) */
				{
				FLASH->SR = FLASH_SR_EOP; /* (5) */
				}
				else
				{
				/* Manage the error cases */
				}


				*(Debug_Flash_Write + (i++<<1))= 	Calibration_Var.I_Inv; /* (2) */
				//*(__IO uint16_t*)(0x8006848 + i) = Calibration_Temporary[i]; /* (2) */
				while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (3) */
				{ /* For robust implementation, add here time-out management */
				}
				if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (4) */
				{
				FLASH->SR = FLASH_SR_EOP; /* (5) */
				}
				else
				{
				/* Manage the error cases */
				}


				*(Debug_Flash_Write + (i++<<1))= 	Calibration_Var.I_PV; /* (2) */
				//*(__IO uint16_t*)(0x8006848 + i) = Calibration_Temporary[i]; /* (2) */
				while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (3) */
				{ /* For robust implementation, add here time-out management */
				}
				if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (4) */
				{
				FLASH->SR = FLASH_SR_EOP; /* (5) */
				}
				else
				{
				/* Manage the error cases */
				}


				*(Debug_Flash_Write + (i++<<1))= 	Calibration_Var.V_Batt; /* (2) */
				//*(__IO uint16_t*)(0x8006848 + i) = Calibration_Temporary[i]; /* (2) */
				while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (3) */
				{ /* For robust implementation, add here time-out management */
				}
				if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (4) */
				{
				FLASH->SR = FLASH_SR_EOP; /* (5) */
				}
				else
				{
				/* Manage the error cases */
				}


				*(Debug_Flash_Write + (i++<<1))= 	1899; /* (2) */
				//*(__IO uint16_t*)(0x8006848 + i) = Calibration_Temporary[i]; /* (2) */
				while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (3) */
				{ /* For robust implementation, add here time-out management */
				}
				if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (4) */
				{
				FLASH->SR = FLASH_SR_EOP; /* (5) */
				}
				else
				{
				/* Manage the error cases */
				}


				*(Debug_Flash_Write + (i++<<1))= 	Calibration_Var.V_Mains; /* (2) */
				//*(__IO uint16_t*)(0x8006848 + i) = Calibration_Temporary[i]; /* (2) */
				while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (3) */
				{ /* For robust implementation, add here time-out management */
				}
				if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (4) */
				{
				FLASH->SR = FLASH_SR_EOP; /* (5) */
				}
				else
				{
				/* Manage the error cases */
				}


				*(Debug_Flash_Write + (i++<<1))= 	Calibration_Var.V_PV; /* (2) */
				//*(__IO uint16_t*)(0x8006848 + i) = Calibration_Temporary[i]; /* (2) */
				while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (3) */
				{ /* For robust implementation, add here time-out management */
				}
				if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (4) */
				{
				FLASH->SR = FLASH_SR_EOP; /* (5) */
				}
				else
				{
				/* Manage the error cases */
				}

				*(Debug_Flash_Write + (i++<<1))= 	Calibration_Var.I_Mains_CT; /* (2) */
				//*(__IO uint16_t*)(0x8006848 + i) = Calibration_Temporary[i]; /* (2) */
				while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (3) */
				{ /* For robust implementation, add here time-out management */
				}
				if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (4) */
				{
				FLASH->SR = FLASH_SR_EOP; /* (5) */
				}
				else
				{
				/* Manage the error cases */
				}

				*(Debug_Flash_Write + (i++<<1))= 	Calibration_Var.I_Inv_CT; /* (2) */
				//*(__IO uint16_t*)(0x8006848 + i) = Calibration_Temporary[i]; /* (2) */
				while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (3) */
				{ /* For robust implementation, add here time-out management */
				}
				if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (4) */
				{
				FLASH->SR = FLASH_SR_EOP; /* (5) */
				}
				else
				{
				/* Manage the error cases */
				}


		FLASH->CR &= ~FLASH_CR_PG; /* (6) */

}

void Flash_Write_1()
{

			 while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (1) */
			  {
			  /* For robust implementation, add here time-out management */
			  }
			  if ((FLASH->CR & FLASH_CR_LOCK) != 0) /* (2) */
			  {
			  FLASH->KEYR = 0x45670123; /* (3) */
			  FLASH->KEYR = 0xCDEF89AB;
			  }

			  FLASH->CR |= FLASH_CR_PG; /* (1) */

			  FLASH->SR |= FLASH_SR_PGERR;

			  uint8_t i=0;

			  *(Debug_Flash_Write_1 + (i++<<1)) = Inv_Cntrl.V_Mains_Calib_Offset; /* (2) */
				//*(__IO uint16_t*)(0x8006848 + i) = Calibration_Temporary[i]; /* (2) */
				while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (3) */
				{ /* For robust implementation, add here time-out management */
				}
				if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (4) */
				{
				FLASH->SR = FLASH_SR_EOP; /* (5) */
				}
				else
				{
				/* Manage the error cases */
				}

		FLASH->CR &= ~FLASH_CR_PG; /* (6) */

}



void Flash_Read()
{
	uint8_t i=0;

	Calibration_Var.I_Batt 		= *(Debug_Flash_Write + (i++<<1));
	Calibration_Var.I_Inv 		= *(Debug_Flash_Write + (i++<<1));
	Calibration_Var.I_PV 		= *(Debug_Flash_Write + (i++<<1));
	Calibration_Var.V_Batt 		= *(Debug_Flash_Write + (i++<<1));
	Calibration_Var.V_Inv 		= *(Debug_Flash_Write + (i++<<1));
	Calibration_Var.V_Mains 	= *(Debug_Flash_Write + (i++<<1));
	Calibration_Var.V_PV 		= *(Debug_Flash_Write + (i++<<1));
	Calibration_Var.I_Mains_CT = *(Debug_Flash_Write + (i++<<1));
	Calibration_Var.I_Inv_CT 	= *(Debug_Flash_Write + (i++<<1));

	i = 0;
	Inv_Cntrl.V_Mains_Calib_Offset 	= *(Debug_Flash_Write_1 + (i++<<1));
	if((Calibration_Var.I_Batt > 2457)||(Calibration_Var.I_Batt <1638))
		{
		Calibration_Var.I_Batt = 2048;
		}
	if((Calibration_Var.I_Inv > 2457)||(Calibration_Var.I_Inv <1638))
		{
		Calibration_Var.I_Inv = 2048;
		}
	if((Calibration_Var.I_PV > 2457)||(Calibration_Var.I_PV <1638))
		{
		Calibration_Var.I_PV = 2048;
		}
	if((Calibration_Var.V_Batt > 2457)||(Calibration_Var.V_Batt <1638))
		{
		Calibration_Var.V_Batt = 2048;
		}
	if((Calibration_Var.V_Inv > 2457)||(Calibration_Var.V_Inv <1638))
		{
		Calibration_Var.V_Inv = 2048;
		}
	if((Calibration_Var.V_Mains > 2457)||(Calibration_Var.V_Mains <1638))
		{
		Calibration_Var.V_Mains = 2048;
		}
	if((Calibration_Var.V_PV > 2457)||(Calibration_Var.V_PV <1638))
		{
		Calibration_Var.V_PV = 2048;
		}
	if((Calibration_Var.I_Mains_CT > 2457)||(Calibration_Var.I_Mains_CT <1638))
		{
		Calibration_Var.I_Mains_CT = 2048;
		}
	if((Calibration_Var.I_Inv_CT > 2457)||(Calibration_Var.I_Inv_CT <1638))
		{
		Calibration_Var.I_Inv_CT = 2048;
		}

	if((Inv_Cntrl.V_Mains_Calib_Offset > 2457)||(Inv_Cntrl.V_Mains_Calib_Offset <1638))
		{
		Inv_Cntrl.V_Mains_Calib_Offset = 2000;
		}
}

void Local_Load_Compensation_if_Exprt_Limit_is_Zero()
{
	/*
	 * At 230V RMS => 2243 is normalized mains ADC reading
	 * At 200% load CT generates 3V peak at the controller pin => it generates 3-1.65 = 1.35V
	 * For 100% load => 1.35/2 = 0.675 peak
	 * Then equivalent RMS for 0.675 peak is 0.675/1.414 = 0.477
	 * Normalized mains CT ADC reading will be = 0.477 * 4096 / 3.3 = 592
	 * So at full load Mains power CT mean = 592 * 2243 = 1329014
	 * 1% of the full load power = 13290
	 */
	if(Inv_Cntrl.Inv_Export_Ipeak_limit <= 10)
	{
		if(Norm_ADC.Mains_Power_CT_Mean > 13290) // 1% full load
		{
			if(Inv_Cntrl.Local_Load_Compensation >= 2 )
			{
				Inv_Cntrl.Local_Load_Compensation = Inv_Cntrl.Local_Load_Compensation - 2;
			}
			else
			{
				Inv_Cntrl.Local_Load_Compensation = 0;
			}
		}
		else if(Norm_ADC.Mains_Power_CT_Mean < -13290) // 1% full load
		{
			Inv_Cntrl.Local_Load_Compensation = Inv_Cntrl.Local_Load_Compensation + 1;
			if(Inv_Cntrl.Local_Load_Compensation >= 1782) // 1782 indicates 100% load
			{
				Inv_Cntrl.Local_Load_Compensation = 1782;
			}
		}
	}
	else
	{
		Inv_Cntrl.Local_Load_Compensation = 0;
	}

}

void Power_Factor_Calculation()
{
	Norm_Rms.Apparent_Power = (Norm_Rms.V_Mains *Norm_Rms.I_Inv_Pri );

	if(Norm_Rms.Apparent_Power != 0)
	{
		if(Norm_ADC.Mains_Power_Mean >= 0)
		{
			Inv_Cntrl.Power_Factor =  ((((int64_t)Norm_ADC.Mains_Power_Mean)<<11)/Norm_Rms.Apparent_Power);
		}
		else
		{
			Inv_Cntrl.Power_Factor =  (((- ((int64_t)Norm_ADC.Mains_Power_Mean))<<11)/Norm_Rms.Apparent_Power);
		}
	}
}

void PV_Power_Unavailable_Detection()
{
	if(Inv_Cntrl.State == 2)
	{
		if((Norm_Avg.V_Batt <= (PV_Cntrl.V_Batt_ref - (PV_Cntrl.V_Batt_ref>>5)))&&(PV_Cntrl.Power_Old < 101100)) // 101100 => 100W of solar power
		{
			Cntr.PV_Absent = Cntr.PV_Absent+1;
			if(Cntr.PV_Absent >= 45000)
			{
				Cntr.PV_Absent = 45000;
			}
		}
		else
		{
			Cntr.PV_Absent = 0;
		}
	}
}

//void Debug_Data_Storage_Fn()
//{
//	if(UART1_Send_Flags.Send_Enable!=1)
//	{
//		Debug_Array[Inv_Cntrl.Count - 1] = Inv_Cntrl.Count;
//
//		if(Inv_Cntrl.Count >= 192)
//		{
//			UART1_Msg_Send_Reqt_Flags.Msg_g = 1;
//		}
//	}
//}
