
#include "stdint.h"

void write(const void *buf, unsigned long count);
int syscall(int syscall_number, int arg1, int arg2, int arg3);
void read(const void *buf, size_t count);
int brk(void *addr);
// void* cos_malloc(int bytes);
char get();
void reset_screen();

void write_char_at(char chr, int column, int row);

void update_echo(int enable);
