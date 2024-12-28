global gdt_flush

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