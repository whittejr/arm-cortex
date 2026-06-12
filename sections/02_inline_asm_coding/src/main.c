#include "stm32wbxx_hal.h"
#include <stdio.h>

extern void initialise_monitor_handles(void); // semihosting cfg

void clk_enable(void);

int main(void) {
    initialise_monitor_handles(); // semihosting
    
    // __asm volatile("LDR R1,=#0x20001000");
    // __asm volatile("LDR R2,=#0x20001004");
    // __asm volatile("LDR R0, [R1]");
    // __asm volatile("LDR R1, [R2]");
    // __asm volatile("ADD R0,R0,R1");
    // __asm volatile("STR R0,[R2]");

    /* asm coding part 3 */
    int val = 50;
    __asm volatile("MOV r0,%0": : "r"(val));

    /* asm coding part 4 */
    int control_reg;
    // read CONTROL register to control_reg
    __asm volatile("MRS %0, CONTROL": "=r"(control_reg));
    
    int p1, *p2;
    p2 = (int*) 0x20000008;

    __asm volatile("LDR %0,[%1]": "=r"(p1): "r"(p2));


    for(;;);
    return 0;
}
