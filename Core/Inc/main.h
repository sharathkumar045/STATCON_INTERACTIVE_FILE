/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32f0xx_ll_adc.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_crs.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Earth_fault_Pin LL_GPIO_PIN_0
#define Earth_fault_GPIO_Port GPIOC

//#define Earth_Check_ADC LL_GPIO_PIN_2
//#define Earth_Check_ADC_GPIO_Port GPIOC

#define Grid_DC_Component_Pin LL_GPIO_PIN_1
#define Grid_DC_Component_GPIO_Port GPIOC
#define Hall_Sensor_Vref_Pin LL_GPIO_PIN_0
#define Hall_Sensor_Vref_GPIO_Port GPIOA
#define PV_Amp_Fast_Pin LL_GPIO_PIN_1
#define PV_Amp_Fast_GPIO_Port GPIOA
#define Mains_Relay_Pin LL_GPIO_PIN_4
#define Mains_Relay_GPIO_Port GPIOF
#define MPPT_OC_Pin LL_GPIO_PIN_5
#define MPPT_OC_GPIO_Port GPIOF
#define Inv_Pri_AMp_Pin LL_GPIO_PIN_4
#define Inv_Pri_AMp_GPIO_Port GPIOA
#define PV_Volt_Fast_Pin LL_GPIO_PIN_5
#define PV_Volt_Fast_GPIO_Port GPIOA
#define DC_Bus_Slow_Pin LL_GPIO_PIN_6
#define DC_Bus_Slow_GPIO_Port GPIOA
#define Mains_Volt_Pin LL_GPIO_PIN_7
#define Mains_Volt_GPIO_Port GPIOA
#define Mains_Amp_Pin LL_GPIO_PIN_4
#define Mains_Amp_GPIO_Port GPIOC
#define DC_Insulation_Pin LL_GPIO_PIN_5
#define DC_Insulation_GPIO_Port GPIOC
#define Inv_Temp_Pin LL_GPIO_PIN_0
#define Inv_Temp_GPIO_Port GPIOB
#define MPPT_PWM_Pin LL_GPIO_PIN_1
#define MPPT_PWM_GPIO_Port GPIOB
#define Fan_On_Pin LL_GPIO_PIN_2
#define Fan_On_GPIO_Port GPIOB
#define DC_Bus_OV_Pin LL_GPIO_PIN_10
#define DC_Bus_OV_GPIO_Port GPIOB
#define Driver_Supply_Ok_Pin LL_GPIO_PIN_12
#define Driver_Supply_Ok_GPIO_Port GPIOB
#define Inv_Temp_Fail_Pin LL_GPIO_PIN_13
#define Inv_Temp_Fail_GPIO_Port GPIOB
#define PWM_1L_Pin LL_GPIO_PIN_14
#define PWM_1L_GPIO_Port GPIOB
#define PWM_2L_Pin LL_GPIO_PIN_15
#define PWM_2L_GPIO_Port GPIOB
#define ZC_Pin LL_GPIO_PIN_6
#define ZC_GPIO_Port GPIOC
#define PWM_1H_Pin LL_GPIO_PIN_9
#define PWM_1H_GPIO_Port GPIOA
#define PWM_2H_Pin LL_GPIO_PIN_10
#define PWM_2H_GPIO_Port GPIOA
#define Switch_Pin LL_GPIO_PIN_11
#define Switch_GPIO_Port GPIOA
#define PFC_Power_ON_Pin LL_GPIO_PIN_12
#define PFC_Power_ON_GPIO_Port GPIOA
#define RS485_Bus_Ctrl_Pin LL_GPIO_PIN_5
#define RS485_Bus_Ctrl_GPIO_Port GPIOB
#define LED3_Pin LL_GPIO_PIN_9
#define LED3_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
