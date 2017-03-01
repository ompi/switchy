#include <stdio.h>
#include <stdarg.h>

#include "uart.h"

void uartInit(uart* uart) {
  uart->base->FCR |= 0x01;
  uart->base->LCR |= 0x83;

  uart->base->FDR = 0x85;
  uart->base->DLL = 0x04;
  uart->base->DLM = 0x00;

  uart->base->LCR &= ~0x80;

  uart->base->TER |= 0x80;
}

void uartSendByte(uart* uart, uint8_t data) {
  while (!(uart->base->LSR & 0x20)) { 
  }
  uart->base->THR = data;
}

void uartSendString(uart* uart, char* data) {
  while (*data) { 
    uartSendByte(uart, *data);
    data++;
  }
}

void uartPrintf(uart* uart, char* format, ...) {
  char* buf[128];
  va_list args;
  va_start(args, format);
  vsnprintf(buf, 256, format, args);
  uartSendString(uart, buf);
  va_end(args);
}
