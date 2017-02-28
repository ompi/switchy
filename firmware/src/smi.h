#ifndef __SMI_H__
#define __SMI_H__

#include <stdint.h>
#include "gpio.h"

typedef struct {
    gpio* mdc;
    gpio* mdio;
    uint32_t clockDelay;
} smi;

void smiInit(smi* smi);
void smiWrite(smi* smi, uint8_t phyAddress, uint8_t regAddress, uint16_t data, uint8_t preamble);
uint16_t smiRead(smi* smi, uint8_t phyAddress, uint8_t regAddress, uint8_t preamble);
static inline void smiPutBit(smi* smi, int data);
static inline int smiGetBit(smi* smi);
static inline void smiBusyWaitHalf(smi* smi);
static inline void smiBusyWaitFull(smi* smi);

#endif /* __SMI_H__ */
