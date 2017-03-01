#ifndef __SWITCH_H__
#define __SWITCH_H__

#include <stdint.h>
#include "smi.h"
#include "uart.h"

void switchDumpRegistersPassive(uart* uart, smi* smi);
void switchDumpRegistersActive(uart* uart, smi* smi);

#endif /* __SWITCH_H__ */
