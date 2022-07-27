/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define SOUND_SPEED 346.0f
#define TIME_TICK_RATE 1.19047619E-5f
int riseTimes = 0;
uint32_t mp_timestamp[4] = {0};
float delta_distance[3][2] = {0};
float chans_distance[1][2] = {0};
vector2 mp_pos[4];
vector2 sound_source_pos;
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    /* USER CODE BEGIN 2 */
    time_init();
    microphone_init();
    printf("Init\n");
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
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
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
matrix22 inv(const matrix22 *matrix)
{
    matrix22 ret;
    float abs = matrix->v00 * matrix->v11 - matrix->v01 * matrix->v10;
    if (abs == 0)
        abs = 1.0f;
    ret.v00 = matrix->v00 / abs;
    ret.v00 = matrix->v00 / abs;
    ret.v00 = matrix->v00 / abs;
    ret.v00 = matrix->v00 / abs;
    return ret;
}

vector2 vector2_dot_matrix22(const vector2 *v, const matrix22 *matrix)
{
    vector2 ret;
    ret.x = v->x * matrix->v00 + v->y * matrix->v01;
    ret.y = v->x * matrix->v01 + v->y * matrix->v11;
    return ret;
}

matrix22 matrix_plus(const matrix22 *m, float value)
{
    matrix22 ret;
    ret.v00 = m->v00 * value;
    ret.v01 = m->v01 * value;
    ret.v10 = m->v10 * value;
    ret.v11 = m->v11 * value;
    return ret;
}

inline void time_init()
{
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_Base_Start(&htim2);
}
inline void microphone_init()
{
    mp_pos[0].x = -25;
    mp_pos[0].y = 0;
    mp_pos[1].x = 25;
    mp_pos[1].y = 0;
    mp_pos[2].x = -25;
    mp_pos[2].y = -15.5;
    mp_pos[3].x = 25;
    mp_pos[3].y = -15.5;
}

void calculate_delta_distance()
{
    delta_distance[0][0] = abs((mp_timestamp[0] - mp_timestamp[1]) * TIME_TICK_RATE * SOUND_SPEED);
    delta_distance[0][1] = abs((mp_timestamp[2] - mp_timestamp[3]) * TIME_TICK_RATE * SOUND_SPEED);

    delta_distance[1][0] = abs((mp_timestamp[0] - mp_timestamp[2]) * TIME_TICK_RATE * SOUND_SPEED);
    delta_distance[1][1] = abs((mp_timestamp[1] - mp_timestamp[3]) * TIME_TICK_RATE * SOUND_SPEED);

    delta_distance[2][0] = abs((mp_timestamp[0] - mp_timestamp[3]) * TIME_TICK_RATE * SOUND_SPEED);
    delta_distance[2][1] = abs((mp_timestamp[1] - mp_timestamp[2]) * TIME_TICK_RATE * SOUND_SPEED);
}

void calculate_chans_distance()
{
    chans_distance[0][0] = abs((mp_timestamp[0] - mp_timestamp[1]) * TIME_TICK_RATE * SOUND_SPEED);
    chans_distance[0][1] = abs((mp_timestamp[0] - mp_timestamp[2]) * TIME_TICK_RATE * SOUND_SPEED);
}
void calculate_sound_source()
{
}

void chans_method()
{
    float x10 = mp_pos[1].x - mp_pos[0].x;
    float y10 = mp_pos[1].y - mp_pos[0].y;
    float x20 = mp_pos[2].x - mp_pos[0].x;
    float y20 = mp_pos[2].y - mp_pos[0].y;
    matrix22 P1;
    P1.v00 = x10;
    P1.v01 = y10;
    P1.v10 = x20;
    P1.v11 = y20;
    P1 = inv(&P1);
    P1 = matrix_plus(&P1, -1.0f);
}
void arg_prase(int argc, char **argv)
{
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
