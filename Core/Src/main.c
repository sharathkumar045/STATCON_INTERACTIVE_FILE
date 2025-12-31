/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

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
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
//	reinit:
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM14_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  Variable_Init();
  UART_Debugging_Slave_Variables_Init();
  Fast_Limit_Flags.DC_insulation = 0;
    ADC1->CR |= ADC_CR_ADEN | ADC_CR_ADSTART;    // adc enable and adc start

  	///*TIMER 3  CONFIG AND DMA *///
	 TIM3_CAPEN_IEN_STRT();            // Timer 3 capture_enable : interupt enable: start of zero crossing detection
//	 CAPTURE_DMA_CH4_CONFIG();         // DMA configuration for capture enable and storing data

     ///* TIMER 1 CONFIG AND DMA*///
	 ADC_DMA_CH1_CONFIG();             // DMA configuration for adc values
	 TIM1_OEN_DT_CCEN_IEN_STRT();      // Timer 1 output_enable : dead_time: capture_compare : interupt_enable: start

	 TIM14_CH1_PWM_Config();

//	 Flash_Erase();
//	 Flash_Write();
//
//	 Copy_Calib_From_Flash();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//	 Norm_Avg.V_PV = 1097;
//	 Norm_Avg.I_PV = 435;

	 UART1_Enable_Receive_Interrupt; //Enabling the receive interrupt
	 UART1_Enable;

	 UART2_Enable_Receive_Interrupt; //Enabling the receive interrupt
	 UART2_Enable;

	 UART_Debugging_Enable_Receive_Interrupt; //Enabling the receive interrupt
	 UART_Debugging_Enable;

	 PFC_Power_ON;
  while (1)
  {
      // ...........................................................................  RMS CALCULATOR..................................................................................................//
  	  if(Inv_Cntrl.RMS_Sqrt_Calc_Request == 1)
  	  {
			Temp_Var.I_Inv_Pri = sqrtf(Norm_Rms.I_Inv_Pri_Mean_Sq); // Rms inverter current
			Temp_Var.V_Mains = sqrtf(Norm_Rms.V_Mains_Mean_Sq);     // Rms Mains voltage
			Temp_Var.I_Mains_CT = sqrtf(Norm_Rms.I_Mains_CT_Mean_Sq);

			Inv_Cntrl.RMS_Sqrt_Calc_Request = 0;

			Norm_Rms.I_Inv_Pri = (Temp_Var.I_Inv_Pri * Calibration_Var.I_Inv)>>11;
			Norm_Rms.I_Mains_CT = (Temp_Var.I_Mains_CT * Calibration_Var.I_Mains_CT)>>11;
			Norm_Rms.V_Mains = (Temp_Var.V_Mains * Calibration_Var.V_Mains)>>11;

			if(Norm_Rms.I_Inv_Pri >4096)
			{
				Norm_Rms.I_Inv_Pri = 4096;
			}
			if(Norm_Rms.V_Mains >4096)
			{
				Norm_Rms.V_Mains = 4096;
			}
			if(Norm_Rms.I_Mains_CT > 4096)
			{
				Norm_Rms.I_Mains_CT = 4096;
			}

			//////////////////////////////////////   For calculation of power factor    //////////////////////////////////////
//			Power_Factor_Calculation();
			//////////////////////////////////////

			if(Inv_Cntrl.State == 1)//state 1 implies grid export mode
			{
				if((Inv_Cntrl.I_Peak_Ref>Inv_Cntrl.Inv_Export_Ipeak_limit>>5)||(Inv_Cntrl.I_Peak_Ref<-(Inv_Cntrl.Inv_Import_Ipeak_limit>>5)))//
				{
					Energy_Calculation(); // Mains export or import energy calculation
//					Energy_Calculation_For_GSM_Card();
				}
			}
//			Energy_Calculation();
			PV_KWhr_Calculation();

			Energy_Calculation_With_Mains_CT(); // Mains export or import energy calculation

			if(Othr.System_Off_Timer<=5000)//turning off debug variable data for first 5 seconds
			{
				Debug_Fast.A=0;
				Debug_Fast.B=0;
				Debug_Fast.C=0;
				Debug_Fast.D=0;
				Debug_Fast.E=0;
				Debug_Fast.F=0;
				Debug_Fast.G=0;
				Debug_Fast.H=0;
				Debug_Fast.I=0;
				Debug_Fast.J=0;
				Debug_Fast.K=0;
				Debug_Fast.L=0;
				Debug_Fast.M=0;
				Debug_Fast.N=0;

				Debug_Rms=Debug_Fast;
				Debug_Avg=Debug_Fast;
				Debug_State = Debug_Fast;
			}
		}

//  	if(Calibration_Var.Copy_To_Flash_Rqst_V_Mains_Offset==1)
//	  {
//		Calibration_Var.Copy_To_Flash_Rqst_V_Mains_Offset=0;
//		TIM1->DIER &= ~TIM_DIER_UIE;//Disable TIM1_INT
//		TIM3->DIER &= (~TIM_DIER_CC1IE & ~TIM_DIER_CC1DE);//Disable TIM3_Capture_Int
//		UART2_Disable_Receive_Interrupt;
//		UART1_Disable_Receive_Interrupt;
//
//		//Make sure that cpu is not doing any other operation during erasing and copying to flash memory
//
//		// Two times flash erase and write is written because for first time when trying to write its not writing in flash
//		Flash_Erase_1();
//		for (__IO uint64_t i = 0; i < 48000; i++)
//			{
//
//			}
//
//
//		Flash_Write_1();
//		for (__IO uint64_t i = 0; i < 48000; i++)
//			{
//
//			}
//
//		Flash_Erase_1();
//		for (__IO uint64_t i = 0; i < 48000; i++)
//			{
//
//			}
//
//
//		Flash_Write_1();
//		for (__IO uint64_t i = 0; i < 48000; i++)
//			{
//
//			}
//
//		//Check if copied properly;
//		//RCC->BDCR|=RCC_BDCR_BDRST;
//		goto reinit;
//
//		for(__IO uint8_t ind=0; ind<200; ind++)
//			{
//				UART2_recvdata[ind]=0;
//			}
//			UART2_Rec_Var.Array_Index=0;
//			UART2_Rec_Var.recdata=0;
//			UART2_Rec_Flags.busy_flag=0;
//			UART2_Rec_Flags.recv_check=0;
//
//		for(__IO uint8_t ind1=0; ind1<200; ind1++)
//			{
//				UART1_recvdata[ind1]=0;
//			}
//			UART1_Rec_Var.Array_Index=0;
//			UART1_Rec_Var.recdata=0;
//			UART1_Rec_Flags.busy_flag=0;
//			UART1_Rec_Flags.recv_check=0;
//			RCC->BDCR|=RCC_BDCR_BDRST;
//
//					goto reinit;
//		TIM1->DIER |= TIM_DIER_UIE;
//		TIM3->DIER |= (TIM_DIER_CC1IE | TIM_DIER_CC1DE);//Enable TIM1_INT
//		UART2_Enable_Receive_Interrupt;
//		UART1_Enable_Receive_Interrupt;
//
//				//Enable TIM3_Capture_Int
//	  }
//
//  	  if(Calibration_Var.Copy_To_Flash_Request==1)
//  	  {
//  	  	Calibration_Var.Copy_To_Flash_Request=0;
//  	  	TIM1->DIER &= ~TIM_DIER_UIE;//Disable TIM1_INT
//  	  	TIM3->DIER &= (~TIM_DIER_CC1IE & ~TIM_DIER_CC1DE);//Disable TIM3_Capture_Int
//  	  	UART2_Disable_Receive_Interrupt;
//		UART1_Disable_Receive_Interrupt;
//
//  	  	//Make sure that cpu is not doing any other operation during erasing and copying to flash memory
//
//		// Two times flash erase and write is written because for first time when trying to write its not writing in flash
//		Flash_Erase();
//  	  	for (__IO uint64_t i = 0; i < 48000; i++)
//  	  	    {
//
//  	  	    }
//
//
//  	  	Flash_Write();
//  	  	for (__IO uint64_t i = 0; i < 48000; i++)
//  	  	    {
//
//  	  	    }
//
//		Flash_Erase();
//  	  	for (__IO uint64_t i = 0; i < 48000; i++)
//  	  	    {
//
//  	  	    }
//
//
//  	  	Flash_Write();
//  	  	for (__IO uint64_t i = 0; i < 48000; i++)
//  	  	    {
//
//  	  	    }
//
//  	  	//Check if copied properly;
//  	  	//RCC->BDCR|=RCC_BDCR_BDRST;
//  	  	//goto reinit;
//
//		for(__IO uint8_t ind=0; ind<200; ind++)
//			{
//				UART2_recvdata[ind]=0;
//			}
//			UART2_Rec_Var.Array_Index=0;
//			UART2_Rec_Var.recdata=0;
//			UART2_Rec_Flags.busy_flag=0;
//			UART2_Rec_Flags.recv_check=0;
//
//		for(__IO uint8_t ind1=0; ind1<200; ind1++)
//			{
//				UART1_recvdata[ind1]=0;
//			}
//			UART1_Rec_Var.Array_Index=0;
//			UART1_Rec_Var.recdata=0;
//			UART1_Rec_Flags.busy_flag=0;
//			UART1_Rec_Flags.recv_check=0;
//			RCC->BDCR|=RCC_BDCR_BDRST;
//
//					goto reinit;
//  	  	TIM1->DIER |= TIM_DIER_UIE;
//  	  	TIM3->DIER |= (TIM_DIER_CC1IE | TIM_DIER_CC1DE);//Enable TIM1_INT
//  	  	UART2_Enable_Receive_Interrupt;
//  	  	UART1_Enable_Receive_Interrupt;
//
//				//Enable TIM3_Capture_Int
//  	  }

	  UART1_Store_Received_Data_In_Variables();
	  UART1_Request_Send_Message();

//	  UART2_Store_Received_Data_In_Variables();
//	  UART2_Request_Send_Message();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI14_Enable();

   /* Wait till HSI14 is ready */
  while(LL_RCC_HSI14_IsReady() != 1)
  {

  }
  LL_RCC_HSI14_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_12);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(48000000);
  LL_SetSystemCoreClock(48000000);
  LL_RCC_HSI14_EnableADCControl();
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
