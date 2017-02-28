#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include "LPC11xx.h"

typedef struct {
    LPC_UART_TypeDef* base;
} uart;

void uartInit(uart* uart);
void uartSendByte(uart* uart, uint8_t data);

#endif /* __UART_H__ */
