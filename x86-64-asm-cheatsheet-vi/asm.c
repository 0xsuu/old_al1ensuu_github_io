
#include <stdio.h>

int main()
{
    __asm__
    (/*AT&T Syntax(default)
        "push %rax;"
        "push %rdi;"
        "mov $0x200001a, %rax;"
        "mov $31, %rdi;"
        "syscall;"
        "pop %rdi;"
        "pop %rax;"*/
     
     //Intel Syntax
        ".intel_syntax;"
        "push rax;" //Save Registers
        "push rdi;"
        "mov rax, 0x200001a;"   //Set syscall number
        "mov rdi, 31;"           //Set argument 1
        "syscall;"
        "pop rdi;"   //Restore Registers
        "pop rax;"
    );
    printf("Hello World!\n");
    return 0;
}