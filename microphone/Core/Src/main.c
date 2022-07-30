/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights sound_source_poserved.</center></h2>
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

//#define DISTANCE_LOG
//#define SS_POS_LOG
#define TIMESTAMP_LOG
#define ANGLE_LOG

float SOUND_SPEED = 346.0f;
float TIME_TICK_RATE = 0.5E-3f;

int mp_timestamp[4] = {0};
int mp_ts_cpy[4] = {0};
float delta_distance[3][2] = {0};
float chans_distance[3][2] = {0};
float x_bound[2];
float y_bound[2];
int mc_fix_bias[4] = {0};
vector2 mp_pos[4];
vector2 sound_source_pos;

uint8_t sample_flag = 0;
extern uint8_t mic_bit[4];
uint8_t mic_order_cpy[4];
uint8_t area_bound[6][2];
float angle = 90;
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

    /* sound_source_poset of all peripherals, Initializes the Flash interface and the Systick. */
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
#ifdef WANGCHAORUI
        if ((!mic_bit[0] && !mic_bit[1] && !mic_bit[2] && !mic_bit[3]))
        {
            if (sample_flag)
            {
                uint8_t data_is_value = 1;
                for (int i = 0; i < 4; i++)
                {
                    mp_timestamp[i] += mc_fix_bias[i];
                    if (mp_ts_cpy == mp_timestamp)
                        data_is_value = 0;
                    mp_ts_cpy[i] = mp_timestamp[i];
                }
                calculate_order();
                if (mic_order_cpy[0] == 0 || mic_order_cpy[0] == 2)
                    data_is_value = 0;
                if (data_is_value)
                {
#ifdef TIMESTAMP_LOG
                    print_tick();
                    printf("begin calculate\n");
#endif
                    calculate_angle();
                    calculate_chans_distance();
                    sound_source = calculate_sound_source();
                    print_to_stm32();
                    sample_flag = 0;
                }
            }
        }
#endif
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
    HAL_TIM_Base_Start(&htim2);
    __HAL_TIM_SetCounter(&htim2, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}
inline void microphone_init()
{
    mp_pos[1].x = -250;
    mp_pos[1].y = 0;
    mp_pos[3].x = 250;
    mp_pos[3].y = 0;
    mp_pos[0].x = -250;
    mp_pos[0].y = -500;
    mp_pos[2].x = 250;
    mp_pos[2].y = -500;

    x_bound[0] = -1500;
    x_bound[1] = 1500;
    y_bound[0] = -5000;
    y_bound[1] = 5000;

    mc_fix_bias[3] = 2000;
    mc_fix_bias[2] = 500;
}
inline void calculate_order()
{
    int tem[4];
    for (int i = 0; i < 4; i++)
    {
        tem[i] = 0;
        int value = mp_ts_cpy[i];
        for (int j = 0; j < 4; j++)
        {
            if (value > mp_ts_cpy[j])
                tem[i]++;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        mic_order_cpy[tem[i]] = i;
    }
}
void print_tick()
{
    // log
    printf("mp_order:");
    for (int i = 0; i < 4; i++)
        printf("%d", mic_order_cpy[i]);
    printf("\n");
    for (int i = 0; i < 4; i++)
        printf("mp_timestamp[%d] = %d\n", i, mp_ts_cpy[i]);
    printf("\n");
}
void print_to_stm32()
{
    printf_huart = &huart2;
    char temp[128];
    sprintf(temp, "angle %.5f posx %.5f posy %.5f", angle, sound_source_pos.x, sound_source_pos.y);
    printf("%s", temp);
    printf_huart = &huart3;
}
inline float get_det_d(int ts1, int ts2)
{
    return ((int)(ts1 - ts2) * TIME_TICK_RATE * SOUND_SPEED);
}
inline float get_det_t(int ts1, int ts2)
{
    return ((int)(ts1 - ts2) * TIME_TICK_RATE);
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
void calculate_angle()
{
    float dt1 = (get_det_d(mp_ts_cpy[2], mp_ts_cpy[0]));
    float dt2 = (get_det_d(mp_ts_cpy[3], mp_ts_cpy[1]));
    if (dt1 > 500 && dt2 < 500)
        dt1 = dt2;
    if (dt2 > 500 && dt1 < 500)
        dt2 = dt1;

    float value1 = (180.f / 3.141592f) * acosf(dt1 / 500);
    float value2 = (180.f / 3.141592f) * acosf(dt2 / 500);
    if ((value1 - value2) < 10)
    {
        angle = (value1 + value2) / 2;
    }
#ifdef ANGLE_LOG
    printf("dt %f %f\n value %f %f\n, angle :%f\n", dt1, dt2, value1, value2, angle);
#endif
}
void calculate_chans_distance()
{

    chans_distance[0][0] = get_det_d(mp_ts_cpy[mic_order_cpy[0]], mp_ts_cpy[mic_order_cpy[1]]);
    chans_distance[0][1] = get_det_d(mp_ts_cpy[mic_order_cpy[0]], mp_ts_cpy[mic_order_cpy[2]]);

    chans_distance[1][0] = get_det_d(mp_ts_cpy[mic_order_cpy[0]], mp_ts_cpy[mic_order_cpy[1]]);
    chans_distance[1][1] = get_det_d(mp_ts_cpy[mic_order_cpy[0]], mp_ts_cpy[mic_order_cpy[3]]);

    chans_distance[2][0] = get_det_d(mp_ts_cpy[mic_order_cpy[0]], mp_ts_cpy[mic_order_cpy[2]]);
    chans_distance[2][1] = get_det_d(mp_ts_cpy[mic_order_cpy[0]], mp_ts_cpy[mic_order_cpy[3]]);
#ifdef DISTANCE_LOG
    for (int i = 0; i < 3; i++)
    {
        printf("cd%d:%f %f\n", i, chans_distance[i][0], chans_distance[i][1]);
    }
    printf("\n");
#endif
}

inline uint8_t is_within_bound(vector2 v)
{
    if (v.x < x_bound[1] && v.x > x_bound[0] && v.y < y_bound[1] && v.y > y_bound[0])
        return 1;

    return 0;
}

#define CHANS_METHOD
vector2 calculate_sound_source()
{
#ifdef CHANS_METHOD
    vector2 sp[3];
    sp[0] = chans_meth(mp_pos[mic_order_cpy[0]], mp_pos[mic_order_cpy[1]], mp_pos[mic_order_cpy[2]], chans_distance[0][0], chans_distance[0][1]);
    sp[1] = chans_meth(mp_pos[mic_order_cpy[0]], mp_pos[mic_order_cpy[1]], mp_pos[mic_order_cpy[3]], chans_distance[1][0], chans_distance[1][1]);
    sp[2] = chans_meth(mp_pos[mic_order_cpy[0]], mp_pos[mic_order_cpy[2]], mp_pos[mic_order_cpy[3]], chans_distance[2][0], chans_distance[2][1]);

    int aa = 0;
    vector2 sum = {0};
    for (int i = 0; i < 3; i++)
    {
        if (is_within_bound(sp[i]))
        {
            aa++;
            sum = vector2_add_vector2(sum, sp[i]);
        }
    }
    if (aa)
        sound_source_pos = vector2_plus(sum, (1.0f / aa));
#ifdef SS_POS_LOG
    printf("ss pos:");
    print_vector(sound_source_pos);
#endif

#endif

    return sound_source_pos;
}
int is_str_equal(char *str_p1, uint8_t size1, char *str_p2)
{
    if (size1 == 0)
        return 0;
    for (int i = 0; i < size1; i++)
    {
        if (str_p1[i] != str_p2[i])
            return 0;
    }
    return 1;
}

void arg_prase(int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
    {
        if (is_str_equal("tickrate", 8, argv[i]))
        {
            TIME_TICK_RATE = atof(argv[++i]);
            printf("set TIME_TICK_RATE :%.9f", TIME_TICK_RATE);
        }
        else if (is_str_equal("soundspeed", 10, argv[i]))
        {
            SOUND_SPEED = atof(argv[++i]);
            printf("set SOUND_SPEED :%5f", SOUND_SPEED);
        }
        else if (is_str_equal("add0bias", 8, argv[i]))
        {
            mc_fix_bias[0] = atoi(argv[++i]);
            printf("set 0bias:%d\n", mc_fix_bias[0]);
        }
        else if (is_str_equal("add1bias", 8, argv[i]))
        {
            mc_fix_bias[1] = atoi(argv[++i]);
            printf("set 0bias:%d\n", mc_fix_bias[1]);
        }
        else if (is_str_equal("add2bias", 8, argv[i]))
        {
            mc_fix_bias[2] = atoi(argv[++i]);
            printf("set 0bias:%d\n", mc_fix_bias[2]);
        }
        else if (is_str_equal("add3bias", 8, argv[i]))
        {
            mc_fix_bias[3] = atoi(argv[++i]);
            printf("set 0bias:%d\n", mc_fix_bias[3]);
        }
    }
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
