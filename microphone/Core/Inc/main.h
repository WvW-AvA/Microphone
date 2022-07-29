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
#include "dtoa.h"
    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */
#define QUEUE_SIZE 1000
#define WINDOW_SIZE 500

    typedef struct _queue
    {
        uint8_t data[QUEUE_SIZE];
        uint8_t front, back;
    } Queue;
    /* USER CODE END ET */

    /* Exported constants --------------------------------------------------------*/
    /* USER CODE BEGIN EC */

    /* USER CODE END EC */

    /* Exported macro ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */
    extern int riseTimes;
    extern int mp_timestamp[4];
    extern float delta_distance[3][2];
    extern float chans_distance[3][2];
    extern vector2 mp_pos[4];
    extern vector2 sound_source_pos;

    void arg_prase(int argc, char **argv);
    void microphone_init();
    void time_init();
    void calculate_delta_distance();
    void calculate_chans_distance();
    vector2 calculate_sound_source();
    float get_det_d(int ts1, int ts2);

    /* USER CODE END EM */

    /* Exported functions prototypes ---------------------------------------------*/
    void Error_Handler(void);

    /* USER CODE BEGIN EFP */
    void push(Queue *q, uint8_t x);
    uint8_t pop(Queue *q);
    uint8_t size(Queue *q);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MIC0_Pin GPIO_PIN_0
#define MIC0_GPIO_Port GPIOD
#define MIC1_Pin GPIO_PIN_1
#define MIC1_GPIO_Port GPIOD
#define MIC2_Pin GPIO_PIN_2
#define MIC2_GPIO_Port GPIOD
#define MIC3_Pin GPIO_PIN_3
#define MIC3_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
