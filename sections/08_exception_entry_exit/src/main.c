// undone

#include <stdio.h>

extern void initialise_monitor_handles(void); // semihosting cfg

/* This function executes in THREAD MODE of the processor */
void generate_interrupt() {
	uint32_t *pSTIR  = (uint32_t*)0xE000EF00;
	uint32_t *pISER0 = (uint32_t*)0xE000E100;

	//enable IRQ3 interrupt
	*pISER0 |= ( 1 << 3);

	//generate an interrupt from software for IRQ3
	*pSTIR = (3 & 0x1FF); // valor de SP 0x2002ffe8 -- valor do xPSR 0x610f0000
    
}

int main(void) {
    initialise_monitor_handles(); // semihosting

    printf("In thread mode : before interrupt\n");

    generate_interrupt();

    printf("In thread mode : after interrupt\n");
    
    
    while (1);
    return 0;
}

void RTC_WKUP_IRQHandler(void) {
	printf("In handler mode : ISR\n");
}
