/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "main.h"
#define STR_MAX_SIZE 32
extern uint8_t tmp;
uint8_t usart1_RxBuffer[4];
uint8_t usart1_RxBufferLen = 0;
uint8_t usart1_RxBufferIndex = 0;
char usart1_TransBuffer[4];
uint8_t usart1_UART_RxBuffer[256];
uint8_t usart1_UART_Rx_Cnt = 0;
uint8_t receive_angle=0;
int receive_x=0;
int receive_y=0;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
USART_HandleTypeDef husart2;
DMA_HandleTypeDef hdma_usart1_rx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	HAL_UART_Receive_IT(&huart1, usart1_UART_RxBuffer, 1);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */
void MX_USART2_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  husart2.Instance = USART2;
  husart2.Init.BaudRate = 115200;
  husart2.Init.WordLength = USART_WORDLENGTH_8B;
  husart2.Init.StopBits = USART_STOPBITS_1;
  husart2.Init.Parity = USART_PARITY_NONE;
  husart2.Init.Mode = USART_MODE_TX_RX;
  husart2.Init.CLKPolarity = USART_POLARITY_LOW;
  husart2.Init.CLKPhase = USART_PHASE_1EDGE;
  husart2.Init.CLKLastBit = USART_LASTBIT_DISABLE;
  if (HAL_USART_Init(&husart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_USART1_ENABLE();

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_USART_MspInit(USART_HandleTypeDef* usartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(usartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    PA4     ------> USART2_CK
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

void HAL_USART_MspDeInit(USART_HandleTypeDef* usartHandle)
{

  if(usartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    PA4     ------> USART2_CK
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);

  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

  if (huart -> Instance == huart1.Instance) {
      static int state; // 0 (^)-> 1 ($)-> 0
      int will_switch = (state == 0 && tmp == '^') || (state == 1 && tmp == '$');
      if (will_switch) {
          if (state == 1) {
              usart1_RxBuffer[usart1_RxBufferIndex] = '\0';
              uint8_t axis = usart1_RxBuffer[0];
              uint8_t *p = &usart1_RxBuffer[1];
              int num = atoi(p);
              if (axis == 'x') receive_x = num;
              else if (axis == 'y') receive_y = num;
          }

          state = !state;
          usart1_RxBufferIndex = 0;
          return;
      }

      if (state)
          usart1_RxBuffer[usart1_RxBufferIndex++] = tmp;
  }
}
//		if(usart1_UART_RxBuffer[0] == 'a'){//&& (usart1_UART_RxBuffer[usart1_UART_Rx_Cnt -1] == 'b') )
//			printf("receive\r\n");
//			Usart1_Data_Detect(usart1_UART_RxBuffer, usart1_UART_Rx_Cnt);
//			usart1_UART_Rx_Cnt = 0;
//			memset(usart1_UART_RxBuffer,'\0',256);
//	    }
//			printf("enter789\r\n");
//			HAL_UART_Receive_IT(&huart1,usart1_RxBuffer,2);
//			printf("enter78932314\r\n");
//	}
//    for (int i = 0; i < len; i++)
//    {
//        if (buffer[i] == ' ' || i == len - 1)
//        {
//            argv[argc] = (char *)malloc(sizeof(char) * (i - tem_p + 1));
//            memcpy(argv[argc], buffer + tem_p, i - tem_p);
//            argv[argc][i - tem_p] = '\0';
//            tem_p = i + 1;
//            argc++;
//            if (argc >= 10)
//                break;
//        }
//    }
//    arg_prase(argc, argv);
//    memset(buffer, 0, len);
//    for (int i = 0; i < argc; i++)
//    {
//        free(argv[i]);
//    }
//    free(argv);
//    HAL_UART_Receive_DMA(huart,buffer, STR_MAX_SIZE);
//		__HAL_UART_CLEAR_IDLEFLAG(huart);


//void Usart1_Data_Detect(uint8_t *usart1_UART_RxBuffer, uint8_t rx_cnt)
//{
//	receive_angle=0;
//	char data_rec[8];
//	
//	uint8_t i,cnt= 0;
//	for(i=rx_cnt-2; i>0; i--)
//	{
//		if(usart1_UART_RxBuffer[i] == 'a')
//		return;
//		else
//		{
//			data_rec[cnt++] = usart1_UART_RxBuffer[rx_cnt-i-1];
//		}
//	}
//	receive_angle = atoi(data_rec);
//	printf("angle is %d\r\n",receive_angle);
//}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
