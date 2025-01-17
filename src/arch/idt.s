global idt_flush

idt_flush:
    MOV eax, [esp+4]
    LIDT [eax]
    STI 
    RET

%macro ISR_NOERRCODE 1
    global isr%1
    isr%1:
        CLI
        PUSH LONG 0
        PUSH LONG %1
        JMP isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
    global isr%1
    isr%1:
        CLI
        PUSH LONG %1
        PUSH LONG %1
        JMP isr_common_stub
%endmacro

%macro IRQ 2
    global irq%1
    irq%1:
        CLI
        PUSH LONG 0
        PUSH LONG %2
        JMP irq_common_stub
%endmacro


ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2 
ISR_NOERRCODE 3 
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

ISR_NOERRCODE 128
ISR_NOERRCODE 177

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

extern isr_handler
isr_common_stub:
    ; Preserve registers that we need on the stack
    pusha
    mov eax, ds
    PUSH eax

    ; for interrupt 14 (cr2 contains the linear address of the last page fault)
    MOV eax, cr2
    PUSH eax

    ; Move into kernel ring important if we are coming from user mode
    MOV ax, 0x10
    MOV ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax

    PUSH esp

    CALL isr_handler

    ; Return to the previous segment for the gdt that was pushed second to last onto the stack
    ADD esp, 8
    POP ebx ; Remember this also increments the stack by another 4
    MOV ds, bx
    MOV es, bx
    MOV fs, bx
    MOV gs, bx
    pop edi       
    pop esi       
    pop ebp       
    add esp, 4 ; skip esp
    pop ebx       
    pop edx      
    pop ecx       
    add esp, 4 ; skip eax as well

    ADD esp, 8 ;pops the variables passed to this routine
    cmp DWORD [esp], 0x00000006
    je align_stack
    STI
    IRET ; get back to user or kernel mode depending on when the interrupt occured

align_stack:
    ADD esp, 4
    STI
    IRET

extern irq_handler
irq_common_stub:
    ; Preserve registers that we need on the stack
    pusha
    mov eax, ds
    PUSH eax

    ; for interrupt 14 (cr2 contains the linear address of the last page fault)
    MOV eax, cr2
    PUSH eax

    ; Move into kernel ring important if we are coming from user mode
    MOV ax, 0x10
    MOV ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax

    PUSH esp

    CALL irq_handler

    ; Return to the previous segment for the gdt that was pushed second to last onto the stack
    ADD esp, 8
    POP ebx ; Remember this also increments the stack by another 4
    MOV ds, bx
    MOV es, bx
    MOV fs, bx
    MOV gs, bx

    POPA
    ADD esp, 8 ;pops the variables passed to this routine
    STI
    IRET



