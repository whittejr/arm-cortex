#include "stm32wbxx_hal.h"
#include <stdio.h>

#define led_pin GPIO_PIN_4
#define led_port GPIOE

extern void initialise_monitor_handles(void); // semihosting cfg

/* This function executes in THREAD MODE of the processor */
void generate_interrupt() {
	uint32_t *pSTIR  = (uint32_t*)0xE000EF00;
	uint32_t *pISER0 = (uint32_t*)0xE000E100;

	//enable IRQ3 interrupt
	*pISER0 |= ( 1 << 3);

	//generate an interrupt from software for IRQ3
	*pSTIR = (3 & 0x1FF);

}
void clk_enable(void);

int main(void) {
    initialise_monitor_handles(); // semihosting
    clk_enable(); //led

    /* EXERCISE */

    printf("In thread mode : before interrupt\n");

    generate_interrupt();

    printf("In thread mode : after interrupt\n");
    
    
    while (1);
    return 0;
}

void clk_enable(void) {
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = led_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(led_port, &GPIO_InitStruct);
}

void RTC_WKUP_IRQHandler(void) {
	printf("In handler mode : ISR\n");
}
