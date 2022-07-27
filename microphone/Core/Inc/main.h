/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */
    typedef struct
    {
        float x;
        float y;
        /* data */
    } vector2;

    typedef struct
    {
        float v00;
        float v01;
        float v10;
        float v11;
    } matrix22;

    /* USER CODE END ET */

    /* Exported constants --------------------------------------------------------*/
    /* USER CODE BEGIN EC */

    /* USER CODE END EC */

    /* Exported macro ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */
    extern int riseTimes;
    extern uint32_t mp_timestamp[4];
    void arg_prase(int argc, char **argv);
    void microphone_init();
    void time_init();
    matrix22 inv(const matrix22 *matrix);
    vector2 vector2_dot_matrix22(const vector2 *v, const matrix22 *matrix);
    matrix22 matrix_plus(const matrix22 *m, float value);
    /* USER CODE END EM */

    /* Exported functions prototypes ---------------------------------------------*/
    void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MP1_Pin GPIO_PIN_0
#define MP1_GPIO_Port GPIOD
#define MP1_EXTI_IRQn EXTI0_IRQn
#define MP2_Pin GPIO_PIN_1
#define MP2_GPIO_Port GPIOD
#define MP2_EXTI_IRQn EXTI1_IRQn
#define MP3_Pin GPIO_PIN_2
#define MP3_GPIO_Port GPIOD
#define MP3_EXTI_IRQn EXTI2_IRQn
#define MP4_Pin GPIO_PIN_3
#define MP4_GPIO_Port GPIOD
#define MP4_EXTI_IRQn EXTI3_IRQn
    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
