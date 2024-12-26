#include "stdint.h"

void memset(void *dest, char val, uint32_t count);
void outPortB(uint16_t port, uint8_t value);

struct InterruptRegisters{
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    // these are pushed by the processor when the interrupt first fires
    uint32_t eip, csm, eflags, useresp, ss;
};

void int32_to_hex_string(uint32_t num, char* hexString);

void printHexInt(uint32_t num);