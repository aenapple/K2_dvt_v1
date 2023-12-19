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
#define MOTOR_ON_Pin GPIO_PIN_11
#define MOTOR_ON_GPIO_Port GPIOC
#define PULSE_ON_Pin GPIO_PIN_12
#define PULSE_ON_GPIO_Port GPIOC
#define AC_ON_Pin GPIO_PIN_14
#define AC_ON_GPIO_Port GPIOC
#define PULSE_RST_Pin GPIO_PIN_15
#define PULSE_RST_GPIO_Port GPIOC
#define PAD1_ON_Pin GPIO_PIN_0
#define PAD1_ON_GPIO_Port GPIOC
#define PAD2_ON_Pin GPIO_PIN_1
#define PAD2_ON_GPIO_Port GPIOC
#define PTC1_ON_Pin GPIO_PIN_2
#define PTC1_ON_GPIO_Port GPIOC
#define PTC2_ON_Pin GPIO_PIN_3
#define PTC2_ON_GPIO_Port GPIOC
#define PTC1_FAN2_Pin GPIO_PIN_6
#define PTC1_FAN2_GPIO_Port GPIOA
#define PTC2_FAN2_Pin GPIO_PIN_7
#define PTC2_FAN2_GPIO_Port GPIOA
#define S_ROTATION_Pin GPIO_PIN_0
#define S_ROTATION_GPIO_Port GPIOB
#define RED_LED_Pin GPIO_PIN_11
#define RED_LED_GPIO_Port GPIOB
#define TOP_REMOVED_Pin GPIO_PIN_12
#define TOP_REMOVED_GPIO_Port GPIOB
#define TOP_PRESENT_Pin GPIO_PIN_13
#define TOP_PRESENT_GPIO_Port GPIOB
#define FAN_PWM_Pin GPIO_PIN_14
#define FAN_PWM_GPIO_Port GPIOB
#define FAN_RPM_Pin GPIO_PIN_15
#define FAN_RPM_GPIO_Port GPIOB
#define FAN_RPM_EXTI_IRQn EXTI4_15_IRQn
#define WC_EEPROM_Pin GPIO_PIN_6
#define WC_EEPROM_GPIO_Port GPIOC
#define PRESENT_CHM1_Pin GPIO_PIN_8
#define PRESENT_CHM1_GPIO_Port GPIOD
#define PRESENT_CHM2_Pin GPIO_PIN_9
#define PRESENT_CHM2_GPIO_Port GPIOD
#define TOP_RST_Pin GPIO_PIN_8
#define TOP_RST_GPIO_Port GPIOC
#define SW_STATOR1_Pin GPIO_PIN_9
#define SW_STATOR1_GPIO_Port GPIOC
#define ON_CHM1_MOT_Pin GPIO_PIN_0
#define ON_CHM1_MOT_GPIO_Port GPIOD
#define DIR_CHM1_MOT_Pin GPIO_PIN_1
#define DIR_CHM1_MOT_GPIO_Port GPIOD
#define ON_CHM2_MOT_Pin GPIO_PIN_2
#define ON_CHM2_MOT_GPIO_Port GPIOD
#define DIR_CHM2_MOT_Pin GPIO_PIN_3
#define DIR_CHM2_MOT_GPIO_Port GPIOD
#define BREAK_ON_Pin GPIO_PIN_4
#define BREAK_ON_GPIO_Port GPIOD
#define PRESENT_TANK_Pin GPIO_PIN_6
#define PRESENT_TANK_GPIO_Port GPIOD
#define LID_OPEN_Pin GPIO_PIN_5
#define LID_OPEN_GPIO_Port GPIOB
#define AC_MAIN_Pin GPIO_PIN_6
#define AC_MAIN_GPIO_Port GPIOB
#define AC_MAIN_EXTI_IRQn EXTI4_15_IRQn
#define AC_MOTOR_Pin GPIO_PIN_7
#define AC_MOTOR_GPIO_Port GPIOB
#define AC_MOTOR_EXTI_IRQn EXTI4_15_IRQn
#define PTC1_FAN_Pin GPIO_PIN_8
#define PTC1_FAN_GPIO_Port GPIOB
#define PTC2_FAN_Pin GPIO_PIN_9
#define PTC2_FAN_GPIO_Port GPIOB
#define SW_STATOR2_Pin GPIO_PIN_10
#define SW_STATOR2_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
