// Após mudar para Unprivileged mode,
// ao tentar acessar endereços protegidos o processador gera HardFault

#include "stm32wbxx_hal.h"
#include <stdio.h>

extern void initialise_monitor_handles(void); // semihosting cfg

void generate_interrupt(void);
void change_access_level_unpriv(void);

int main(void) {
    initialise_monitor_handles(); // semihosting
    
    printf("In thread mode : before interrupt\n");

    void (*fun_ptr) (void);
    fun_ptr = change_access_level_unpriv;
    fun_ptr();

    printf("In thread mode : after interrupt\n");


    for(;;); 
    return 0;
}

/* This function executes in THREAD MODE of the processor */
void generate_interrupt(void) {
	uint32_t *pSTIR  = (uint32_t*)0xE000EF00;
	uint32_t *pISER0 = (uint32_t*)0xE000E100;

	//enable IRQ3 interrupt
	*pISER0 |= ( 1 << 3);

	//generate an interrupt from software for IRQ3
	*pSTIR = (3 & 0x1FF);

}

void change_access_level_unpriv(void) {
    // read
    __asm volatile("MRS R0, CONTROL");
    // modify
    __asm volatile("ORR R0,R0,#0X01");
    // write
    __asm volatile("MSR CONTROL,R0");
}

void HardFault_Handler(void) {
    printf("Hard fault detected\n");
    while(1);
}
