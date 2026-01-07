/*
 * New_Struct_Variables.h
 *
 *  Created on: Apr 30, 2019
 *      Author: krishna
 */

#ifndef NEW_STRUCT_VARIABLES_H_
#define NEW_STRUCT_VARIABLES_H_

struct Intant_ADC{
	__IO int32_t I_Inv_Pri_AC;          //   +/-4096
	__IO int32_t V_Mains_AC;
	__IO int32_t V_Inv_AC;
	__IO int32_t I_Mains_CT;
	__IO int64_t Mains_Power_Sum;
	__IO int32_t Mains_Power_Mean;
	__IO int64_t Mains_Power_CT_Sum;
	__IO int32_t Mains_Power_CT_Mean;
	__IO int64_t Inv_Power_Sum;
	__IO int32_t Inv_Power_Mean;
	__IO int32_t Earth_Fault;
	};
extern struct Intant_ADC Norm_ADC;

struct Avg_Values{
	__IO uint32_t V_PV;
	__IO uint32_t I_PV;
	__IO uint32_t V_Batt;
	__IO int32_t I_Batt;      //  this can be +/-
	__IO uint32_t V_PV_Sum;
	__IO uint32_t I_PV_Sum;
	__IO uint32_t V_Batt_Sum;
	__IO int32_t I_Batt_Sum;
	__IO uint32_t Multiplier;      // this is (1/192)<<18 = 1365
	__IO uint32_t Power_Multiplier;
	__IO uint16_t Batt_Temperature;
};
extern struct Avg_Values  Norm_Avg;

struct RMS_Values{
	__IO int32_t I_Inv_Pri;
	__IO int32_t V_Mains;
	__IO uint32_t V_Inv;
	__IO int32_t I_Mains_CT;
	__IO uint32_t I_Inv_Pri_Sq_Sum;
	__IO uint32_t I_Mains_CT_Sq_Sum;
	__IO uint32_t V_Mains_Sq_Sum;
	__IO uint32_t V_Inv_Sq_Sum;
	__IO uint64_t I_Inv_Pri_Mean_Sq;
	__IO uint64_t I_Mains_CT_Mean_Sq;
	__IO uint64_t V_Mains_Mean_Sq;
	__IO uint32_t V_Inv_Mean_Sq;
	__IO uint64_t Apparent_Power;
	__IO int32_t Earth_Fault;
	__IO uint32_t Earth_Fault_Sq_Sum;
	__IO uint64_t Earth_Fault_Mean_Sq;
};
extern struct RMS_Values Norm_Rms;

struct Counters{
	__IO uint32_t ADC_Avg;     // changed from PV_Avg cntr
	__IO uint32_t Mean_Sq_Sum;
	__IO uint32_t Mppt_Loop;
	__IO uint32_t Inv_EOS0_1;
	__IO uint32_t PV_EOS0;
	__IO uint32_t Over_Temp;
	__IO uint32_t Inv_Soft_Start;
	__IO uint32_t Inv_Off_to_Mains_Relay_Off_Delay;
	__IO uint32_t ZCD_Diag_Delay;
	__IO uint32_t ZCD_Error;
	__IO uint32_t Diag_delay;
	__IO uint32_t Watt_mSec;
	__IO uint32_t Watt_Sec;
	__IO uint32_t Watt_Min;
	__IO uint32_t Watt_Hour;
	__IO uint32_t Calibration_Avg;
	__IO uint32_t Grid_To_Inv_Off_Delay;
	__IO uint16_t Tfr_Saturation_Detect;
	__IO uint16_t PV_Absent;
	__IO uint32_t Inv_Shortckt_Count_Reset_Counter;
	__IO uint32_t Inv_Fast_I_limit_Count_Reset_Counter;
	__IO uint16_t Relay_Activation_Delay;
	__IO uint16_t Grid_Failure_Detection;
	__IO uint32_t Noload_Detect;
	__IO uint32_t Noload_Turnoff_to_Turnon_Delay;
	__IO uint16_t Calibration_Rqst;

	__IO uint16_t V_Mains_Offset_Calib_Avg;
	__IO uint16_t V_Mains_Offset_Calib_Rqst;
	__IO uint32_t Fault_Indication_Time_Extend;

	__IO uint32_t PV_Ref_Soft;
};
extern struct Counters Cntr;

struct ADC_Limits{
	__IO int16_t I_Inv_Pri_AC_UL;
	__IO int16_t I_Inv_Pri_AC_LL;
	__IO int16_t V_Mains_AC_UL;
	__IO int16_t V_Mains_AC_LL;
	__IO uint16_t V_PV_UL;
	__IO uint16_t V_PV_LL;
	__IO uint16_t I_PV_UL;
	__IO uint16_t V_Batt_UL;
	__IO uint16_t V_Batt_LL;
};
extern  struct  ADC_Limits Norm_ADC_Limits;

struct ADC_Avg_Limits{
	__IO uint16_t V_PV_UL_H_Cut_Off;
	__IO uint16_t V_PV_UL_L_Cut_In;
	__IO uint16_t V_PV_LL_L_Cut_Off;
	__IO uint16_t V_PV_LL_H_Cut_In;
	__IO uint16_t I_PV_UL;
	__IO uint16_t V_Batt_UL_H_Cut_Off;
	__IO uint16_t V_Batt_UL_L_Cut_In;
	__IO uint16_t V_Batt_LL_L_Cut_Off;
	__IO uint16_t V_Batt_LL_H_Cut_In;
	__IO uint16_t Batt_Temp_UL_H_Cut_Off;
	__IO uint16_t Batt_Temp_UL_L_Cut_In;
	__IO uint16_t Batt_Temp_LL_L_Cut_Off;
	__IO uint16_t Batt_Temp_LL_H_Cut_In;
};
extern struct ADC_Avg_Limits Norm_Avg_Limits;

struct RMS_Limits{
	__IO uint16_t I_Inv_Pri_AC_UL;
	__IO uint16_t I_Inv_Pri_AC_LL;
	__IO uint16_t V_Mains_AC_UL_H_Cut_Off;
	__IO uint16_t V_Mains_AC_UL_L_Cut_In;
	__IO uint16_t V_Mains_AC_LL_L_Cut_Off;
	__IO uint16_t V_Mains_AC_LL_H_Cut_In;
	__IO uint16_t I_Inv_Pri_AC_100_Percent;
	__IO uint16_t I_Inv_Pri_AC_110_Percent;
	__IO uint16_t I_Inv_Pri_AC_125_Percent;
	__IO uint16_t I_Inv_Pri_AC_150_Percent;
	__IO uint16_t I_Inv_Pri_AC_200_Percent;
};
extern struct RMS_Limits Norm_RMS_Limits;

struct Other_Limits{

	__IO uint16_t Batt_Temp50;    // this variable corresponds to 50 degrees celcius of temperature
	__IO uint16_t Batt_Temp80;    // this variable corresponds to 80 degrees celcius of temperature
	__IO uint8_t  Sine_Length;  // 192 -> corresponds to 20ms of time
	__IO uint8_t Half_Sine_Length; // 96-> corresponds to 10ms of time
	__IO uint16_t Fault_Reg_1;
	__IO uint16_t Fault_Reg_2;
	__IO unsigned Recieved_Settings_Ok:1;
	__IO unsigned Recieved_Settings_Error:1;
	__IO uint32_t System_Off_Timer;
	__IO uint32_t System_Off_Timer_Limit;
	__IO uint32_t System_Off_Timer_Limit_Cycle_Count;//to count number of times the system off timer limit is crossed
	__IO uint16_t V_Batt_Float_Ref;
	__IO uint16_t V_Batt_Boost_Ref;
	__IO uint16_t V_Batt_Equaliz_Ref;
	__IO uint16_t I_Batt_Boost_Ref;
	__IO uint16_t I_Batt_Chg_Equaliz_Ref;

	__IO uint16_t V_Grid_Float_Ref;
	__IO uint16_t V_Grid_Boost_Ref;

	__IO uint16_t I_Batt_Chg_Boost_Ref;
	__IO uint16_t I_Batt_Chg_Boost_Cutoff_ref;//in boost mode if current falls below this limit then we exit boost mode
	__IO uint16_t I_Batt_Chg_Float_Ref;

	__IO uint8_t  Batt_Chg_Mode; //Float:1, Boost:2, Equilization:3
	__IO uint8_t Batt_Chg_Prev_Mode;
	__IO unsigned Batt_Equalization_Reqst:1;

	__IO uint8_t Grid_Batt_Chg_Mode; //Float:1, Boost:2,
//	__IO uint8_t Grid_Batt_Chg_Prev_Mode;

	__IO unsigned Batt_Chg_EOS_Timer_Count_Reqst:1;
	__IO uint32_t Batt_Chg_EOS_Timer;
	__IO uint32_t Batt_Chg_EOS_Timer_Limit;

//	__IO unsigned Grid_Batt_Chg_EOS_Timer_Count_Reqst:1;
//	__IO uint32_t Grid_Batt_Chg_EOS_Timer;
//	__IO uint32_t Grid_Batt_Chg_EOS_Timer_Limit;

	__IO unsigned Batt_Equilization_Mode_Timer_Count_Reqst:1;
	__IO uint32_t Batt_Equilization_Mode_Timer_Minutes;
	__IO uint32_t Batt_Equilization_Mode_Timer_Seconds;
	__IO uint32_t Batt_Equilization_Mode_Timer_Minutes_Limit;

	__IO unsigned Batt_Boost_Mode_Timer_Count_Reqst:1;
	__IO uint32_t Batt_Boost_Mode_Timer_Minutes;
	__IO uint32_t Batt_Boost_Mode_Timer_Seconds;
	__IO uint32_t Batt_Boost_Mode_Timer_Minutes_Limit;

	__IO unsigned Batt_Grid_Boost_Mode_Timer_Count_Reqst:1;
	__IO uint32_t Batt_Grid_Boost_Mode_Timer_Minutes;
	__IO uint32_t Batt_Grid_Boost_Mode_Timer_Seconds;
	__IO uint32_t Batt_Grid_Boost_Mode_Timer_Minutes_Limit;

	__IO uint32_t Batt_Charge_Discharge_Cycle_Count;
	__IO uint32_t Batt_Charge_Discharge_Cycle_Count_Limit;

//	__IO uint32_t Grid_Batt_Charge_Discharge_Cycle_Count;
//	__IO uint32_t Grid_Batt_Charge_Discharge_Cycle_Count_Limit;

	__IO unsigned GPRS_On_Off_Command:1;
	__IO uint32_t PV_Power_Limit;
	__IO unsigned Inv_Off_Latch:1;
	__IO uint8_t Overload_110_percent_Count;
	__IO uint8_t Overload_125_percent_Count;
	__IO uint8_t Overload_150_percent_Count;
	__IO uint32_t Overload_Count_Reset_Timer;
	__IO uint32_t Overload_Count_Reset_Timer_Limit;
	__IO unsigned Previous_Switch_State:1;
	__IO uint8_t V_RMS_Inv_OV_Shutdown_Count;
	__IO uint32_t Batt_Temp_Comp_Factor;
	__IO unsigned Batt_Temp_Outof_Bounds_Flag:1;

	__IO uint16_t Relay_OFF_Delay_Time;
//	__IO uint16_t Relay_ON_Delay_Time;
	__IO uint16_t Relay_Activation_Delay;

	__IO uint16_t Inv_Short_Ckt_Count;

	__IO uint16_t Inv_Fast_I_limit_Count;

	__IO uint16_t Noload_Shutdown_Activate;
	__IO uint16_t I_Load_CT_NoLoad_Detect_RMS_Limit;

	__IO unsigned Noload_Detect_Cntr_Rqst:1;;
	__IO unsigned Noload_Turnoff_to_Turnon_Delay_Cntr_Rqst:1;

	__IO unsigned Noload_Shutdown_Flag:1;

	__IO unsigned Previous_PowerON_Status_VbattS2:1;

	__IO uint16_t V_Mains_Offset_Calib_Rqst;

	__IO unsigned Fault_Indication_Time_Extend_Rqst:1;
	__IO unsigned Fault_Indication_Time_Extended_I_Inv:1;
	__IO unsigned Fault_Indication_Time_Extended_Short_Circuit:1;
	__IO unsigned Fault_Indication_Time_Extended_Fast_I_Inv:1;

	__IO uint16_t ZCD_Counter;
	__IO uint16_t ZCD_Present_State;
	__IO uint16_t ZCD_Prev_State;
	__IO uint16_t ZCD_Value;
	__IO uint16_t ZCD_Value_From_Timer;

};
extern struct Other_Limits Othr;

struct Limit_Flags{
	__IO unsigned I_Inv:1;
	__IO unsigned V_Mains:1;
	__IO unsigned Freq:1;
	__IO unsigned V_PV:1;
	__IO unsigned I_PV:1;
	__IO unsigned V_Batt:1;
	__IO unsigned Batt_Over_Temp:1;
	__IO unsigned Inv_Over_Temp:1;
	__IO unsigned I_Inv_Over_Load_110:1;
	__IO unsigned I_Inv_Over_Load_125:1;
	__IO unsigned I_Inv_Over_Load_150:1;
	__IO unsigned Grid_Failure:1;
	__IO unsigned V_Batt_For_PV:1;
	__IO unsigned DC_insulation:1;

	__IO unsigned Short_Circuit:1;
	__IO unsigned V_Mains_Offset_Calib_Failure:1;
};
extern struct Limit_Flags Fast_Limit_Flags;
extern struct Limit_Flags RMS_Limit_Flags;
extern struct Limit_Flags Avg_Limit_Flags;

struct Inv_Control_Var{
	__IO unsigned RMS_Sqrt_Calc_Request:1;
	__IO unsigned Start_Ov_Temp_Cnt_Request:1;  // used to start the Over_Temp counter when battery or inveerter temperature is higher than a set limit
	__IO unsigned EOS0_Cntr_Request:1;
	__IO unsigned ZCD:1;
	__IO unsigned First_Time_State_Entry:1;
	__IO unsigned Inv_Off_to_Mains_Relay_Off_Delay_Flag:1;
	__IO uint8_t State;
	__IO uint8_t Prev_State;
	__IO int32_t V_Generate;
	__IO int32_t V_Delta_Generate;
	__IO int32_t I_Peak_Ref;
	__IO int32_t I_Ref;
	__IO int32_t I_Error_Inv;
	__IO int32_t V_Batt_Err;
	__IO int32_t V_Batt_Err_Integ;
	__IO int32_t V_Batt_ref;
	__IO int32_t PV_ref;
	__IO uint32_t Count;
	__IO uint16_t Duty_Cycle_1;
	__IO uint16_t Duty_Cycle_2;
	__IO uint32_t Ki_V_Batt;
	__IO uint32_t Kp_I_Inv_Loop;
	__IO uint32_t Soft_Start_Var;
	__IO uint16_t PerUnit;        // this variable has been given the value 4096
	__IO uint16_t Time_Period_Grid;
	__IO uint16_t Time_Period_Grid_Shifted;
	__IO int16_t Inv_Export_Ipeak_limit;
	__IO int16_t Inv_Import_Ipeak_limit;
	__IO uint16_t Time_Period_Grid_UL;  //  42533
	__IO uint16_t Time_Period_Grid_LL;  // 37735
	__IO int64_t V_Mains_FF;
	__IO int32_t V_Mains_Adjstr;
	__IO uint32_t V_Batt_For_Mains_Adjst;
	__IO uint32_t Inverter_ON_Delay;
	__IO int16_t Diff_of_Voltage;
	__IO int16_t Prev_V_Mains_AC;
	__IO int32_t Over_Load_Cntr;
	__IO int32_t Power_Factor;
	__IO unsigned Direct_State_Entry;
	__IO unsigned Inverter_ON_Delay_Rqst:1;
	__IO unsigned Disable_Grid_Freq_Update_and_Freq_Diag:1;
	__IO unsigned Start_ZCD_Diagonosis_Delay_Cntr:1;
	__IO unsigned Over_Load_Cntr_Rqst:1;
	__IO unsigned Grid_Disconnected_Flag:1;
	__IO unsigned Synchronizing_Var:1;
	__IO unsigned Diag_Delay:1;
	__IO unsigned In_Transition_to_Inv_Mode:1;

	__IO uint32_t I_Inv_Pri_Calib_Offset;
	__IO uint32_t I_Inv_Pri_Calib_Offset_Sum;

	__IO uint32_t I_Mains_CT_Calib_Offset;
	__IO uint32_t I_Mains_CT_Calib_Offset_Sum;

	__IO uint32_t I_Inv_CT_Calib_Offset;
	__IO uint32_t I_Inv_CT_Calib_Offset_Sum;
	__IO unsigned Calib_Successful_Flag:1;

	__IO uint16_t V_Grid_Boost_Ref;
	__IO uint16_t V_Grid_Float_Ref;
	__IO int32_t Integral_For_Sat_Detect;
	__IO uint16_t Local_Load_Compensation;
	__IO uint16_t Inverter_Rating;

	__IO uint16_t Wave_Shift_Count;
	__IO uint16_t Half_Wave_Symm_Count;
	__IO uint16_t Half_Wave_Symm_Adjst;
	__IO unsigned PV_Absent:1;
	__IO unsigned Relay_Activation_Delay_Rqst:1;
	__IO int32_t V_Mains_Failure_Band_UL;
	__IO int32_t V_Mains_Failure_Band_LL;
	__IO unsigned Inv_Off_Dueto_Low_V_Batt:1;

	__IO uint16_t V_Mains_Calib_Offset;
	__IO uint32_t V_Mains_Calib_Offset_Sum;

	__IO uint16_t Earth_Fault_Calib_Offset;

	__IO int32_t Kp_V_DC_Link_Loop;
	__IO int32_t Ki_V_DC_Link_Loop;
	__IO int32_t V_Loop_Proportional_OP;
	__IO int32_t V_Batt_Err_Prop_Integ;

	__IO int32_t Divide_1_By_5P3;
	__IO uint32_t State_1_Counter;
	__IO uint8_t State_1_Start_Counter_Flag;

	__IO int32_t Inv_PWM_50_Percent;

	__IO int8_t I_Inv_Soft_Start;
};
extern struct Inv_Control_Var Inv_Cntrl;

struct PV_Control_Var{
	__IO unsigned Duty_Inc:1;
	__IO unsigned Duty_Dec:1;
	__IO unsigned Duty_Sweep_Mode:1; //(1-> D increment mode)
	__IO unsigned I_PV_Soft_Limit_Flag:1;
	__IO unsigned V_PV_Soft_Limit_Flag:1;
	__IO unsigned First_Time_State_Entry:1;
	__IO uint8_t State;
	__IO int32_t Power_Old;
	__IO int32_t Power_New;
	__IO uint16_t Duty_Cycle;
	__IO uint16_t Delta_Duty;
	__IO uint32_t Mppt_Loop_Time; // 96000-> corresponds to 10 sec of time
	__IO uint16_t V_PV_Soft_Limit;
	__IO uint16_t I_PV_Soft_Limit;
	__IO uint16_t V_Batt_ref;
	__IO uint16_t V_PV_OC;

	__IO uint16_t Delta_Duty_Pos_Step;
	__IO uint16_t Delta_Duty_Neg_Step;
};
extern struct PV_Control_Var PV_Cntrl;


//please do not decrease the data type of the below structure
struct Debug_Var{
	__IO uint16_t A;
	__IO uint16_t B;
	__IO uint16_t C;
	__IO uint16_t D;
	__IO uint16_t E;
	__IO uint16_t F;
	__IO uint16_t G;
	__IO uint16_t H;
	__IO uint16_t I;
	__IO uint16_t J;
	__IO uint16_t K;
	__IO uint16_t L;
	__IO uint16_t M;
	__IO uint16_t N;
	__IO uint16_t Overall;


	__IO uint16_t I_Inv_CT_ADC_Dummy_ADC;
	__IO uint16_t I_Mains_CT_ADC_Dummy_ADC;
	__IO uint16_t I_Inv_pri_ADC_Dummy_ADC;
	__IO uint16_t Mains_Volt_ADC_Dummy_ADC;
	__IO uint16_t I_Batt_ADC_Dummy_ADC;
	__IO uint16_t I_PV_ADC_Dummy_ADC;
	__IO uint16_t V_PV_ADC_Dummy_ADC;
	__IO uint16_t V_Batt_ADC_Dummy_ADC;
	__IO uint16_t Batt_Temp_Dummy_ADC;
	__IO uint16_t V_Inv_ADC_Dummy_ADC;

};
extern struct Debug_Var Debug_Fast;
extern struct Debug_Var Debug_Avg;
extern struct Debug_Var Debug_Rms;
extern struct Debug_Var Debug;
extern struct Debug_Var Debug1;
extern struct Debug_Var Debug_State;
extern struct Debug_Var Debug_Grid_Tie;

struct Metering_Var{

	__IO uint64_t Export_Energy;
	__IO uint64_t Export_Energy_CT;
	__IO uint32_t Export_KWH;
	__IO uint32_t Export_KWH_CT;
	__IO uint64_t Import_Energy;
	__IO uint64_t Import_Energy_CT;
	__IO uint32_t Import_KWH;
	__IO uint32_t Import_KWH_CT;
	__IO uint64_t Batt_Ampere_Out_Sum;
	__IO uint32_t Batt_AH_Out;
	__IO uint64_t Batt_Ampere_In_Sum;
	__IO uint32_t Batt_AH_In;
	__IO uint64_t PV_KWH_Sum;
	__IO uint32_t PV_KWH;
	__IO uint32_t Apparent_Power;

};
extern struct Metering_Var Metering;
extern struct Metering_Var Metering_GSM_Card;


struct Temporary_Settings_Var{

	__IO uint16_t Inv_Volt_Set;
	__IO uint16_t V_Batt_Float_ref;
	__IO uint16_t V_Batt_Equaliz_ref;
	__IO unsigned Batt_Equalization_Reqst:1;
	__IO uint16_t V_Batt_Boost_ref;
	__IO uint16_t V_Batt_ref;
	__IO uint16_t V_Batt_UL_H_Cut_Off;
	__IO uint16_t V_Batt_UL_L_Cut_In;
	__IO uint16_t V_Batt_LL_H_Cut_In;
	__IO uint16_t V_Batt_LL_L_Cut_Off;
	__IO uint16_t I_Batt_Charg_UL;
	__IO uint16_t I_Inv_Pri_AC_100_Percent;
	__IO uint16_t Relay_OFF_Delay_Time;//in multiples of 104us seconds
	__IO uint16_t Relay_ON_Delay_Time;//in multiples of 104us seconds
//	__IO uint16_t I_Inv_Pri_AC_100_Percent;
//	__IO uint16_t I_Inv_Pri_AC_110_Percent;
//	__IO uint16_t I_Inv_Pri_AC_125_Percent;
//	__IO uint16_t I_Inv_Pri_AC_150_Percent;
	__IO uint16_t Inv_Ipeak_limit;
	__IO uint16_t Inv_Import_Ipeak_limit;
	__IO uint16_t V_PV_UL_H_Cut_Off;
	__IO uint16_t V_PV_UL_L_Cut_In;
	__IO uint16_t V_PV_LL_H_Cut_In;
	__IO uint16_t V_PV_LL_L_Cut_Off;
	__IO uint16_t I_PV_UL;
	__IO uint16_t PV_Power_Limit;
	__IO uint16_t V_Mains_AC_UL_H_Cut_Off;
	__IO uint16_t V_Mains_AC_UL_L_Cut_In;
	__IO uint16_t V_Mains_AC_LL_H_Cut_In;
	__IO uint16_t V_Mains_AC_LL_L_Cut_Off;

	__IO uint16_t Mode;
	__IO uint16_t Grid_Connect_BV;
	__IO uint16_t Grid_Chg_Start_BV;
	__IO uint16_t Grid_Chg_Stop_BV;
	__IO uint16_t Grid_Dis_BV;
	__IO uint16_t Grid_Dis_Batt_Current;
	__IO uint16_t Charging_Current_Through_Grid;
	__IO uint16_t PV_Chg_V_Batt_ref;
	__IO uint16_t V_Grid_Boost_Ref;
	__IO uint16_t V_Grid_Float_Ref;

	__IO uint16_t Inverter_Rating;
	__IO uint16_t Battery_Type;
	__IO uint16_t Batt_Boost_Mode_Timer_Minutes_Limit;

	__IO uint16_t Noload_Shutdown_Activate;
	__IO uint16_t I_Load_CT_NoLoad_Detect_RMS_Limit;

	__IO uint16_t V_Mains_Offset_Calib_Rqst;

};
extern struct Temporary_Settings_Var Temp_Settings;

struct Temporary_Variables{
	__IO uint32_t I_Inv_Pri;
	__IO uint32_t V_Mains;
	__IO uint32_t V_Inv;
	__IO uint32_t I_Mains_CT;
	__IO uint32_t I_Inv_CT;
	__IO uint32_t Earth_Fault;


};
extern struct Temporary_Variables Temp_Var;


struct Calibration_Settings{
	__IO uint16_t V_Inv;
	__IO uint16_t V_Batt;
	__IO uint16_t I_Inv;
	__IO uint16_t I_Batt;
	__IO uint16_t V_PV;
	__IO uint16_t I_PV;
	__IO uint16_t V_Mains;
	__IO uint16_t I_Mains_CT;
	__IO uint16_t I_Inv_CT;
	__IO uint16_t Earth_Fault;

	__IO unsigned Error:1;
	__IO unsigned OK:1;
	__IO unsigned Copy_To_Flash_Request:1;
	__IO unsigned Send_Prev_Calib_Values;
	__IO unsigned Copy_To_Flash_Rqst_V_Mains_Offset:1;

};
extern struct Calibration_Settings Calibration_Temp;
extern struct Calibration_Settings Calibration_Var;

extern __IO uint16_t ADC_Value[11];
extern const __IO int32_t Sine_PWM[192];
extern const __IO int32_t Sine_Wave_170V[192];
extern const __IO int32_t Sine_Wave_270V[192];
extern __IO int16_t Debug_Array[199];


#endif /* NEW_STRUCT_VARIABLES_H_ */
