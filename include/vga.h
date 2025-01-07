#pragma once
#include "stdint.h"

#define COLOR8_BLACK        0
#define COLOR8_BLUE         1
#define COLOR8_GREEN        2
#define COLOR8_CYAN         3
#define COLOR8_RED          4
#define COLOR8_MAGENTA      5
#define COLOR8_BROWN        6
#define COLOR8_LIGHT_GRAY   7
#define COLOR8_DARK_GRAY    8
#define COLOR8_LIGHT_BLUE   9
#define COLOR8_LIGHT_GREEN  10
#define COLOR8_LIGHT_CYAN   11
#define COLOR8_LIGHT_RED    12
#define COLOR8_LIGHT_MAGENTA 13
#define COLOR8_YELLOW       14
#define COLOR8_WHITE        15

#define width 80
#define height 25
#define margin 2
#define VGA_PORT_COMMAND 0x3D4
#define VGA_PORT_DATA 0x3D5

void print(const char* s);
void print_len(const char* s, uint32_t len);
void vga_set_text_color(uint16_t color);
void scrollUp();
void newLine();
void Reset();
void BackSpace();
void print_char_at(char chr, int column, int row);
