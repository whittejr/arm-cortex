#include <stdint.h>
#include <stdio.h>

#define I2C1_IRQNO  30
#define TIM2_IRQNO  28

uint32_t *pNVIC_IPRBase  = (uint32_t*) 0xE000E400;   // interrupt PRIORITY base
uint32_t *pNVIC_ISERBase = (uint32_t*) 0xE000E100;  // interrupt SET-ENABLE base
uint32_t *pNVIC_ISPRBase = (uint32_t*) 0XE000E200;  // interrupt SET-PENDING base

extern void initialise_monitor_handles(void); // semihosting cfg

void configure_priority_for_irqs(uint8_t irq_no, uint8_t priority) {
    // 1. find out iprx
    uint8_t iprx = irq_no / 4; // encontrar o IPR (é dividido em 4 seções, cada uma de 8 bits. Ex.: IPR0 possui IRQ0 | IRQ1 | IRQ2 e IRQ3)
    uint32_t *ipr = pNVIC_IPRBase + iprx;

    // 2. position in iprx
    uint8_t pos = (irq_no % 4) * 8;

    // 3. configure the priority
    *ipr &= ~(0xFF << pos); // clear
    *ipr |= (priority << pos);  
     
}

int main(void) {
    initialise_monitor_handles(); // semihosting

    // 1. Priority configuration for the peripherals
    configure_priority_for_irqs(TIM2_IRQNO, 0x80);
    configure_priority_for_irqs(I2C1_IRQNO, 0x70);

    // 2. Set the interrupt pending bit in the NVIC PR (PENDING REGISTER)
    *pNVIC_ISPRBase |= (1 << TIM2_IRQNO);

    // 3. Enable the IRQs in NVIC ISER
    *pNVIC_ISERBase |= (1 << I2C1_IRQNO);
    *pNVIC_ISERBase |= (1 << TIM2_IRQNO);


    for(;;); 
    return 0;
}


// ISRs
void TIM2_IRQHandler(void) {
    printf("[TIM2 IRQ handler]\n");
    *pNVIC_ISPRBase |= (1 << I2C1_IRQNO);
    while(1);
}

void I2C1_EV_IRQHandler(void) {
    printf("I2C1 IRQ handler\n");
}
