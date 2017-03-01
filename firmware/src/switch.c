#include "switch.h"

void switchDumpRegistersPassive(uart* uart, smi* smi) {
  uint16_t phy;
  uint16_t reg;

  uartPrintf(uart, "     0               1               \r\n");
  uartPrintf(uart, "     0123456789abcdef0123456789abcdef\r\n");

  for (reg = 0; reg < 32; reg++) {
    uartPrintf(uart, "0x%.2x ", reg);
    for (phy = 0; phy < 32; phy++) {
      int ret = smiRead(smi, phy, reg, 1);
      if (ret != -1) {
        if (ret != 0x0000 && ret != 0xffff) {
          uartSendByte(uart, '#');
        } else {
          uartSendByte(uart, '.');
        }
      } else {
        uartSendByte(uart, ' ');
      }
    }
    uartPrintf(uart, "\r\n");
  }
}

static int countOnes(uint16_t value) {
  int ones = 0;
  uint16_t mask = 1;
  int i;

  for (i = 0; i < 16; i++) {
    if (value & mask) {
      ones++;
    }
    mask <<= 1;
  }
  return ones;
}

#define WRITE_DELAY 100000

void switchDumpRegistersActive(uart* uart, smi* smi) {
  volatile uint64_t i;
  uint16_t phy;
  uint16_t reg;

  uartPrintf(uart, "     0               1               \r\n");
  uartPrintf(uart, "     0123456789abcdef0123456789abcdef\r\n");

  for (reg = 0; reg < 32; reg++) {
    uartPrintf(uart, "0x%.2x ", reg);
    for (phy = 0; phy < 32; phy++) {
      int defaultValue, moreValue, lessValue;
      int defaultOnes, moreOnes, lessOnes;

      defaultValue = smiRead(smi, phy, reg, 1);
      if (defaultValue == -1) {
        uartSendByte(uart, ' ');
        continue;
      }

      smiWrite(smi, phy, reg, 0x0000, 1);
      for (i = 0; i < WRITE_DELAY; i++) {
      }

      lessValue = smiRead(smi, phy, reg, 1);
      if (lessValue == -1) {
        uartSendByte(uart, '?');
        continue;
      }

      smiWrite(smi, phy, reg, defaultValue, 1);
      for (i = 0; i < WRITE_DELAY; i++) {
      }
      smiWrite(smi, phy, reg, 0xFFFF, 1);
      for (i = 0; i < WRITE_DELAY; i++) {
      }

      moreValue = smiRead(smi, phy, reg, 1);
      if (moreValue == -1) {
        uartSendByte(uart, '?');
        continue;
      }

      smiWrite(smi, phy, reg, defaultValue, 1);
      for (i = 0; i < WRITE_DELAY; i++) {
      }

      defaultOnes = countOnes(defaultValue);
      lessOnes = countOnes(lessValue);
      moreOnes = countOnes(moreValue);

      if (defaultValue == 0x0000 && moreValue == 0x0000 && lessValue == 0x0000) {
        uartSendByte(uart, '0');
      } else if (defaultValue == 0xffff && moreValue == 0xffff && lessValue == 0xffff) {
        uartSendByte(uart, 'F');
      } else if (defaultValue == moreValue && defaultValue == moreValue) {
        uartSendByte(uart, 'C');
      } else if (lessOnes == defaultOnes && moreOnes == defaultOnes && lessValue != moreValue) {
        uartSendByte(uart, '#');
      } else if (lessOnes < defaultOnes && moreOnes > defaultOnes) {
        uartSendByte(uart, '#');
      } else if (lessOnes < defaultOnes) {
        uartSendByte(uart, '#');
      } else if (moreOnes > defaultOnes) {
        uartSendByte(uart, '#');
      } else {
        uartSendByte(uart, '.');
      }
    }
    uartPrintf(uart, "\r\n");
  }
}

void switchDumpStats(uart* uart, smi* smi) {
  int whatever;
  uint64_t i;

  uartPrintf(uart, "capturing counters...\r\n");
  for (whatever = 0; whatever < (1 << 12); whatever++) {
    smiWrite(smi, 0x1f, 0x1d, (1 << 15) | (0x5 << 12) | whatever, 1); // capture all counters on all whatevers
  }

  for (whatever = 0; whatever < (1 << 12); whatever++) {
    smiWrite(smi, 0x1f, 0x1d, (1 << 15) | (0x4 << 12) | whatever, 1); // read whatever counters on all whatevers

    int low, high, count;
    high = smiRead(smi, 0x1f, 0x1e, 1);
    low = smiRead(smi, 0x1f, 0x1f, 1);

    count = (high << 16) | low;

    if (count % 10 == 0 && count != 0)
    if (low != -1 && high != -1) {
      uartPrintf(uart, "%.4x: %d\r\n", whatever, count);
    }
  }

/*  uartPrintf(uart, "flush counters...\r\n");
  smiWrite(smi, 0x1f, 0x1d, (1 << 15) | (0x1 << 12), 1); // flush all
*/
  for (i = 0; i < 1000000; i++) {
  }
}
