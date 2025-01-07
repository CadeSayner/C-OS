#define KBD_DATA_PORT  0x60
#define KBD_STATUS_PORT 0x64

#include "utils.h"

void kbd_handler(struct InterruptRegisters* regs);
void enable_echoing();
void disable_echoing();

