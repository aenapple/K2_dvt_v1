/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ON_BACK_LAMP_Pin GPIO_PIN_11
#define ON_BACK_LAMP_GPIO_Port GPIOC
#define ON_FRONT_LAMP_Pin GPIO_PIN_12
#define ON_FRONT_LAMP_GPIO_Port GPIOC
#define FAN_RPM_Pin GPIO_PIN_13
#define FAN_RPM_GPIO_Port GPIOC
#define FAN_RPM_EXTI_IRQn EXTI4_15_IRQn
#define LOCK_LEFT_Pin GPIO_PIN_14
#define LOCK_LEFT_GPIO_Port GPIOC
#define LOCK_RIGHT_Pin GPIO_PIN_15
#define LOCK_RIGHT_GPIO_Port GPIOC
#define IN_BACK_LAMP_Pin GPIO_PIN_0
#define IN_BACK_LAMP_GPIO_Port GPIOC
#define IN_BACK_LAMP_EXTI_IRQn EXTI0_1_IRQn
#define IN_FRONT_LAMP_Pin GPIO_PIN_1
#define IN_FRONT_LAMP_GPIO_Port GPIOC
#define IN_FRONT_LAMP_EXTI_IRQn EXTI0_1_IRQn
#define MOTOR_INB_Pin GPIO_PIN_12
#define MOTOR_INB_GPIO_Port GPIOB
#define MOTOR_INA_Pin GPIO_PIN_13
#define MOTOR_INA_GPIO_Port GPIOB
#define DAM_RIGHT_Pin GPIO_PIN_14
#define DAM_RIGHT_GPIO_Port GPIOB
#define DAM_LEFT_Pin GPIO_PIN_15
#define DAM_LEFT_GPIO_Port GPIOB
#define RED_LED_Pin GPIO_PIN_7
#define RED_LED_GPIO_Port GPIOC
#define PROG3_Pin GPIO_PIN_9
#define PROG3_GPIO_Port GPIOC
#define RESET3_Pin GPIO_PIN_0
#define RESET3_GPIO_Port GPIOD
#define INT3_Pin GPIO_PIN_1
#define INT3_GPIO_Port GPIOD
#define PROG2_Pin GPIO_PIN_2
#define PROG2_GPIO_Port GPIOD
#define RESET2_Pin GPIO_PIN_3
#define RESET2_GPIO_Port GPIOD
#define INT2_Pin GPIO_PIN_4
#define INT2_GPIO_Port GPIOD
#define PROG1_Pin GPIO_PIN_5
#define PROG1_GPIO_Port GPIOD
#define RESET1_Pin GPIO_PIN_6
#define RESET1_GPIO_Port GPIOD
#define INT1_Pin GPIO_PIN_3
#define INT1_GPIO_Port GPIOB
#define FAN_PWM_Pin GPIO_PIN_9
#define FAN_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
