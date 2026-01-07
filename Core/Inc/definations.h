/*
 * #defines.h
 *
 *  Created on: 23-Mar-2019
 *      Author: ankur
 */

#ifndef DEFINATIONS_H_
#define DEFINATIONS_H_

#include "stm32f0xx.h"
#include "New_Struct_Variables.h"


#endif /* DEFINATIONS_H_ */

#define I_Inv_Pri_Offset_ADC    ADC_Value[0]
#define I_PV_ADC  				ADC_Value[1]
#define I_Inv_pri_ADC   		ADC_Value[2]
#define V_PV_ADC  				ADC_Value[3]
#define V_DC_Link_ADC      		ADC_Value[4]
#define Mains_Volt_ADC        	ADC_Value[5]
#define Inv_Temp_ADC       		ADC_Value[6]
#define Earth_Fault_ADC			ADC_Value[7]
#define Grid_DC_Component_ADC	ADC_Value[8]
#define I_Mains_CT_ADC      	ADC_Value[9]
#define DC_Insulation_ADC      	ADC_Value[10]
//#define Earth_Check_ADC         ADc_Value[11]  //need to change


//#define I_Inv_CT_ADC    Debug.I_Inv_CT_ADC_Dummy_ADC
//#define I_Mains_CT_ADC  Debug.I_Mains_CT_ADC_Dummy_ADC
//#define I_Inv_pri_ADC   Debug.I_Inv_pri_ADC_Dummy_ADC
//#define Mains_Volt_ADC  Debug.Mains_Volt_ADC_Dummy_ADC
//#define I_Batt_ADC      Debug.I_Batt_ADC_Dummy_ADC
//#define I_PV_ADC        Debug.I_PV_ADC_Dummy_ADC
//#define V_PV_ADC        Debug.V_PV_ADC_Dummy_ADC
//#define V_Batt_ADC      Debug.V_Batt_ADC_Dummy_ADC
//#define Batt_Temp       Debug.Batt_Temp_Dummy_ADC
//#define V_Inv_ADC       Debug.V_Inv_ADC_Dummy_ADC

/* ..................................................................................ALL #DEFINES ........................................................................................................*/
#define Two_Hundred_ms_Count 	1920
#define Half_Second_Count 		4800
#define One_Second_Count 		9600
#define Two_Second_Count 		19200
#define Three_Second_Count 		28800
#define Four_Second_Count 		38400
#define Five_Second_Count 		48000
#define Ten_Second_Count 		96000
#define Twenty_Second_Count 	192000
#define Thirty_Second_Count 	288000
#define One_Minute_Count 		576000
#define Ninety_Second_Count 	864000
#define Two_Minute_Count 		1152000
#define Fifteen_Minute_Count 	8640000
/*
#define I_Inv_Pri_AC_100_Percent 1639 		// 160A
#define I_Inv_Pri_AC_110_Percent 1741 		// 175A
#define I_Inv_Pri_AC_125_Percent 2048		// 200A
#define I_Inv_Pri_AC_150_Percent 2458 		// 240A
*/
#define Inverter_Is_Overloaded 	(Norm_Rms.I_Inv_Pri>Norm_RMS_Limits.I_Inv_Pri_AC_100_Percent)

#define Delta_Time_Period 		20 // 10 = 0.16ms, maximum valu should be 30
#define Soft_Start_1_Percent 	655                    // 0.01 <<16 = 655
#define Soft_Start_75_Percent 	49999
#define Soft_Start_70_Percent 	46666
#define Soft_Start_50_Percent 	33333
#define V_Inv_Sec_203V 			1785
#define V_Inv_Sec_210V 			1850

#define V_Batt_450V 			(int64_t)2565
#define V_Batt_390V 			(int64_t)1620 // instead of 1644 which corresponds to 400V
#define V_Batt_Error_45V 		(int64_t)185
#define V_PV_160V  				(int64_t)2696
#define I_Inv_Pri_Peak_22A		(int64_t)2000
#define I_Inv_Pri_Peak_M_22A  	(int64_t)-2000
#define I_PV_25A  				(int64_t)750
#define Power_Limit  			(int64_t)741
#define V_PV_400V  				(int64_t)2282

#define I_Export_Limit_10A_Peak 			(int64_t)1200 //6Amp
#define I_Import_Limit_10A_Peak 			(int64_t)0 //6Amp
#define V_Batt_ADC_310V 					(int64_t)1307


#define Norm_V_Batt_450V_Lsh_12 			6905868    //ADC_Value of 400V*4096 making into normalize
#define V_Batt_2V 							8
#define V_Batt_10V 							41
#define E_for_1KWHr 						65765436609
#define Point_01_E_for_1KWHr 				657654366 //=.01 Kwh
#define E_for_1KWHr_Mains_CT 				159483023700
#define Point_01_E_for_1KWHr_Mains_CT 		1594830237 //=.01 Kwh
#define Value_for_1AHr 						7372818
#define PV_E_For_1KWHr 						42934424821
#define P_01_PV_E_For_1KWHr 				429344248 //=.01 Kwh‬

#define MPPT_PWM_Channel 					(TIM14->CCR1)
#define MPPT_Time_Period 					(TIM14->ARR)

#define PWM_50_percnt 						(TIM1->ARR>>1)

#define GPIO_ON 							(GPIOC->ODR|=GPIO_ODR_13)
#define GPIO_OFF 							(GPIOC->ODR&=(~GPIO_ODR_13))
#define Mains_Relay_Off 					(GPIOF->ODR &= (~GPIO_ODR_4))   // Switching off mains relay
#define Mains_Relay_On  					(GPIOF->ODR |= GPIO_ODR_4)// Switching on mains relay (inverter relay is on instead of mains)
#define Is_Mains_Relay_ON 					(GPIOC->ODR&GPIO_ODR_13)
#define Is_Inv_Off 							((TIM1->CCR2==0)&&(TIM1->CCR3==0)&&(!(TIM1->BDTR & 0x8000)))
#define Switch_Is_ON 						(GPIOA->IDR&GPIO_IDR_11) // checking if the switch is switched OFF
#define Switch_Is_OFF 						!(GPIOA->IDR&GPIO_IDR_11)
#define Shutdown_Is_High 					(GPIOF->IDR&GPIO_IDR_0) // checking if the switch is switched OFF
#define Shutdown_Is_Low 					!(GPIOF->IDR&GPIO_IDR_0)

#define ZCD_Pin_is_High 					(GPIOC->IDR&GPIO_IDR_6)
#define ZCD_Pin_is_Low 						!(GPIOC->IDR&GPIO_IDR_6)

#define Driver_Supply_Ok 					(GPIOB->IDR&GPIO_IDR_12)
#define Driver_Supply_is_Not_Ok				!(GPIOB->IDR&GPIO_IDR_12)

#define PFC_Power_OFF 						(GPIOA->ODR &= (~GPIO_ODR_12))   // Switching off mains relay
#define PFC_Power_ON  						(GPIOA->ODR |= GPIO_ODR_12)// Switching on mains relay (inverter relay is on instead of mains)

#define DC_Insulation_is_Ok 				(GPIOC->IDR&GPIO_IDR_5)
#define DC_Insulation_is_Not_Ok				!(GPIOC->IDR&GPIO_IDR_5)

#define I_PV_Outof_Fast_Limits_UL  			(I_PV_ADC>Norm_ADC_Limits.I_PV_UL)
#define V_PV_Outof_Fast_Limits 				(V_PV_ADC>Norm_ADC_Limits.V_PV_UL)||(V_PV_ADC<Norm_ADC_Limits.V_PV_LL)
#define I_Batt_Charg_Outof_Fast_Limits_UL 	((MOBD==0)&&(I_Batt_ADC>Norm_ADC_Limits.I_Batt_Charg_UL))
#define I_Batt_Discharg_Outof_Fast_Limits_UL ((MOBD==1)&&(I_Batt_ADC>Norm_ADC_Limits.I_Batt_Discharg_UL))
#define V_Batt_Outof_Fast_Limits 			(V_DC_Link_ADC>Norm_ADC_Limits.V_Batt_UL)||(V_DC_Link_ADC<Norm_ADC_Limits.V_Batt_LL)
#define I_Inv_Pri_AC_Outof_Fast_Limits 		(Norm_ADC.I_Inv_Pri_AC>Norm_ADC_Limits.I_Inv_Pri_AC_UL)||(Norm_ADC.I_Inv_Pri_AC<Norm_ADC_Limits.I_Inv_Pri_AC_LL)
#define V_Inv_Outof_Fast_Limits_UL 			(Norm_ADC.V_Inv_AC>Norm_ADC_Limits.V_Inv_UL)||(Norm_ADC.V_Inv_AC<Norm_ADC_Limits.V_Inv_LL)
#define V_Mains_Outof_Fast_Limits 			(Norm_ADC.V_Mains_AC>Norm_ADC_Limits.V_Mains_AC_UL)||(Norm_ADC.V_Mains_AC<Norm_ADC_Limits.V_Mains_AC_LL)

#define V_Batt_Outof_Fast_Limits_For_Inv 	(V_DC_Link_ADC>(uint32_t)(3300U))||(V_DC_Link_ADC<(uint32_t)(1500U)) //3300 means 66V and 1500 means 30V
#define V_Batt_Outof_Fast_Limits_For_PV 	(V_DC_Link_ADC>(uint32_t)(3300U))||(V_DC_Link_ADC<(uint32_t)(2250U)) //3300 means 66V and 2250 means 45V

#define I_PV_Outof_Avg_Limits_UL 			(Norm_Avg.I_PV>Norm_Avg_Limits.I_PV_UL)
#define V_PV_Outof_Avg_Limits_Cutoff 		(Norm_Avg.V_PV>Norm_Avg_Limits.V_PV_UL_H_Cut_Off)||(Norm_Avg.V_PV<Norm_Avg_Limits.V_PV_LL_L_Cut_Off)
#define V_PV_Inside_Avg_Limits_Cutin 		(Norm_Avg.V_PV<=Norm_Avg_Limits.V_PV_UL_L_Cut_In)&&(Norm_Avg.V_PV>=Norm_Avg_Limits.V_PV_LL_H_Cut_In)

#define I_Batt_Charg_Outof_Avg_Limits_UL 	(Norm_Avg.I_Batt<-(Norm_Avg_Limits.I_Batt_Charg_UL)) //I_Average can be positive and negative
#define I_Batt_Discharg_Outof_Avg_Limits_UL (Norm_Avg.I_Batt>Norm_Avg_Limits.I_Batt_Discharg_UL)
#define V_Batt_Outof_Avg_Limits_Cutoff 		((Norm_Avg.V_Batt>Norm_Avg_Limits.V_Batt_UL_H_Cut_off)||(Norm_Avg.V_Batt<Norm_Avg_Limits.V_Batt_LL_L_Cut_Off))//3300 means 66V and 1500 means 30V
#define V_Batt_Outof_Avg_Limits_For_S1 		(Norm_Avg.V_Batt>(uint32_t)(3300U)||(Norm_Avg.V_Batt<(uint32_t)(1500U)))//3300 means 66V and 1500 means 30V
#define V_Batt_Outof_Avg_Limits_Cutoff_For_PV (Norm_Avg.V_Batt>(uint32_t)(3300U))||(Norm_Avg.V_Batt<(uint32_t)(2250U))//3300 means 66V and 2250 means 45V

#define V_Batt_Inside_Avg_Limits_Cutin 		(Norm_Avg.V_Batt<Norm_Avg_Limits.V_Batt_UL_L_Cut_In)&&(Norm_Avg.V_Batt>Norm_Avg_Limits.V_Batt_LL_H_Cut_In)
#define V_Batt_Inside_Avg_Limits_For_S2 	(Norm_Avg.V_Batt<Norm_Avg_Limits.V_Batt_UL_L_Cut_In)&&(Norm_Avg.V_Batt>Norm_Avg_Limits.V_Batt_LL_H_Cut_In)
#define V_Batt_Inside_Avg_Limits_For_S1 	(Norm_Avg.V_Batt<(uint32_t)(3200U))&&(Norm_Avg.V_Batt>(uint32_t)(1600U)) // between 64V and 32V
#define V_Batt_Inside_Avg_Limits_Cutin_For_PV (Norm_Avg.V_Batt<(uint32_t)(3000U))&&(Norm_Avg.V_Batt>(uint32_t)(2500U))//2500 means 50V , 3000 means 60V

#define Temp_Batt_Outof_Avg_Limits_Cutoff 	(Norm_Avg.Batt_Temperature>Norm_Avg_Limits.Batt_Temp_UL_H_Cut_Off)||(Norm_Avg.Batt_Temperature<Norm_Avg_Limits.Batt_Temp_LL_L_Cut_Off)
#define Temp_Batt_Inside_Avg_Limits_Cutin 	(Norm_Avg.Batt_Temperature<=Norm_Avg_Limits.Batt_Temp_UL_L_Cut_In)&&(Norm_Avg.Batt_Temperature>=Norm_Avg_Limits.Batt_Temp_LL_H_Cut_In)

#define I_Inv_Pri_Outof_RMS_Limits_UL 		(Norm_Rms.I_Inv_Pri>Norm_RMS_Limits.I_Inv_Pri_AC_UL)
#define V_Mains_Outof_RMS_Limits_Cutoff 	(Norm_Rms.V_Mains>Norm_RMS_Limits.V_Mains_AC_UL_H_Cut_Off)||(Norm_Rms.V_Mains<Norm_RMS_Limits.V_Mains_AC_LL_L_Cut_Off)
#define V_Mains_Inside_RMS_Limits_Cutin 	(Norm_Rms.V_Mains<Norm_RMS_Limits.V_Mains_AC_UL_L_Cut_In)&&(Norm_Rms.V_Mains>Norm_RMS_Limits.V_Mains_AC_LL_H_Cut_In)

#define Freq_Out_Of_limits 					(Inv_Cntrl.Time_Period_Grid>Inv_Cntrl.Time_Period_Grid_UL)||(Inv_Cntrl.Time_Period_Grid<Inv_Cntrl.Time_Period_Grid_LL)
#define Freq_In_limits 						(Inv_Cntrl.Time_Period_Grid<Inv_Cntrl.Time_Period_Grid_UL)&&(Inv_Cntrl.Time_Period_Grid>Inv_Cntrl.Time_Period_Grid_LL)

#define Grid_is_Disconnected 				((Fast_Limit_Flags.V_Mains)||(Fast_Limit_Flags.Freq)||(RMS_Limit_Flags.V_Mains))
#define Grid_is_Connected 					((V_Mains_Inside_RMS_Limits_Cutin)&&(Freq_In_limits))

#define Grid_is_low 						(Norm_Rms.V_Mains<Norm_RMS_Limits_170V)
#define Norm_RMS_Limits_170V 				1657 //4096 => 420V

#define Ready_To_Run 						((Switch_Is_ON)&(Othr.Recieved_Settings_Ok)&(Calibration_Var.OK))

#define Debug_Flash_Write  					((uint16_t *) 0x0800F800)
#define Debug_Flash_Write_1  				((uint16_t *) 0x0800FC00)

#define Earth_Check_ADC_Out_Of_Limits       (Earth_Check_ADC < 546 || Earth_Check_ADC > 3003)

//#define INV_SENSE_RELAY                     (READ)

