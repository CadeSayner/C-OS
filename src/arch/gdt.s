global gdt_flush
global switch_to_user_mode_process
global load_page_table

gdt_flush:
    MOV eax, [esp+4]
    LGDT [eax]
    MOV eax, 0x10 ; offset in bytes at which the segment we want it in this case the kernel data segment
    MOV ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax
    MOV ss, ax 
    JMP 0x08:.flush ; far jump to initialise the code segment

.flush:
    RET

switch_to_user_mode_process:
    mov ebx, [esp+4]
    and esp, 0xFFFFFFF0 ; Align stack to 16-byte boundary
    cli
    mov   ax, 0x23
    mov   ds, eax
    mov   es, eax
    mov   fs, eax
    mov   gs, eax

    push  dword 0x23
    push  dword 0xB0000000
    pushfd
    push  dword 0x1B
    push  ebx
    iret


load_page_table:
    mov eax, [esp+4]
    mov cr3, eax
    RET

global tss_flush
tss_flush:
    MOV ax, 0x2B
    LTR ax
    RET

