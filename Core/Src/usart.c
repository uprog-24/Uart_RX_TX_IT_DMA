/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
uint8_t uart1_rx_cnt = 0; // Receive buffer count

volatile bool is_data_ready_to_be_read = false;
volatile bool is_rx_full_completed = false;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		is_data_ready_to_be_read = true;
		is_rx_full_completed = true;
	}
}

volatile bool is_tx_completed = false;
volatile bool is_tx_full_completed = false;
/* Регистр TXE пуст */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		is_tx_completed = true;
		is_tx_full_completed = true;
	}
}

volatile bool is_rx_half_completed = false;
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		is_rx_half_completed = true;
	}
}

volatile bool is_tx_half_completed = true;
void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		is_tx_half_completed = true;
	}
}
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle) {

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	if (uartHandle->Instance == USART1) {
		/* USER CODE BEGIN USART1_MspInit 0 */

		/* USER CODE END USART1_MspInit 0 */
		/* USART1 clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART1 GPIO Configuration
		 PA9     ------> USART1_TX
		 PA10     ------> USART1_RX
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* USART1 DMA Init */
		/* USART1_RX Init */
		hdma_usart1_rx.Instance = DMA1_Channel3;
		hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
		hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
		if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK) {
			Error_Handler();
		}

		__HAL_LINKDMA(uartHandle, hdmarx, hdma_usart1_rx);

		/* USART1_TX Init */
		hdma_usart1_tx.Instance = DMA1_Channel2;
		hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart1_tx.Init.Mode = DMA_CIRCULAR;
		hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
		if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK) {
			Error_Handler();
		}

		__HAL_LINKDMA(uartHandle, hdmatx, hdma_usart1_tx);

		/* USART1 interrupt Init */
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		/* USER CODE BEGIN USART1_MspInit 1 */

		/* USER CODE END USART1_MspInit 1 */
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle) {

	if (uartHandle->Instance == USART1) {
		/* USER CODE BEGIN USART1_MspDeInit 0 */

		/* USER CODE END USART1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration
		 PA9     ------> USART1_TX
		 PA10     ------> USART1_RX
		 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

		/* USART1 DMA DeInit */
		HAL_DMA_DeInit(uartHandle->hdmarx);
		HAL_DMA_DeInit(uartHandle->hdmatx);

		/* USART1 interrupt Deinit */
		HAL_NVIC_DisableIRQ(USART1_IRQn);
		/* USER CODE BEGIN USART1_MspDeInit 1 */

		/* USER CODE END USART1_MspDeInit 1 */
	}
}

/* USER CODE BEGIN 1 */
void receive_data_from_uart(char *rx_buff) {
	char rx_byte = ' ';
	char cnt;
	uint8_t isDataEnd = 0;

	memset(rx_buff, 0, strlen(rx_buff));
	uart1_rx_cnt = 0;
	while (!isDataEnd) {
		HAL_UART_Receive_IT(&huart1, (uint8_t*) &rx_byte, 1);

		if (is_data_ready_to_be_read) {
			is_data_ready_to_be_read = false;

			rx_buff[uart1_rx_cnt++] = rx_byte; // Receive byte

			if ((rx_buff[uart1_rx_cnt - 1] == 0x0A)
					&& (rx_buff[uart1_rx_cnt - 2] == 0x0D)) { // Chars end: CR+LF
				cnt = uart1_rx_cnt + '0';
				isDataEnd = 1;
			}
		}

	}

}

uint32_t index_byte = 0;
void receive_data_from_uart_dma(char *rx_buff, uint16_t rx_buff_len,
		char *final_buff, uint16_t final_buff_len) {

	uint16_t rx_half_len = rx_buff_len / 2;
	index_byte = 0;

	HAL_UART_Receive_DMA(&huart1, (uint8_t*) rx_buff, rx_buff_len);

	if (is_rx_half_completed) {
		is_rx_half_completed = false;

		memcpy(final_buff + index_byte, rx_buff, rx_half_len);
		memset(rx_buff, '\0', rx_half_len);
		index_byte += rx_half_len;
	}

	if (is_rx_full_completed) {
		is_rx_full_completed = false;

		memcpy(final_buff + index_byte, rx_buff + index_byte, rx_half_len);
		memset(rx_buff + rx_half_len, '\0', rx_half_len);
		index_byte += rx_half_len;
	}

}

uint32_t index_tx_byte = 0;
void transmit_data_to_uart_dma(char *start_buff, uint16_t start_buff_len,
		char *tx_buff, uint16_t tx_buff_len) {

	while (index_tx_byte < start_buff_len) {
		is_tx_full_completed = false;

		memset(tx_buff, '\0', tx_buff_len);
		memcpy(tx_buff, start_buff + index_tx_byte, tx_buff_len);
		HAL_UART_Transmit_DMA(&huart1, (uint8_t*) tx_buff, tx_buff_len);
		while (!is_tx_full_completed) {
		}
		HAL_UART_DMAStop(&huart1);
		index_tx_byte += tx_buff_len;
	}

	if (index_tx_byte == start_buff_len) {
		index_tx_byte = 0;
		HAL_UART_DMAStop(&huart1);
		memset(tx_buff, '\0', tx_buff_len);
	}
}

void transmit_data_to_uart(char *tx_buff) {
	is_tx_completed = false;
	HAL_UART_Transmit_IT(&huart1, (uint8_t*) tx_buff, strlen(tx_buff));
	while (!is_tx_completed) {
	}
}

/* USER CODE END 1 */
