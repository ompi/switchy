#include <stdint.h>
volatile uint32_t counter = 0u;

//extern "C"
__attribute__((interrupt("IRQ")))
void SysTick_Handler(void) {
  ++counter;
}
