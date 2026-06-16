#include <stdio.h>
#include <string.h>

#define SRAM_START          0x20000008U
#define SRAM_SIZE           0x2FFF8U
#define SRAM_END            ((SRAM_START) + (SRAM_SIZE))
#define STACK_START         SRAM_END

#define STACK_MSP_START     STACK_START
#define STACK_MSP_END       (STACK_START + 512)
#define STACK_PSP_START     STACK_MSP_END

int fun_add(int a, int b, int c, int d) {
    return a + b + c + d;
}

void generate_exception(void) {
    __asm volatile("SVC #0x2");
}

__attribute__((naked)) void change_sp_to_psp(void) {
    __asm volatile(".equ SRAM_END, (0x20000008 + (192 * 1024))");
    __asm volatile(".equ PSP_START, (SRAM_END - 512)");
    __asm volatile ("LDR R0,=PSP_START");
    __asm volatile("MSR PSP, R0");
    __asm volatile("MOV R0,#0x02");
    __asm volatile("MSR CONTROL, R0");
    __asm volatile("BX LR");
}

extern void initialise_monitor_handles(void); // semihosting cfg

int main(void) {
    initialise_monitor_handles(); // semihosting

    change_sp_to_psp();

    int ret;
    ret = fun_add(1, 4, 5, 6);
    printf("result = %d\n", ret);

    generate_exception();

    for(;;); 
    return 0;
}

void SVC_Handler(void) {

}
