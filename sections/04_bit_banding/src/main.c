// Após mudar para Unprivileged mode,
// ao tentar acessar endereços protegidos o processador gera HardFault

#include "stm32wbxx_hal.h"
#include <stdio.h>

#define ALIAS_BASE      0x22000000U
#define BITBAND_BASE    0x20000000U

extern void initialise_monitor_handles(void); // semihosting cfg

int main(void) {
    initialise_monitor_handles(); // semihosting

    uint8_t *ptr = (uint8_t*) 0x20000200;
    *ptr = 0xff;
    
    // normal method
    // clearing 7th bit position
    *ptr &= ~(1 << 7);

    // reset to 0xff
    *ptr = 0xff;
    
    // bit band method
    uint8_t *alias_addr = (uint8_t*) (ALIAS_BASE + (32 * (0x20000200 - BITBAND_BASE)) + 7 * 4);

    // cleating 7th bit of address 0x20000200
    *alias_addr = 0;

    for(;;); 
    return 0;
}

void HardFault_Handler(void) {
    printf("Hard fault detected\n");
    while(1);
}
