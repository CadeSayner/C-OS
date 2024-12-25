BITS 32
section .text 
    ALIGN 4
    DD 0x1BADB002 ; Tells GRUB that this is a bootloader for an OS
    DD 0x00000000
    DD -(0x1BADB002 + 0x00000000)

global start ; Make the symbol accessible to other files
extern kmain ; Tells the assembler/linker to resolve the symbol outside of this file

start:
    CLI ; turn off interupts
    MOV esp, stack_space ; intialise the stack pointer to the stack space allocated to the kernel within the ISO
    CALL kmain ; Call kernel main
    HLT ; if the execution thread every escapes kmain the system should immediately halt
HaltKernel:
    CLI
    HLT
    JMP HaltKernel

section .bss
; Allocate 8KB for the kernel's initial stack
RESB 8192 
stack_space: 

