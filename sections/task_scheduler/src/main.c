#include <stdint.h>
#include <stdio.h>
#include "main.h"

void enable_processor_faults(void);

/* task functions */
void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);
void init_tasks_stack(void);

/* systick function */
void init_systick_timer(uint32_t tick_hz);
/* scheduler function */
__attribute__ ((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack);

/* global variables */
uint32_t psp_of_tasks[MAX_TASKS] = { T1_STACK_START, T2_STACK_START, T3_STACK_START, T4_STACK_START };
uint32_t task_handlers[MAX_TASKS];

extern void initialise_monitor_handles(void); // semihosting cfg
int main(void) {
    initialise_monitor_handles(); // semihosting

	enable_processor_faults();

	init_scheduler_stack(SCHED_STACK_START);

	task_handlers[0] = (uint32_t) task1_handler;
	task_handlers[1] = (uint32_t) task2_handler;
	task_handlers[2] = (uint32_t) task3_handler;
	task_handlers[3] = (uint32_t) task4_handler;

	init_tasks_stack();

	init_systick_timer(TICK_HZ);

	switch_sp_to_psp();

	while(1);
}

void task1_handler(void) {
	while(1) {
		printf("This is task1");
	}
}
void task2_handler(void) {
	while(1) {
		printf("This is task2");
	}
}
void task3_handler(void) {
	while(1) {
		printf("This is task3");
	}
}
void task4_handler(void) {
	while(1) {
		printf("This is task4");
	}
}

void init_systick_timer(uint32_t tick_hz) {

	/* pointers to systick cfg addresses */
	uint32_t *pSRVR = (uint32_t*) 0xE000E014; // address to the SysTick Reload Value Register
	uint32_t *pSCSR = (uint32_t*) 0xE000E010; // address to the SysTick Control and Status Register

	uint32_t count_value = (SYSTICK_TIM_CLK / tick_hz) - 1;

	// clear the value of SVR
	*pSRVR &= ~(0x00FFFFFF);

	// load the value into SVR (SysTick Value Register)
	*pSRVR |= count_value;

	// some systick setttings
	// bit 1 = CLKSOURCE (0 = external clock | 1 = processor clock)
	// bit 2 = TICKINT 	 (Enables SysTick exception request )
	*pSCSR |= (1 << 1);
	*pSCSR |= (1 << 2);

	// enable systick
	// bit 0 = ENABLE (Enables the counter)
	*pSCSR |= (1 << 0);
	
}

/* SysTick Handler */
void SysTick_Handler(void) {
}

__attribute__ ((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack) {
	__asm volatile("MSR MSP,%0": : "r" (sched_top_of_stack) : );
	__asm volatile("BX LR");
}

void init_tasks_stack(void) {
	uint32_t *pPSP;
	for (int i = 0; i < MAX_TASKS; i++) {
		pPSP = (uint32_t*) psp_of_tasks[i];

		pPSP--;
		*pPSP = DUMMY_XPSR; // 0x01000000

		pPSP--; // PC
		*pPSP = 0xFFFFFFFD;

		for (int j = 0; j < 13; j++) {
			pPSP--; // general purpose registers
			*pPSP = 0;
		}
		psp_of_tasks[i] = (uint32_t) pPSP;

	}
}

// __attribute__ ((naked)) void switch_sp_to_psp(void) {
// 	__asm volatile("")
// }

void enable_processor_faults(void) {
	uint32_t *pSHCSR = (uint32_t*) 0xE000ED24;

	*pSHCSR |= (1 << 16); // mem manage
	*pSHCSR |= (1 << 17); // bus fault
	*pSHCSR |= (1 << 18); // usage fault
}
