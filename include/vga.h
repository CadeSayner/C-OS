#pragma once
#include "stdint.h"

#define COLOR8_BLACK 0
#define COLOR8_LIGHT_GRAY 7

#define width 80
#define height 25

void print(const char* s);
void scrollUp();
void newLine();
void Reset();
void BackSpace();