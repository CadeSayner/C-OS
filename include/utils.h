#include "stdint.h"
#include "multiboot.h"
#pragma once
void memset(void *dest, char val, uint32_t count);
void outPortB(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

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

void elf32_parse_modules(struct module* procs, struct module_struct* mods, int mod_count);

void print_dec_uint(uint32_t num);

int32_t parse_uint(const char* s);

uint32_t pow(int n, int m);