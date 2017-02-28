#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>
#include "LPC11xx.h"

typedef struct {
    LPC_GPIO_TypeDef* bank;
    uint8_t pin;
} gpio;

void gpioDirIn(gpio* gpio);
void gpioDirOut(gpio* gpio);

void gpioSet(gpio* gpio);
void gpioClear(gpio* gpio);

int gpioGet(gpio* gpio);

#endif /* __GPIO_H__ */
