/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g4xx_hal.h"

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
#define ENC2_A_Pin GPIO_PIN_13
#define ENC2_A_GPIO_Port GPIOC
#define ENC2_A_EXTI_IRQn EXTI15_10_IRQn
#define ENC1_B_Pin GPIO_PIN_0
#define ENC1_B_GPIO_Port GPIOA
#define ENC1_B_EXTI_IRQn EXTI0_IRQn
#define ENC1_A_Pin GPIO_PIN_1
#define ENC1_A_GPIO_Port GPIOA
#define ENC1_A_EXTI_IRQn EXTI1_IRQn
#define LIM3_2_Pin GPIO_PIN_2
#define LIM3_2_GPIO_Port GPIOA
#define LIM3_2_EXTI_IRQn EXTI2_IRQn
#define LIM3_1_Pin GPIO_PIN_3
#define LIM3_1_GPIO_Port GPIOA
#define LIM3_1_EXTI_IRQn EXTI3_IRQn
#define LIM1_1_Pin GPIO_PIN_10
#define LIM1_1_GPIO_Port GPIOB
#define LIM1_1_EXTI_IRQn EXTI15_10_IRQn
#define LIM2_1_Pin GPIO_PIN_11
#define LIM2_1_GPIO_Port GPIOB
#define LIM2_1_EXTI_IRQn EXTI15_10_IRQn
#define LIM1_2_Pin GPIO_PIN_12
#define LIM1_2_GPIO_Port GPIOB
#define LIM1_2_EXTI_IRQn EXTI15_10_IRQn
#define LIM4_1_Pin GPIO_PIN_14
#define LIM4_1_GPIO_Port GPIOB
#define LIM4_1_EXTI_IRQn EXTI15_10_IRQn
#define LIM4_2_Pin GPIO_PIN_15
#define LIM4_2_GPIO_Port GPIOB
#define LIM4_2_EXTI_IRQn EXTI15_10_IRQn
#define LIM2_2_Pin GPIO_PIN_8
#define LIM2_2_GPIO_Port GPIOA
#define LIM2_2_EXTI_IRQn EXTI9_5_IRQn
#define LED_1_Pin GPIO_PIN_9
#define LED_1_GPIO_Port GPIOA
#define LED_2_Pin GPIO_PIN_10
#define LED_2_GPIO_Port GPIOA
#define ENC4_B_Pin GPIO_PIN_4
#define ENC4_B_GPIO_Port GPIOB
#define ENC4_B_EXTI_IRQn EXTI4_IRQn
#define ENC4_A_Pin GPIO_PIN_5
#define ENC4_A_GPIO_Port GPIOB
#define ENC4_A_EXTI_IRQn EXTI9_5_IRQn
#define ENC3_B_Pin GPIO_PIN_6
#define ENC3_B_GPIO_Port GPIOB
#define ENC3_B_EXTI_IRQn EXTI9_5_IRQn
#define ENC3_A_Pin GPIO_PIN_7
#define ENC3_A_GPIO_Port GPIOB
#define ENC3_A_EXTI_IRQn EXTI9_5_IRQn
#define ENC2_B_Pin GPIO_PIN_9
#define ENC2_B_GPIO_Port GPIOB
#define ENC2_B_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
