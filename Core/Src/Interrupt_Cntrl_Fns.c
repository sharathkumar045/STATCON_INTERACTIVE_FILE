/*
 * Interrupt_Cntrl_Fns.c
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

void Inv_State_0_Int_Cntrl()
{
	if (Inv_Cntrl.First_Time_State_Entry != 0)
	{
		Cntr.Relay_Activation_Delay 			= 0;
		Inv_Cntrl.Relay_Activation_Delay_Rqst 	= 0;

		Inv_Cntrl.Count 						= 1;
		Inv_Cntrl.Duty_Cycle_1 					= 0;
		Inv_Cntrl.Duty_Cycle_2 					= 0;
		Inv_Cntrl.I_Error_Inv 					= 0;
		Inv_Cntrl.I_Peak_Ref 					= 0;
		Inv_Cntrl.I_Ref 						= 0;
		Inv_Cntrl.Soft_Start_Var 				= 0;
		Inv_Cntrl.State 						= 0;
		Inv_Cntrl.V_Batt_Err 					= 0;
		Inv_Cntrl.V_Batt_Err_Integ 				= 0;
		Inv_Cntrl.V_Delta_Generate 				= 0;
		Inv_Cntrl.V_Generate 					= 0;
		Inv_Cntrl.Time_Period_Grid_Shifted 		= 1249;
		TIM1->ARR 								= 1249;
		Inv_Cntrl.Synchronizing_Var 			= 0;

		Inv_Cntrl.V_Batt_ref 					= 1920; //400V
		Inv_Cntrl.Kp_V_DC_Link_Loop 			= 28672;//7<<12 = 28672
		Inv_Cntrl.Ki_V_DC_Link_Loop 			= 4;//1<<12 = 4096
		Inv_Cntrl.I_Inv_Pri_Calib_Offset 		= 2010;
		Inv_Cntrl.I_Inv_Pri_Calib_Offset_Sum 	= 0;

		Inv_Cntrl.I_Mains_CT_Calib_Offset 		= 2048;
		Inv_Cntrl.I_Mains_CT_Calib_Offset_Sum 	= 0;

		Inv_Cntrl.I_Inv_CT_Calib_Offset 		= 2048;
		Inv_Cntrl.I_Inv_CT_Calib_Offset_Sum 	= 0;
		Inv_Cntrl.V_Mains_Calib_Offset 			= 2048;

		Inv_Cntrl.Earth_Fault_Calib_Offset      = 2048;

		Cntr.Inv_EOS0_1 						= 0;
		Cntr.ZCD_Diag_Delay 					= 0;
		Cntr.Diag_delay 						= 0;
		Cntr.ZCD_Error 							= 0;
		Cntr.Calibration_Avg 					= 0;
		Cntr.Calibration_Rqst 					= 0;

		Inv_Cntrl.First_Time_State_Entry 		= 0;
		Inv_Cntrl.Direct_State_Entry 			= 0;

	}

	Inverter_OFF();
	//MPPT_OFF();
	Switch_Off_Mains_Relay();
	Mains_Relay_Off;

	if (V_Batt_Inside_Avg_Limits_Cutin)
	{
		Fast_Limit_Flags.V_Batt = 0;
		Avg_Limit_Flags.V_Batt = 0;
		Debug_Grid_Tie.B = 0;
	}
	else
	{
		Debug_Grid_Tie.B = 1;
	}

	if (V_Mains_Inside_RMS_Limits_Cutin) //  this if statement checks if Mains_Ok signal and Voltage is in limit then mainsvolt_limit_flag is set to zero other wise set to 1
	{
		Fast_Limit_Flags.V_Mains = 0;
		Fast_Limit_Flags.Grid_Failure = 0;
		RMS_Limit_Flags.V_Mains = 0;
		Debug_Grid_Tie.C = 0;
	}
	else
	{
		Debug_Grid_Tie.C = 1;
	}

	if (Freq_In_limits)
	{
		Fast_Limit_Flags.Freq = 0;
		Debug.B = 0;
		Debug_Grid_Tie.D = 0;
	}
	else
	{
		Debug_Grid_Tie.D = 1;
	}

//	if(Othr.V_Mains_Offset_Calib_Rqst == 1)  //This rqst is sent through settings
//	{
//		Cntr.V_Mains_Offset_Calib_Rqst = Cntr.V_Mains_Offset_Calib_Rqst + 1;
//		if(Cntr.V_Mains_Offset_Calib_Rqst >= Half_Second_Count)
//		{
//			Cntr.V_Mains_Offset_Calib_Rqst = Half_Second_Count;
//		}
//		if(Cntr.V_Mains_Offset_Calib_Rqst >= Half_Second_Count)
//		{
//			if((Mains_Volt_ADC >1900)&&(Mains_Volt_ADC<2200))
//			{
//				Cntr.V_Mains_Offset_Calib_Avg = Cntr.V_Mains_Offset_Calib_Avg + 1;
//				Inv_Cntrl.V_Mains_Calib_Offset_Sum = Inv_Cntrl.V_Mains_Calib_Offset_Sum + Mains_Volt_ADC;
//				if(Cntr.V_Mains_Offset_Calib_Avg >= 16)
//				{
//					Inv_Cntrl.V_Mains_Calib_Offset = Inv_Cntrl.V_Mains_Calib_Offset_Sum>>4;
//					Inv_Cntrl.V_Mains_Calib_Offset_Sum = 0;
//					Cntr.V_Mains_Offset_Calib_Avg = 0;
//					Cntr.V_Mains_Offset_Calib_Rqst = 0;
//					Othr.V_Mains_Offset_Calib_Rqst = 0;
//					Calibration_Var.Copy_To_Flash_Rqst_V_Mains_Offset=1;
//					Avg_Limit_Flags.V_Mains_Offset_Calib_Failure = 0;
//				}
//			}
//			else
//			{
//				Inv_Cntrl.V_Mains_Calib_Offset_Sum = 0;
//				Cntr.V_Mains_Offset_Calib_Avg = 0;
//				Cntr.V_Mains_Offset_Calib_Rqst = 0;
//				Othr.V_Mains_Offset_Calib_Rqst = 0;
//				Calibration_Var.Copy_To_Flash_Rqst_V_Mains_Offset=0;
//				Avg_Limit_Flags.V_Mains_Offset_Calib_Failure = 1;
//			}
//		}
//	}

//	current calibration starts here

	Cntr.Calibration_Rqst = Cntr.Calibration_Rqst + 1;
	if(Cntr.Calibration_Rqst >= One_Second_Count)
	{
		Cntr.Calibration_Rqst = One_Second_Count;
	}

	if(Cntr.Calibration_Rqst >=One_Second_Count)
	{
		if(((I_Inv_pri_ADC > 1800)&&(I_Inv_pri_ADC < 2200))/*&&((I_Mains_CT_ADC > 1900)&&(I_Mains_CT_ADC < 2200))*/)
		{
			Cntr.Calibration_Avg = Cntr.Calibration_Avg + 1;
			Inv_Cntrl.I_Inv_Pri_Calib_Offset_Sum = Inv_Cntrl.I_Inv_Pri_Calib_Offset_Sum + I_Inv_pri_ADC;
//			Inv_Cntrl.I_Mains_CT_Calib_Offset_Sum = Inv_Cntrl.I_Mains_CT_Calib_Offset_Sum + I_Mains_CT_ADC;

			if(Cntr.Calibration_Avg >= 16)
			{
				Inv_Cntrl.I_Inv_Pri_Calib_Offset = Inv_Cntrl.I_Inv_Pri_Calib_Offset_Sum>>4;

//				Inv_Cntrl.I_Mains_CT_Calib_Offset = Inv_Cntrl.I_Mains_CT_Calib_Offset_Sum>>4;

				Inv_Cntrl.Calib_Successful_Flag = 1;

				Cntr.Calibration_Avg = 0;
				Cntr.Calibration_Rqst = 0;
				Inv_Cntrl.I_Inv_Pri_Calib_Offset_Sum = 0;
				Inv_Cntrl.I_Mains_CT_Calib_Offset_Sum = 0;
				Debug_Grid_Tie.E = 0;
			}
		}
		else
		{
			Cntr.Calibration_Avg = 0;
			Cntr.Calibration_Rqst = 0;
			Inv_Cntrl.I_Inv_Pri_Calib_Offset_Sum = 0;
			Inv_Cntrl.I_Mains_CT_Calib_Offset_Sum = 0;
			Debug_Grid_Tie.E = 1;
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/***************************State changing code*****************************/
	if ((!Fast_Limit_Flags.V_Mains)
	&& (!RMS_Limit_Flags.V_Mains)
	&& (!Fast_Limit_Flags.Freq)
	&& (Othr.Inv_Off_Latch==0)
	&& (!Fast_Limit_Flags.V_Batt)
	&& (!Avg_Limit_Flags.V_Batt)
	&& (Inv_Cntrl.Calib_Successful_Flag))
	{
		Cntr.Inv_EOS0_1 = Cntr.Inv_EOS0_1 + 1;              // end of state 0 to 1 counter
		if (Cntr.Inv_EOS0_1 >= One_Second_Count) // this counter introduces 5 seconds delay before going to state 1
		{

			Inv_Cntrl.State = 1;
			Inv_Cntrl.First_Time_State_Entry = 1;
			Cntr.Inv_EOS0_1 = 0;
			Inv_Cntrl.Calib_Successful_Flag = 0;
		}
	}

	else
	{
		Cntr.Inv_EOS0_1 = 0;
	}

	if ((Inv_Cntrl.ZCD == 1)&&(Fast_Limit_Flags.Freq==0)) // Initializing the count to zero at every zero crossing  wr Zero_cross_value
	{
		Inv_Cntrl.ZCD = 0;
		Inv_Cntrl.Count = Inv_Cntrl.Wave_Shift_Count;
		Update_Sine_Freq_To_Grid_Freq();
	}
}

void Inv_State_1_Int_Cntrl()
{
	if (Inv_Cntrl.First_Time_State_Entry == 1) // initialize only the required variables
	{
//		Cntr.PV_Ref_Soft = 0;
		Inv_Cntrl.V_Batt_Err_Integ=0;

		if (Inv_Cntrl.Count == 1)
		{
			Inv_Cntrl.Relay_Activation_Delay_Rqst = 1;
		}
		if (Inv_Cntrl.Relay_Activation_Delay_Rqst == 1)
		{
			Cntr.Relay_Activation_Delay = Cntr.Relay_Activation_Delay + 1;
		}

		Othr.Relay_Activation_Delay = 0; //remove later on 90deg

		if (Cntr.Relay_Activation_Delay >= (Othr.Relay_Activation_Delay))//// add 48  for relay turn on at 90 deg
		{
			Cntr.Relay_Activation_Delay = 0;
			Inv_Cntrl.Relay_Activation_Delay_Rqst = 0;

			Switch_On_Mains_Relay();  //inverter relay is switched on
			Inv_Cntrl.Disable_Grid_Freq_Update_and_Freq_Diag = 1;
			Inv_Cntrl.Diag_Delay = 1; //stops diagnostics for 10 seconds
			Inv_Cntrl.Local_Load_Compensation = 0;
		}

		if(Cntr.ZCD_Diag_Delay>=Ten_Second_Count)    // introducing introducing a delay of 1second
		{
			Cntr.ZCD_Diag_Delay = 0;
			Inv_Cntrl.First_Time_State_Entry = 0;
			Inv_Cntrl.Disable_Grid_Freq_Update_and_Freq_Diag = 0;
			Inv_Cntrl.V_Batt_Err = 0;
			Inv_Cntrl.I_Peak_Ref = 0;

//			Inv_Cntrl.PV_ref = V_PV_ADC - 20;

		}
	}

	if (Inv_Cntrl.ZCD == 1) // Initializing the count to zero at every zero crossing  wr Zero_cross_value
	{
		Inv_Cntrl.ZCD = 0;
		Inv_Cntrl.Count = Inv_Cntrl.Wave_Shift_Count;
		Anti_Islanding_Update_Sine_Freq_To_Grid_Freq();
	}

	    /////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (Inv_Cntrl.First_Time_State_Entry == 0)
	{
		if(Inv_Cntrl.Count == Inv_Cntrl.Half_Wave_Symm_Count)
		{
			Inv_Cntrl.Count = Inv_Cntrl.Half_Wave_Symm_Adjst;
			/*
			 * As Delta_Time_Period is 20 which means 320us
			 * 1250 implies 20ms hence 20 implies 320us which is nothing but 3 counts
			 * Since interrupt comes slower than actual freq, our count reaches only 189 in every cycle instead of 192.
			 * In half grid cycle our count reaches 94.5 => 94, now when count reaches exactly Wave_Shift_Count + 94,
			 * we need to advance the count by (192-189)/2 = 1.
			 */
		}
		Inverter_ON();

		Inv_Cntrl.V_Batt_ref = V_Batt_390V;
		Inv_Cntrl.V_Batt_Err = (V_DC_Link_ADC - Inv_Cntrl.V_Batt_ref); //minimize hardware rc filter delays when sensing V_Batt_ADC

//		Inv_Cntrl.V_Batt_Err = (V_PV_ADC - Inv_Cntrl.V_Batt_ref); //minimize hardware rc filter delays when sensing V_Batt_ADC

		if(Inv_Cntrl.V_Batt_Err > V_Batt_Error_45V)
		{
			Inv_Cntrl.V_Batt_Err = V_Batt_Error_45V;
		}

		if(Inv_Cntrl.V_Batt_Err < -(V_Batt_Error_45V))
		{
			Inv_Cntrl.V_Batt_Err = -V_Batt_Error_45V;
		}

		Inv_Cntrl.V_Loop_Proportional_OP = (Inv_Cntrl.V_Batt_Err * Inv_Cntrl.Kp_V_DC_Link_Loop)>>12;

		if (Inv_Cntrl.V_Batt_Err >= V_Batt_2V)
		{
			Inv_Cntrl.V_Batt_Err_Integ = ((Inv_Cntrl.V_Batt_Err * Inv_Cntrl.Ki_V_DC_Link_Loop)) + Inv_Cntrl.V_Batt_Err_Integ;
		}

		if (Inv_Cntrl.V_Batt_Err <= -(V_Batt_2V))
		{
			Inv_Cntrl.V_Batt_Err_Integ = ((Inv_Cntrl.V_Batt_Err * Inv_Cntrl.Ki_V_DC_Link_Loop)) + Inv_Cntrl.V_Batt_Err_Integ;
		}

		if(Inv_Cntrl.V_Batt_Err <= -(V_Batt_10V))
		{
			Inv_Cntrl.V_Batt_Err_Integ = 0;
		}

		if(Norm_Rms.I_Inv_Pri >= 840)
		{
			Inv_Cntrl.V_Batt_Err_Integ = Inv_Cntrl.V_Batt_Err_Integ - 1;
		}
		Batt_V_Err_Integ_Saturator_Code(); // this function contains the code which limits the Inv_Cntrl.V_Batt_Err_Integ value to inverter peak limit

		Inv_Cntrl.I_Peak_Ref = Inv_Cntrl.V_Batt_Err_Prop_Integ;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Inv_Cntrl.I_Ref = (Inv_Cntrl.I_Peak_Ref * Sine_PWM[Inv_Cntrl.Count - 1])>> 12; // multiplying the peak  value with sine reference  : 2000 gives 2000 value

		Inv_Cntrl.I_Error_Inv = (Inv_Cntrl.I_Ref - Norm_ADC.I_Inv_Pri_AC); //  comparing the reference current with inverter current

		Inv_Cntrl.V_Delta_Generate = ((Inv_Cntrl.I_Error_Inv * (int32_t) Inv_Cntrl.Kp_I_Inv_Loop) >> 12); // voltage generated after the current loop

		V_Delta_Generate_Saturator_Code();   // limiting V_Delta_Generate to 1000
		Inv_Cntrl.V_Delta_Generate=Inv_Cntrl.V_Delta_Generate>>3;


		Inv_Cntrl.V_Batt_For_Mains_Adjst = V_DC_Link_ADC;
		if(V_DC_Link_ADC<V_Batt_ADC_310V) // 1508 -> 30V
		{
		 Inv_Cntrl.V_Batt_For_Mains_Adjst = V_Batt_ADC_310V;
		}

		Inv_Cntrl.V_Mains_Adjstr =  (Norm_V_Batt_450V_Lsh_12/Inv_Cntrl.V_Batt_For_Mains_Adjst);

		Inv_Cntrl.V_Mains_FF = ( ( (Norm_ADC.V_Mains_AC * Inv_Cntrl.Divide_1_By_5P3)>>12 )* Inv_Cntrl.V_Mains_Adjstr) >> 12;

		Inv_Cntrl.V_Generate =	(Inv_Cntrl.V_Mains_FF + Inv_Cntrl.V_Delta_Generate); // we are scaling by 0.5 so that we can limit the Inv_Cntrl.V_Generate to fit into duty_cycle values

		V_Generate_Saturator_Code(); // this function has the code which limits the Inv_Cntrl.V_Generate value to V_Peak_Inverter_Capability

		Inv_Cntrl.Inv_PWM_50_Percent = PWM_50_percnt;

		if(Inv_Cntrl.V_Generate > Inv_Cntrl.Inv_PWM_50_Percent)
		{
			Inv_Cntrl.V_Generate = Inv_Cntrl.Inv_PWM_50_Percent;
		}
		if(Inv_Cntrl.V_Generate < -(Inv_Cntrl.Inv_PWM_50_Percent))
		{
			Inv_Cntrl.V_Generate = -Inv_Cntrl.Inv_PWM_50_Percent;
		}

		if(Inv_Cntrl.V_Generate >= 0)
		{
			Inv_Cntrl.Duty_Cycle_1 = (Inv_Cntrl.V_Generate)<<1;
			Inv_Cntrl.Duty_Cycle_2 = 0;
		}
		else
		{
			Inv_Cntrl.Duty_Cycle_1 = 0;
			Inv_Cntrl.Duty_Cycle_2 = -((Inv_Cntrl.V_Generate)<<1);
		}

		if(Inv_Cntrl.I_Peak_Ref >= 65)
		{
			Inverter_ON();
			TIM1->BDTR |= TIM_BDTR_MOE; //Main   OC enable MOE bit
		}
		else
		{
			Inv_Cntrl.I_Peak_Ref = 0;
			Inverter_OFF();
			TIM1->BDTR &= ~TIM_BDTR_MOE; //Main   OC enable MOE bit
		}

//		Inv_Cntrl.Duty_Cycle_1 = ((PWM_50_percnt) + (Inv_Cntrl.V_Generate));
//
//		Inv_Cntrl.Duty_Cycle_2 = ((PWM_50_percnt) - (Inv_Cntrl.V_Generate));

		TIM1->CCR2 = Inv_Cntrl.Duty_Cycle_1; // these two lines denote the values to be send to the  CCR register

		TIM1->CCR3 = Inv_Cntrl.Duty_Cycle_2;
	}
}

void PV_State_0_Int_Cntrl_Fn()
{
	if (PV_Cntrl.First_Time_State_Entry != 0)
	{
		PV_Cntrl.Delta_Duty 	        = 1; //refers to 1V
		PV_Cntrl.Duty_Cycle 	        = 0;
		PV_Cntrl.Duty_Dec 		        = 0;
		PV_Cntrl.Duty_Inc 		        = 0;
		PV_Cntrl.Duty_Sweep_Mode        = 1;
		PV_Cntrl.Power_New 		        = 0;
		PV_Cntrl.Power_Old 		        = 0;
		PV_Cntrl.State 			        = 0;
		PV_Cntrl.I_PV_Soft_Limit_Flag   = 0;
		PV_Cntrl.First_Time_State_Entry = 0;
		Cntr.Mppt_Loop 			        = 0;
		Cntr.PV_EOS0 			        = 0;
	}
	MPPT_OFF();

	if (V_PV_Inside_Avg_Limits_Cutin)
	{
		Fast_Limit_Flags.V_PV = 0;
		Avg_Limit_Flags.V_PV = 0;
	}

	if (V_Batt_Inside_Avg_Limits_Cutin)
	{
		Fast_Limit_Flags.V_Batt = 0;
		Avg_Limit_Flags.V_Batt = 0;
		Debug_Grid_Tie.B = 0;
	}

//	if ((!Fast_Limit_Flags.V_PV) && (!Avg_Limit_Flags.V_PV) && (Fast_Limit_Flags.V_Batt) && (Avg_Limit_Flags.V_Batt))
//	{
		Cntr.PV_EOS0 = Cntr.PV_EOS0 + 1;            // end of state 0  counter
		if (Cntr.PV_EOS0 >= Five_Second_Count) // this counter introduces 5 seconds delay before going to state 1
		{
			PV_Cntrl.State = 1;
			PV_Cntrl.First_Time_State_Entry = 1;
			Cntr.PV_EOS0 = 0;
		}
//	}
//	else
//	{
//		Cntr.PV_EOS0 = 0;
//	}

}

void PV_State_1_Int_Cntrl_Fn()
{
	if (PV_Cntrl.First_Time_State_Entry != 0)
	{
		PV_Cntrl.First_Time_State_Entry = 0;
		MPPT_ON();

		TIM14->CCR1 = ((V_PV_ADC>>2) - (V_PV_ADC>>3));
	}

	if (Cntr.Mppt_Loop >= Two_Hundred_ms_Count)      // for every 200 milli second
	{
		Cntr.Mppt_Loop = 0;
		PV_Cntrl.Power_New = Norm_ADC.Mains_Power_Mean;
		if (PV_Cntrl.Duty_Sweep_Mode == 1) // 1 means previously we have  incremented the duty cycle and ; 0 means we have decremented the duty cycle ; some other number means nothing has been done previously
		{
			if (PV_Cntrl.Power_New >= PV_Cntrl.Power_Old)
			{
				PV_Cntrl.Duty_Inc = 1;
				PV_Cntrl.Duty_Dec = 0;
			}
			else
			{
				PV_Cntrl.Duty_Inc = 0;
				PV_Cntrl.Duty_Dec = 1;
			}
		}
		else if (PV_Cntrl.Duty_Sweep_Mode == 0)
		{
			if (PV_Cntrl.Power_New >= PV_Cntrl.Power_Old)
			{
				PV_Cntrl.Duty_Inc = 0;
				PV_Cntrl.Duty_Dec = 1;
			}
			else
			{
				PV_Cntrl.Duty_Inc = 1;
				PV_Cntrl.Duty_Dec = 0;
			}
		}

		PV_Cntrl.Power_Old = PV_Cntrl.Power_New;

		if(MPPT_PWM_Channel<=(175))//if duty cycle too less
		{
			PV_Cntrl.Duty_Inc = 1;
			PV_Cntrl.Duty_Dec = 0;
		}

		if(MPPT_PWM_Channel >= (400)) //if duty >90%
		{
			PV_Cntrl.Duty_Inc = 0;
			PV_Cntrl.Duty_Dec = 1;
		}

		// please do not change the sequence of the next two function calls below
			PV_Duty_Dec_Code();

			PV_Duty_Inc_Code();
	}
}

