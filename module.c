int main(int argc, char const *argv[])
{
    asm volatile("mov $0x12345678, %eax");
}
