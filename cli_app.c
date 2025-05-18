/*
 * cli_app.c
 *
 *  Created on: May 18, 2025
 *      Author: lucasalmeida
 */

#include <cli_app.h>
#include "uart_app.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "queue.h"

extern UART_HandleTypeDef UART_CLI_HANDLER;
extern osMessageQueueId_t CLI_QUEUE_HANDLER;

typedef StaticTask_t osStaticThreadDef_t;
osThreadId_t CLIHandle;
uint32_t CLIBuffer[1024];
osStaticThreadDef_t CLIControlBlock;
const osThreadAttr_t CLI_attributes = { .name = "CLI", .cb_mem = &CLIControlBlock, .cb_size =
		sizeof(CLIControlBlock), .stack_mem = &CLIBuffer[0], .stack_size = sizeof(CLIBuffer),
		.priority = (osPriority_t) osPriorityNormal7, };
void CLI_task(void *argument);

typedef StaticQueue_t osStaticMessageQDef_t;
osMessageQueueId_t cli_queueHandle;
uint8_t cli_queueBuffer[32 * sizeof(MSG)];
osStaticMessageQDef_t cli_queueControlBlock;
const osMessageQueueAttr_t cli_queue_attributes = { .name = "cli_queue", .cb_mem =
		&cli_queueControlBlock, .cb_size = sizeof(cli_queueControlBlock),
		.mq_mem = &cli_queueBuffer, .mq_size = sizeof(cli_queueBuffer) };

void cli_init(void)
{
	cli_queueHandle = osMessageQueueNew(32, sizeof(MSG), &cli_queue_attributes);
	CLIHandle = osThreadNew(CLI_task, NULL, &CLI_attributes);
}

void process_cli(MSG msg)
{
	// Just an ECHO to begin
	HAL_UART_Transmit(&UART_CLI_HANDLER, msg.buffer, msg.len, 100);
}

void CLI_task(void *argument)
{
	init_uart_APP();
	MSG msg = { 0 };
	for (;;)
	{
		if (xQueueReceive(CLI_QUEUE_HANDLER, &msg, portMAX_DELAY))
		{
			process_cli(msg);
		}
		osDelay(1);
	}
}
