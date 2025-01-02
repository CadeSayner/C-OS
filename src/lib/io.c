#include "stdint.h"
#include "utils.h"

static uint8_t buffer[4096];

// represents a line of input after enter was pressed
static uint8_t line[4096];

uint16_t curr_position = 0;

void initIO(){
    reset_input_buffer();
}

void cpy_buffer_to_line(){
    for (uint16_t i = 0; i < 4096; i++)
    {
        line[i] = buffer[i];
    }
}

void add_char_to_input_buffer(uint8_t chr){
    if(chr == '\n'){
        cpy_buffer_to_line();
        reset_input_buffer();
    }else{
        buffer[curr_position++] = chr;
    }  
}

// read bytes bytes from std input buffer
void kread(uint8_t *buf, int bytes){
   // allow interrupts
   asm volatile("sti");
   
   while(line[0] == 0){}
   // line is populated so we can now write the buffer passed to us
   for (uint16_t i = 0; i < bytes; i++)
   {
        buf[i] = line[i];
   }

   // done reading the line so can reset it
   reset_line();
}

void reset_input_buffer(){
    memset(buffer, 0, sizeof(buffer));
    curr_position = 0;
}

void reset_line(){
    memset(line, 0, sizeof(line));
}