#include "gpio.h"

void gpioDirIn(gpio* gpio) {
  gpio->bank->DIR &= ~(1 << gpio->pin);
}

void gpioDirOut(gpio* gpio) {
  gpio->bank->DIR |= (1 << gpio->pin);
}

void gpioSet(gpio* gpio) {
  gpio->bank->DATA |= (1 << gpio->pin);
}

void gpioClear(gpio* gpio) {
  gpio->bank->DATA &= ~(1 << gpio->pin);
}

int gpioGet(gpio* gpio) {
  return (gpio->bank->DATA >> gpio->pin) & 0x01;
}
