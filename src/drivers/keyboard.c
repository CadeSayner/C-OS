#include "keyboard.h"
#include "stdint.h"
#include "vga.h"
#include "io.h"
/** read_scan_code:
 *  Reads a scan code from the keyboard
 *
 *  @return The scan code (NOT an ASCII character!)
 */

char keymap[] = {
        0x1E, 'a', 'A',
        0x0E, '\b', '\b',
        0x1C, '\n', '\n',
        0x30, 'b', 'B',
        0x2E, 'c', 'C',
        0x20, 'd', 'D',
        0x12, 'e', 'E',
        0x21, 'f', 'F',
        0x22, 'g', 'G',
        0x23, 'h', 'H',
        0x17, 'i', 'I',
        0x24, 'j', 'J',
        0x25, 'k', 'K',
        0x26, 'l', 'L',
        0x32, 'm', 'M',
        0x31, 'n', 'N',
        0x18, 'o', 'O',
        0x19, 'p', 'P',
        0x10, 'q', 'Q',
        0x13, 'r', 'R',
        0x14, 't', 'T',
        0x1F, 's', 'S',
        0x16, 'u', 'U',
        0x2F, 'v', 'V',
        0x11, 'w', 'W',
        0x2D, 'x', 'X',
        0x15, 'y', 'Y',
        0x2C, 'z', 'Z',
        0x39, ' ', ' ',  // Space
        0x02, '1', '1',
        0x03, '2', '2',
        0x04, '3', '3',
        0x05, '4', '4',
        0x06, '5', '5',
        0x07, '6', '6',
        0x08, '7', '7',
        0x09, '8', '8',
        0x0A, '9', '9',
        0x0B, '0', '0',

};

bool echoing = true;
bool shift = false;

uint8_t read_scan_code(void)
{
    return inb(KBD_DATA_PORT);
}

void enable_echoing(){
    echoing = true;
}

void disable_echoing(){
    echoing = false;
}

uint8_t read_keyboard_status(void){
    return inb(KBD_STATUS_PORT);
}

char getASCI(uint8_t keyCode){
    for (uint8_t i = 0; i < sizeof(keymap); i+=3)
    {
        if(keymap[i] == keyCode){
            if(!shift){
                return keymap[i+1];
            }
            return keymap[i+2];
        }
    }
    return 0;
}

void kbd_handler(struct InterruptRegisters* regs){
    uint8_t scan_code = read_scan_code();

    if(scan_code == 0x2A){
        // shift has been pressed
        shift = true;
    }

    else if(scan_code == 0xAA){
        //shift released
        shift = false
    }

    else{
        uint8_t asci_char = getASCI(scan_code);
        if(asci_char != 0){
            uint8_t asci_arr[2];
            asci_arr[0] = asci_char;
            asci_arr[1] = '\0';
            // echo to the "terminal screen"
            if(echoing){
                print(asci_arr);
            }
            // send to buffer
            add_char_to_input_buffer(asci_char);
        }
    }
}