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
__attribute__ ((naked)) void switch_sp_to_psp(void);

/* global variables */
uint8_t current_task = 0; // task1 is running

extern void initialise_monitor_handles(void); // semihosting cfg
int main(void) {
    initialise_monitor_handles(); // semihosting

	enable_processor_faults();

	init_scheduler_stack(SCHED_STACK_START);

	init_tasks_stack();

	init_systick_timer(TICK_HZ);

	switch_sp_to_psp();

	task1_handler();

	while(1);
}

void task1_handler(void) {
	while(1) {
		printf("This is task1\n");
	}
}
void task2_handler(void) {
	while(1) {
		printf("This is task2\n");
	}
}
void task3_handler(void) {
	while(1) {
		printf("This is task3\n");
	}
}
void task4_handler(void) {
	while(1) {
		printf("This is task4\n");
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


void save_psp_value(uint32_t current_psp_value) {
	user_tasks[current_task].psp_value = current_psp_value; // psp value == stack address
}

void update_next_task(void) {
	current_task++;
	current_task %= MAX_TASKS; 
}

/* SysTick Handler */
__attribute__ ((naked)) void SysTick_Handler(void) {
	/* save the context of current task */

	// 1. get current running task's PSP value
	__asm volatile("MRS R0,PSP");
	// 2. using that PSP value store SF2 (R4 to R11)
	__asm volatile("STMDB R0!,{R4-R11}");
	// 3. save the current value of PSP
	__asm volatile("PUSH {LR}"); // i know!!
	__asm volatile("BL save_psp_value");	

	/* retrieve the context of next task */

	// 1. decide next task to run
	__asm volatile("BL update_next_task");
	// 2. get it's past PSP value 
	__asm volatile("BL get_psp_value");
	// 3. using that PSP value retrieve SF2 (R4 to R11)
	__asm volatile("LDMIA R0!,{R4-R11}");
	// 4. update PSP and exit
	__asm volatile("MSR PSP,R0");

	__asm volatile("POP {LR}");

	__asm volatile("BX LR");
}

__attribute__ ((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack) {
	__asm volatile("MSR MSP,%0": : "r" (sched_top_of_stack) : );
	__asm volatile("BX LR");
}

void init_tasks_stack(void) {

	user_tasks[0].current_state = TASK_RUNNING_STATE;
	user_tasks[1].current_state = TASK_RUNNING_STATE;
	user_tasks[2].current_state = TASK_RUNNING_STATE;
	user_tasks[3].current_state = TASK_RUNNING_STATE;

	user_tasks[0].psp_value = T1_STACK_START;
	user_tasks[1].psp_value = T2_STACK_START;
	user_tasks[2].psp_value = T3_STACK_START;
	user_tasks[3].psp_value = T4_STACK_START;

	user_tasks[0].task_handler = task1_handler;
	user_tasks[1].task_handler = task2_handler;
	user_tasks[2].task_handler = task3_handler;
	user_tasks[3].task_handler = task4_handler;

	uint32_t *pPSP;
	for (int i = 0; i < MAX_TASKS; i++) {
		pPSP = (uint32_t*) user_tasks[i].psp_value;

		pPSP--;
		*pPSP = DUMMY_XPSR; // 0x01000000

		pPSP--; // PC
		*pPSP = (uint32_t) user_tasks[i].task_handler;

		pPSP--; // LR
		*pPSP = 0xFFFFFFFD;

		for (int j = 0; j < 13; j++) {
			pPSP--; // general purpose registers
			*pPSP = 0;
		}
		user_tasks[i].psp_value = (uint32_t) pPSP;

	}
}

uint32_t get_psp_value(void) {
	return user_tasks[current_task].psp_value;
}

__attribute__ ((naked)) void switch_sp_to_psp(void) {
	// 1. initialize the PSP with TASK1 stack start

	// get the value of psp current_task
	__asm volatile("PUSH {LR}");	// LR that connects back to main()
	__asm volatile("BL get_psp_value");
	__asm volatile("MSR PSP,R0"); // initialize PSP
	__asm volatile("POP {LR}");	// pops back LR value

	// 2. change SP to PSP using CONTROL register
	__asm volatile("MOV R0,#0x02");
	__asm volatile("MSR CONTROL,R0");
	__asm volatile("BX LR");
}

void enable_processor_faults(void) {
	uint32_t *pSHCSR = (uint32_t*) 0xE000ED24;

	*pSHCSR |= (1 << 16); // mem manage
	*pSHCSR |= (1 << 17); // bus fault
	*pSHCSR |= (1 << 18); // usage fault
}

void HardFault_Handler(void) {
	printf("Exception: HardFault\n");
	while(1);
}
void MemManage_Handler(void) {
	printf("Exception: MemManage\n");
	while(1);
}
void BusFault_Handler(void) {
	printf("Exception: BusFault\n");
	while(1);
}
void UsageFault_Handler_c(uint32_t *pBaseStackFrame) {
	// uint32_t *pUFSR = (uint32_t*) 0xE000ED2A;
	printf("Exception: UsageFault\n");
	// printf("UFSR = %x\n", (*pUFSR) & 0xff);
	// printf("MSP = %p\n", pBaseStackFrame);
	// printf("Value of R0 = %p\n", pBaseStackFrame[0]);
	// printf("Value of R1 = %p\n", pBaseStackFrame[1]);
	// printf("Value of R2 = %p\n", pBaseStackFrame[2]);
	// printf("Value of R3 = %p\n", pBaseStackFrame[3]);
	// printf("Value of R12 = %p\n", pBaseStackFrame[4]);
	// printf("Value of LR = %p\n", pBaseStackFrame[5]);
	// printf("Value of PC = %p\n", pBaseStackFrame[6]);
	// printf("Value of XPSR = %p\n", pBaseStackFrame[7]);
	while(1);
}
