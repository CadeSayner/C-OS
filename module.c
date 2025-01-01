char modname[] __attribute__((section(".modname"), aligned(4))) = "hello world module";

int main()
{
    asm volatile("mov $0x12345678, %eax");
    char input[] = "hello world";
    syscall(1, 1, input, 12);
    while(1==1){}
    return 0;
}

void syscall(int syscall_number, int arg1, int arg2, int arg3) {
    int result;
    asm volatile (
        "int $128"              // Trigger interrupt vector 127
        : "=a" (result)         // Store return value in 'result'
        : "a" (syscall_number), // Pass syscall number in 'eax'
          "b" (arg1),           // Pass arguments in other registers
          "c" (arg2),
          "d" (arg3)
    );
}


