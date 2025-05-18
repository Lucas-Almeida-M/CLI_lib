/*
 * uart_app.c
 *
 *  Created on: May 18, 2025
 *      Author: lucasalmeida
 */

#include "uart_app.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "queue.h"

uint8_t rx_buffer[RX_BUF_SIZE];
uint8_t main_buffer[MAIN_BUF_SIZE];
MSG msg = { 0 };

extern UART_HandleTypeDef UART_CLI_HANDLER;
extern DMA_HandleTypeDef UART_CLI_HDMA_RX_HANDLER;

extern osMessageQueueId_t CLI_QUEUE_HANDLER;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART2)
	{
		BaseType_t xHigherPriorityTaskWokenByPost;
		memset(&msg, 0, sizeof(msg));
		memcpy(msg.buffer, rx_buffer, Size);
		msg.len = Size;

		xQueueGenericSendFromISR(CLI_QUEUE_HANDLER, &msg, &xHigherPriorityTaskWokenByPost,
				queueSEND_TO_BACK);
		HAL_UARTEx_ReceiveToIdle_DMA(&UART_CLI_HANDLER, rx_buffer, RX_BUF_SIZE);

		__HAL_DMA_DISABLE_IT(&UART_CLI_HDMA_RX_HANDLER, DMA_IT_HT);
	}
}

void init_uart_APP(void)
{
	HAL_UARTEx_ReceiveToIdle_DMA(&UART_CLI_HANDLER, rx_buffer, RX_BUF_SIZE);
	__HAL_DMA_DISABLE_IT(&UART_CLI_HDMA_RX_HANDLER, DMA_IT_HT);
}

