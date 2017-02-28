#include "smi.h"

void smiInit(smi* smi) {
  gpioClear(smi->mdc);
  gpioClear(smi->mdio); 

  gpioDirOut(smi->mdc);
  gpioDirIn(smi->mdio);
}

void smiWrite(smi* smi, uint8_t phyAddress, uint8_t regAddress, uint16_t data, uint8_t preamble) {
  int mask, i;
  
  gpioDirOut(smi->mdio);
  
  if (preamble) {
    for (i = 0; i < smi->preambleLength; i++) {
      smiPutBit(smi, 1);
    }
  }
  
  smiPutBit(smi, 0);                             // start
  smiPutBit(smi, 1);

  smiPutBit(smi, 0);                             // write
  smiPutBit(smi, 1);

  for (mask = (1 << 4); mask > 0; mask >>= 1)    // phy address
    smiPutBit(smi, phyAddress & mask);

  for (mask = (1 << 4); mask > 0; mask >>= 1)    // register address
    smiPutBit(smi, regAddress & mask);

  smiPutBit(smi, 1);                             // turn around
  smiPutBit(smi, 0);
  
  for (mask = (1 << 15); mask > 0; mask >>= 1)   // data
    smiPutBit(smi, (data & mask) ? 1 : 0);

  gpioDirIn(smi->mdio);
  
  smiPutBit(smi, 0);                             // idle
}

int smiRead(smi* smi, uint8_t phyAddress, uint8_t regAddress, uint8_t preamble) {
  uint16_t data = 0;
  int mask, i, nack;
  
  gpioDirOut(smi->mdio);
  
  if (preamble) {
    for (i = 0; i < smi->preambleLength; i++) {
      smiPutBit(smi, 1);
    }
  }
      
  smiPutBit(smi, 0);                             // start
  smiPutBit(smi, 1);

  smiPutBit(smi, 1);                             // read
  smiPutBit(smi, 0);

  for (mask = (1 << 4); mask > 0; mask >>= 1)    // phy address
    smiPutBit(smi, phyAddress & mask);

  for (mask = (1 << 4); mask > 0; mask >>= 1)    // register address
    smiPutBit(smi, regAddress & mask);

  gpioDirIn(smi->mdio);
  
  nack = smiGetBit(smi);                         // turn around
  if (nack) {
    return -1;
  }
  
  for (mask = (1 << 15); mask > 0; mask >>= 1)   // data
    data |= smiGetBit(smi) ? mask : 0;
  
  smiPutBit(smi, 0);                             // idle

  return data;
}

static inline void smiPutBit(smi* smi, int data) {
  if (data) {
    gpioSet(smi->mdio);
  } else {
    gpioClear(smi->mdio);
  }

  smiBusyWaitHalf(smi);
  
  gpioSet(smi->mdc);
  smiBusyWaitFull(smi);

  gpioClear(smi->mdc);
  smiBusyWaitHalf(smi);
}

static inline int smiGetBit(smi* smi) {
  int data;

  smiBusyWaitHalf(smi);
  
  gpioSet(smi->mdc);
  smiBusyWaitFull(smi);

  gpioClear(smi->mdc);
  data = gpioGet(smi->mdio);
  smiBusyWaitHalf(smi);

  return data;
}

static inline void smiBusyWaitHalf(smi* smi) {
  volatile uint32_t i;
  for (i = 0; i < (smi->clockDelay >> 1); i++) {
  }
}

static inline void smiBusyWaitFull(smi* smi) {
  volatile uint32_t i;
  for (i = 0; i < smi->clockDelay; i++) {
  }
}
