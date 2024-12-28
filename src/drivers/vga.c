#include "vga.h"

uint16_t column = 0;
uint16_t line = 0;
uint16_t* const vga = (uint16_t* const) 0xC00B8000; // Load the address of video memory as a pointer to a 16 bit integer
const uint16_t defaultColor = (COLOR8_LIGHT_GRAY << 8) | (COLOR8_BLACK << 12);
uint16_t currentColor = defaultColor;

void Reset(){
    line = 0;
    column = 0;
    currentColor - defaultColor;
    for (uint16_t y = 0; y < height; y++){
        for (uint16_t x = 0; x < width; x++){
            vga[y * width + x] = ' ' | defaultColor;
        }
    }
}

void BackSpace(){
    vga[line*width + column-1] = ' ';
    if(column == 0){
        line--;
        column = width - 1;
    }else{
        column--;
    }
}

void newLine(){
    if(line < height - 1){
        line++;
        column = 0;
    }
    else{
        scrollUp();
        column = 0;
    }
}

void scrollUp(){
    for (uint16_t y = 1; y < height; y++){
        for (uint16_t x = 0; x < width; x++){
            vga[(y-1) * width + x] = vga[y*width + x];
        }
    }
    for (uint16_t x = 0; x < width; x++)
    {
        vga[(height-1)*width + x] = ' ' | currentColor;
    }
}

void print(const char* s){
    // this loop will terminate once /0 is read
    while(*s){
        switch(*s){
            case '\n':
                newLine();
                break;
            case '\r':
                column = 0;
                break;
            case '\b':
                BackSpace();
                break;
            case '\t':
                if(column == width){
                    newLine();
                }
                uint16_t tabLen = 4 - (column % 4);
                while(tabLen != 0){
                    vga[line * width + (column++)] = ' ' | currentColor;
                    tabLen --;
                }
            default:
                if(column == width){
                    newLine();
                }
                vga[line * width + (column++)] = *s | currentColor;
                break;
        }
        s++;
    }
}