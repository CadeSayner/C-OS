int main()
{
    asm volatile("mov $0x12345678, %eax");
    while(1==1){}
    return 0;
}


