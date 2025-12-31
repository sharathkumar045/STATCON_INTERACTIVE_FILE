/*
 * User_Defined_Functions.h
 *
 *  Created on: 30-Mar-2019
 *      Author: ankur
 */

#ifndef USER_DEFINED_FUNCTIONS_H_
#define USER_DEFINED_FUNCTIONS_H_



#endif /* USER_DEFINED_FUNCTIONS_H_ */
void Inv_State_0_While_loop_Fn(void);
void Inv_State_0_Int_Diag(void);
void Inv_State_0_Int_Cntrl(void);

void Inv_State_1_While_loop_Fn(void);
void Inv_State_1_Int_Diag(void);
void Inv_State_1_Int_Cntrl(void);

void Inv_State_2_While_loop_Fn(void);
void Inv_State_2_Int_Cntrl(void);
void Inv_State_2_Int_Diag(void);

void Inv_State_3_While_loop_Fn(void);
void Inv_State_3_Int_Diag(void);
void Inv_State_3_Int_Cntrl(void);

void Inv_State_4_While_loop_Fn(void);
void Inv_State_4_Int_Diag(void);
void Inv_State_4_Int_Cntrl(void);

void PV_State_0_While_Loop_Fn(void);
void PV_State_0_Int_Diag_Fn(void);
void PV_State_0_Int_Cntrl_Fn(void);

void PV_State_1_While_Loop_Fn(void);
void PV_State_1_Int_Diag_Fn(void);
void PV_State_1_Int_Cntrl_Fn(void);

void TIM1_OEN_DT_CCEN_IEN_STRT(void);
void ADC_DMA_CH1_CONFIG(void);

void TIM3_CAPEN_IEN_STRT(void);
void CAPTURE_DMA_CH4_CONFIG(void);

void TIM14_CH1_PWM_Config(void);

void PV_Duty_Dec_Code(void);
void PV_Duty_Inc_Code(void);

void ADC_Avg_And_RMS_Cal(void);
void Update_Sine_Freq_To_Grid_Freq(void);
void Anti_Islanding_Update_Sine_Freq_To_Grid_Freq(void);
void Batt_V_Err_Integ_Saturator_Code(void);
void V_Generate_Saturator_Code(void);
void V_Delta_Generate_Saturator_Code(void);

void Cntrs_Incrmnt_Fn_for_100us_ISR(void);
void Grid_Volt_Meansquare_Calc(void);

void Fast_Diagnostics(void);
void Avg_Diagnostics(void);
void RMS_Diagnostics(void);

void Inverter_OFF(void);
void Inverter_ON(void);

void MPPT_OFF(void);
void MPPT_ON(void);

void Switch_Off_Mains_Relay(void);
void Switch_On_Mains_Relay(void);

void Variable_Init(void);

void Grid_Export_To_Inverter_State_Transition_Fn(void);
void Inverter_Mode_To_Grid_Export_State_Transition_Fn(void);
void Check_if_V_Mains_Out_of_Bounds(void);

void Energy_Calculation(void);
void AH_Calculation(void);
void PV_KWhr_Calculation(void);
void Energy_Calculation_For_GSM_Card(void);
void AH_Calculation_For_GSM_Card(void);
void PV_KWhr_Calculation_For_GSM_Card(void);
void Energy_Calculation_With_Mains_CT(void);
void Energy_Calculation_With_Mains_CT_For_GSM_Card(void);

void Copy_Calib_To_Flash(void);
void Copy_Calib_From_Flash(void);

void Flash_Erase();
void Flash_Write();
void Flash_Erase_1();
void Flash_Write_1();
void Flash_Read();


void Batt_Boost_Float_Changeover(void);
void Grid_Batt_Boost_Float_Changeover(void);
void Batt_Temp_Comp_Factor_Calculator(void);
void Generate_Fault_Data_For_Metering(void);

void State_Transition_Function(void);
void Grid_To_Inverter_State_Transition_Fn(void);
void Inverter_To_Grid_Mode_Transition_Fn(void);

void Local_Load_Compensation_if_Exprt_Limit_is_Zero(void);
void Power_Factor_Calculation(void);
void PV_Power_Unavailable_Detection(void);

void Zero_Crossing_Detection_Counter_Fn(void);

void Debug_Data_Storage_Fn(void);
