#include <stdint.h>
#include <stdio.h>

extern void initialise_monitor_handles(void); // semihosting cfg

#define USART1_IQRNO 36
int main(void) {
    initialise_monitor_handles(); // semihosting

    // 1. Manually pend the pending bit for the USART1 IRQ number in VIC
    uint32_t *pISPR1 = (uint32_t*) 0xE000E204;
    *pISPR1 |= (1 << (USART1_IQRNO % 32) );

    // 2. Enable the USART1 IRQ number in NVIC
    uint32_t *pISER1 = (uint32_t*) 0xE000E104;
    *pISER1 |= (1 << (USART1_IQRNO % 32) );

    for(;;); 
    return 0;
}

// USART1 IRQ
void USART1_IRQHandler(void) {
    printf("in USART1 ISR\n");
}
