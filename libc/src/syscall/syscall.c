
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