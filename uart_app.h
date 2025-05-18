/*
 * uart_app.h
 *
 *  Created on: May 18, 2025
 *      Author: lucasalmeida
 */

#ifndef CLI_LIB_UART_APP_H_
#define CLI_LIB_UART_APP_H_

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "defines.h"

#define RX_BUF_SIZE     1024
#define MAIN_BUF_SIZE   1024
#define MAX_RX_DATA     128

typedef struct
{
	uint8_t buffer[MAX_RX_DATA];
	uint16_t len;
} MSG;

void init_uart_APP(void);

#endif /* CLI_LIB_UART_APP_H_ */
