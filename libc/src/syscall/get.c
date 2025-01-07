char get(){
    return syscall(44, 0, 0, 0);
}