#include <stdio.h>

#include "LPC11xx.h"
#include "core_cm0.h"

#include "gpio.h"
#include "smi.h"
#include "uart.h"

extern volatile uint32_t counter;

int main() {
  LPC_IOCON->PIO0_1 = 0xd0;

  LPC_IOCON->PIO1_4 = 0xd0;
  LPC_IOCON->PIO1_5 = 0xd0;

  LPC_IOCON->PIO1_7 = 0xd1;

  LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12);
  LPC_SYSCON->UARTCLKDIV = 4;

  gpio mdc = { bank: LPC_GPIO1, pin: 4 };
  gpio mdio = { bank: LPC_GPIO1, pin: 5 };

  smi smi = {
    mdc: &mdc,
    mdio: &mdio,
    clockDelay: 10,
    preambleLength: 1,
  };

  uart uart = {
    base: LPC_UART_BASE,
  };

  smiInit(&smi);
  uartInit(&uart);

  SysTick_Config(12e6 / 1000);

  volatile uint64_t i;
  uint16_t phy;
  uint16_t reg;

  char buf[256];

  while(1) {
    snprintf(buf, 256, "     0               1               \r\n");
    uartSendString(&uart, buf);
    snprintf(buf, 256, "     0123456789abcdef0123456789abcdef\r\n");
    uartSendString(&uart, buf);

    for (reg = 0; reg < 32; reg++) {
      snprintf(buf, 256, "0x%.2x ", reg);
      uartSendString(&uart, buf);
      for (phy = 0; phy < 32; phy++) {
        int ret = smiRead(&smi, phy, reg, 1);
        uartSendByte(&uart, (ret != -1) ? '#' : ' ');
      }
      uartSendByte(&uart, '\r');
      uartSendByte(&uart, '\n');
    }
    for (i = 0; i < 1000000; i++) {
    }
  }
}
