#include "vga.h"
#include "utils.h"

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

void vga_set_text_color(uint16_t color){
    currentColor = (color << 8) | (COLOR8_BLACK << 12);
}

void BackSpace(){
    vga[line*width + column-1] = ' ';
    if(column == 0){
        return;
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

void set_cursor_position(uint16_t position) {
    outPortB(VGA_PORT_COMMAND, 0x0F);  // 0x0F is the register for low byte
    outPortB(VGA_PORT_DATA, position & 0xFF);  // Low byte

    // Set the high byte of the cursor position
    outPortB(VGA_PORT_COMMAND, 0x0E);  // 0x0E is the register for high byte
    outPortB(VGA_PORT_DATA, (position >> 8) & 0xFF);  // High byte
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
                update_cursor();
                break;
            case '\r':
                column = 0;
                update_cursor();
                break;
            case '\b':
                BackSpace();
                update_cursor();
                break;
            case '\v':
                newLine();
                update_cursor();
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
                update_cursor();
                break;
            default:
                if(column == width){
                    newLine();
                }
                vga[line * width + (column++)] = *s | currentColor;
                update_cursor();
                break;
        }
        s++;
    }
}

void update_cursor(){
    set_cursor_position(line*width + column);
}

void print_len(const char* s, uint32_t len){
    for (uint32_t i = 0; i < len; i++)
    {
        char character[2];
        character[0] = s[i];
        character[1] = '\0';
        print(character);
    }
}