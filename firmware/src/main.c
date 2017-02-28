#include "LPC11xx.h"
#include "core_cm0.h"

#include "gpio.h"
#include "smi.h"

extern volatile uint32_t counter;

int main() {
  LPC_IOCON->PIO1_4 = 0xd0;
  LPC_IOCON->PIO1_5 = 0xd0;

  gpio mdc = { bank: LPC_GPIO1, pin: 4 };
  gpio mdio = { bank: LPC_GPIO1, pin: 5 };

  smi smi = {
    mdc: &mdc,
    mdio: &mdio,
    clockDelay: 10
  };

  smiInit(&smi);

  SysTick_Config(12e6 / 1000);

  volatile uint32_t i;
  volatile uint32_t j;
  while(1) {
    for (i = 8; i < 16; i++) {
      smiRead(&smi, i, 0x00, 1);
      for (j = 0; j < 1000; j++) { }
    }
  }
}
