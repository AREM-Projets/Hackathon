/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>

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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define DIR2_Pin GPIO_PIN_2
#define DIR2_GPIO_Port GPIOB
#define Trig_sonar_Pin GPIO_PIN_10
#define Trig_sonar_GPIO_Port GPIOB
#define PWMD_Pin GPIO_PIN_11
#define PWMD_GPIO_Port GPIOB
#define ledFR_Pin GPIO_PIN_12
#define ledFR_GPIO_Port GPIOB
#define ledBR_Pin GPIO_PIN_14
#define ledBR_GPIO_Port GPIOB
#define ledFL_Pin GPIO_PIN_15
#define ledFL_GPIO_Port GPIOB
#define DIR1_Pin GPIO_PIN_8
#define DIR1_GPIO_Port GPIOC
#define echo_sonar_Pin GPIO_PIN_8
#define echo_sonar_GPIO_Port GPIOA
#define servo_out_Pin GPIO_PIN_11
#define servo_out_GPIO_Port GPIOA
#define ledBL_Pin GPIO_PIN_12
#define ledBL_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define PWMG_Pin GPIO_PIN_15
#define PWMG_GPIO_Port GPIOA
#define ENC1A_Pin GPIO_PIN_4
#define ENC1A_GPIO_Port GPIOB
#define ENC1B_Pin GPIO_PIN_5
#define ENC1B_GPIO_Port GPIOB
#define ENC2A_Pin GPIO_PIN_6
#define ENC2A_GPIO_Port GPIOB
#define ENC2B_Pin GPIO_PIN_7
#define ENC2B_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
