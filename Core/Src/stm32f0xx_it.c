/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

//#include "variables.h"
#include "New_Struct_Variables.h"
#include "definations.h"
#include "math.h"
#include "User_Defined_Functions.h"
#include "UART2_Slave_Variables.h"
#include "UART2_Slave_Functions.h"
#include "UART1_Slave_Variables.h"
#include "UART1_Slave_Functions.h"
#include "UART_Debugging_Variables.h"
#include "UART_Debugging_Functions.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */

		Inverter_OFF();
		MPPT_OFF();
		Mains_Relay_Off;
		Debug_Fast.L = 1;

    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */

  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles TIM1 break, update, trigger and commutation interrupts.
  */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 0 */

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Cntrs_Incrmnt_Fn_for_100us_ISR();    //inverter functions which run in all inverter states (weather export or inverter state)

/////////////////////////////////Average Limits/////////////////////////////////////////////////////////////////////////////////////////////////////////


	if((Cntr.ADC_Avg == 10)&&(Inv_Cntrl.Diag_Delay == 0)) //as there may be delay in calculation of RMS after square root calc request is sent
	{
		Avg_Diagnostics();    // Diagnosis for all the average parameters

///////////////////////////////RMS Limits///////////////////////////////////////////////////////////////////////////////////////////////////////////////

		RMS_Diagnostics();    // Diagnosis for all the RMS parameters
	}

	while(!(DMA1->ISR&DMA_ISR_TCIF1))  //wait for DMA1 transfer complete flag (updating ADC array)
		{

		}
	DMA1->IFCR|=DMA_IFCR_CTCIF1;
	GPIO_ON;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if(Inv_Cntrl.Diag_Delay == 0)
	{
		Fast_Diagnostics();   // This diagnosis if any spurious happens
	}

	if((Norm_ADC.I_Inv_Pri_AC>I_Inv_Pri_Peak_22A) || (Norm_ADC.I_Inv_Pri_AC<I_Inv_Pri_Peak_M_22A))    //I_Inv_Pri_200A->2970 if the inverter current upper limit is crossed the switch off the inverter immediately
	{
		if(Inv_Cntrl.State == 1)
		{
			Debug_Fast.A |= (0x1U<<7);
			Debug_Fast.D = Norm_ADC.I_Inv_Pri_AC;
		}
		Debug_Fast.B |= (0x1U<<7);
		if(TIM1->BDTR&TIM_BDTR_MOE)
		{
			Othr.Inv_Fast_I_limit_Count=Othr.Inv_Fast_I_limit_Count+1;
		}
		Inverter_OFF();
		Fast_Limit_Flags.I_Inv = 1;
		Inv_Cntrl.State = 0;
		Inv_Cntrl.First_Time_State_Entry=1;

		if(Othr.Inv_Fast_I_limit_Count >= 10)
		{
			Othr.Inv_Off_Latch=1;
			Othr.Inv_Fast_I_limit_Count=0;
			Cntr.Inv_Fast_I_limit_Count_Reset_Counter=0;
		}
	}
	else
	{
		Debug_Fast.B &= ~(0x1U<<7);
	}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ADC_Avg_And_RMS_Cal();    // This function is used for calculating voltage and current average values

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Othr.Recieved_Settings_Ok = 1;             	// Forcefully we are making the bit 1 to run the inverter code
//	if(Othr.Recieved_Settings_Ok == 1)
//	{
		switch (PV_Cntrl.State)                   // shift this to switch case
		{
			case 0:

				PV_State_0_Int_Cntrl_Fn();            // In this parameters are monitored whether they are in limits or not
				break;

			case 1:

				PV_State_1_Int_Cntrl_Fn();            // In this MPPT control and switching takes place
				break;

			default:
				PV_Cntrl.State = 0; 									// state zero will be running as default condition if PV_Cntrl.State is not initialized
				PV_Cntrl.First_Time_State_Entry=1;
				break;
		}

	/********************************Inverter control states**********************************/

		switch (Inv_Cntrl.State)
		{
			case 0:

				Inv_State_0_Int_Cntrl();       // All the parameters are monitored and checked whether they are in limits or not

				break;

			case 1:     //STATE 1 (GRID EXPORT/IMPORT) RELATED INTERUPT CODE.

				Inv_State_1_Int_Cntrl();       // Exporting or importing takes place

				break;

			default:
				Inv_Cntrl.State = 0;
				Inv_Cntrl.First_Time_State_Entry=1;
				break;
		}
//	}

  /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 0 */
  /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 1 */

		UART_Debugging_Populate_Txdata_Array_With_Message_Debug_Data();

//	if(UART2_Send_Var.Count == 10)        //This code runs for every 1ms
//	{
//		UART2_Send_Function_Inside_Timer_Interrupt();
//		UART2_Send_Var.Count = 0;
//	}
	if(UART1_Send_Var.Count == 10)        //This code runs for every 1ms
	{
		UART_Debugging_Send_Function_Inside_1ms_Timer_Interrupt();
		UART1_Send_Function_Inside_Timer_Interrupt();
		UART1_Send_Var.Count = 0;
	}

	Inv_Cntrl.Prev_State = Inv_Cntrl.State;

		GPIO_OFF;
	TIM1->SR &= ~(TIM_SR_UIF); 						// clearing the update interrupt flag

  /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
//	UART1_Receive_ISR_Program();

//	UART1_Receive_ISR_Program();

    UART1_Clear_Receive_Flag;
    USART1->ICR|=USART_ICR_ORECF;


  /* USER CODE END USART1_IRQn 0 */
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */


//	UART2_Receive_ISR_Program();

//	UART1_Receive_ISR_Program();
    UART2_Clear_Receive_Flag;
    USART2->ICR|=USART_ICR_ORECF;

  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
