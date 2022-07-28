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
#include "math.h"
#include "dtoa.h"
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
#define TIME_TICK_RATE 5.59238092E-6f
int mp_timestamp[4] = {0};
float delta_distance[3][2] = {0};
float chans_distance[3][2] = {0};
vector2 mp_pos[4];
vector2 sound_source_pos;

uint8_t sample_flag = 0;
extern uint8_t mic_cur[4];

extern uint8_t order;
extern uint8_t mic_order[4];

void push(Queue *q, uint8_t x)
{
    q->data[q->front++] = x;
    q->front %= QUEUE_SIZE;
}

uint8_t pop(Queue *q)
{
    if (q->back == q->front)
        return -1;
    uint8_t ret = q->data[q->back++];
    q->back %= QUEUE_SIZE;
    return ret;
}

uint8_t size(Queue *q)
{
    return (q->front - q->back + QUEUE_SIZE) % QUEUE_SIZE;
}
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
    MX_TIM10_Init();
    /* USER CODE BEGIN 2 */
    time_init();
    microphone_init();
    printf("Init\n");
    vector2 sound_source;
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        if ((mic_cur[0] * mic_cur[1] * mic_cur[2] * mic_cur[3]))
        {
            order = 0;
            if (sample_flag)
            {
                sample_flag = 0;
                printf("begin calculate\n");
                calculate_chans_distance();
                sound_source = calculate_sound_source();
                printf("ss pos:");
                print_vector(sound_source);
            }
        }
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

inline void time_init()
{
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_Base_Start(&htim10);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
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

inline float get_det_d(float ts1, float ts2)
{
    return ((int)(ts1 - ts2) * TIME_TICK_RATE * SOUND_SPEED);
}

void calculate_delta_distance()
{
    delta_distance[0][0] = get_det_d(mp_timestamp[0], mp_timestamp[1]);
    delta_distance[0][1] = get_det_d(mp_timestamp[2], mp_timestamp[3]);

    delta_distance[1][0] = get_det_d(mp_timestamp[0], mp_timestamp[2]);
    delta_distance[1][1] = get_det_d(mp_timestamp[1], mp_timestamp[3]);

    delta_distance[2][0] = get_det_d(mp_timestamp[0], mp_timestamp[3]);
    delta_distance[2][1] = get_det_d(mp_timestamp[1], mp_timestamp[2]);
}

void calculate_chans_distance()
{

    chans_distance[0][0] = get_det_d(mp_timestamp[mic_order[0]], mp_timestamp[mic_order[1]]);
    chans_distance[0][1] = get_det_d(mp_timestamp[mic_order[0]], mp_timestamp[mic_order[2]]);

    chans_distance[1][0] = get_det_d(mp_timestamp[mic_order[0]], mp_timestamp[mic_order[1]]);
    chans_distance[1][1] = get_det_d(mp_timestamp[mic_order[0]], mp_timestamp[mic_order[3]]);

    chans_distance[2][1] = get_det_d(mp_timestamp[mic_order[0]], mp_timestamp[mic_order[2]]);
    chans_distance[2][1] = get_det_d(mp_timestamp[mic_order[0]], mp_timestamp[mic_order[3]]);

    for (int i = 0; i < 3; i++)
    {
        printf("cd%d:%f %f ", i, chans_distance[i][0], chans_distance[i][1]);
    }
    printf("\n");
}
#define CHANS_METHOD
vector2 calculate_sound_source()
{
    vector2 res = {0};
#ifdef CHANS_METHOD
    vector2 sp[3];
    sp[0] = chans_meth(mp_pos[mic_order[0]], mp_pos[mic_order[1]], mp_pos[mic_order[2]], chans_distance[0][0], chans_distance[0][1]);
    sp[1] = chans_meth(mp_pos[mic_order[0]], mp_pos[mic_order[1]], mp_pos[mic_order[3]], chans_distance[1][0], chans_distance[1][1]);
    sp[2] = chans_meth(mp_pos[mic_order[0]], mp_pos[mic_order[2]], mp_pos[mic_order[3]], chans_distance[2][0], chans_distance[2][1]);
    for (int i = 0; i < 3; i++)
    {
        res = vector2_add_vector2(res, sp[i]);
        print_vector(sp[i]);
    }
    res = vector2_plus(res, (1.0f / 3.0f));
#endif
    return res;
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
