#include "utils.h"

uint32_t write(struct InterruptRegisters *regs);
void update_echo(struct InterruptRegisters *regs);
void reset_screen(struct InterruptRegisters *regs);
void sys_write_char_at(struct InterruptRegisters *regs);