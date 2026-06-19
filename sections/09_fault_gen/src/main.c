// // // undone

// // #include <stdio.h>

// // #define USAGEFAULTENA	18
// // #define BUSFAULTENA		17
// // #define MEMFAULTENA		16

// // extern void initialise_monitor_handles(void); // semihosting cfg

// // int main(void) {
// //     initialise_monitor_handles(); // semihosting

// // 	// 1. enable all configurable exceptions (usage fault, mem manage fault and bus fault)
// // 	uint32_t *pSHCRS = (uint32_t*) 0xE000ED24;
// // 	*pSHCRS |= (1 << USAGEFAULTENA) | (1 << BUSFAULTENA) | (1 << MEMFAULTENA);

// // 	// 3.  force the processor to execute some undefined instruction
// // 	uint32_t *pSRAM = (uint32_t*) 0x20010000;
// // 	*pSRAM = 0xFFFFFFFF;

// // 	void (*some_address) (void);
// // 	some_address = (void*) 0x20010001;
// // 	some_address();
// // 	// 4. analyze the fault

    
// //     while (1);
// //     return 0;
// // }

// // // 2. implement the fault handlers
// // void HardFault_Handler(void) {
// // 	printf("Exception: HardFault\n");
// // 	while(1);
// // }
// // void MemManage_Handler(void) {
// // 	printf("Exception: MemManage\n");
// // 	while(1);
// // }
// // void BusFault_Handler(void) {
// // 	printf("Exception: BusFault\n");
// // 	while(1);
// // }
// // void UsageFault_Handler_c(uint32_t *pBaseStackFrame) {
// // 	uint32_t *pUFSR = (uint32_t*) 0xE000ED2A;
// // 	printf("Exception: UsageFault\n");
// // 	printf("UFSR = %x\n", (*pUFSR) & 0xff);
// // 	printf("MSP = %p\n", pBaseStackFrame);
// // 	printf("Value of R0 = %p\n", pBaseStackFrame[0]);
// // 	printf("Value of R1 = %p\n", pBaseStackFrame[1]);
// // 	printf("Value of R2 = %p\n", pBaseStackFrame[2]);
// // 	printf("Value of R3 = %p\n", pBaseStackFrame[3]);
// // 	printf("Value of R12 = %p\n", pBaseStackFrame[4]);
// // 	printf("Value of LR = %p\n", pBaseStackFrame[5]);
// // 	printf("Value of PC = %p\n", pBaseStackFrame[6]);
// // 	printf("Value of XPSR = %p\n", pBaseStackFrame[7]);
// // 	while(1);
// // }

// // __attribute__ ((naked)) void UsageFault_Handler(void) {
// // 	// we extracted the value of MSP which happens to be
// // 	// base addr of the stack frame (saved during the exception
// // 	// entry from thread to handler mode)
// // 	__asm volatile ("MRS r0, MSP");
// // 	__asm ("B UsageFault_Handler_c");
// // }


// // divide by 0 ->

// #include <stdio.h>

// #define USAGEFAULTENA	18
// #define BUSFAULTENA		17
// #define MEMFAULTENA		16

// #define	DIV_0_TRP		4

// int fun_divide(int x, int y) {
// 	return x / y;
// }

// extern void initialise_monitor_handles(void); // semihosting cfg

// int main(void) {
//     initialise_monitor_handles(); // semihosting

// 	// 1. enable all configurable exceptions (usage fault, mem manage fault and bus fault)
// 	uint32_t *pSHCRS = (uint32_t*) 0xE000ED24;
// 	// *pSHCRS |= (1 << USAGEFAULTENA) | (1 << BUSFAULTENA) | (1 << MEMFAULTENA);

// 	// 2. enable divide by zero trap
// 	uint32_t *pCCR = 0xE000ED14;
// 	*pCCR |= (1 << DIV_0_TRP);

// 	// 3. attempt to divide by zero
// 	int value = fun_divide(5, 0);

//     while (1);
//     return 0;
// }

// // 2. implement the fault handlers
// void HardFault_Handler(void) {
// 	printf("Exception: HardFault\n");
// 	while(1);
// }
// void MemManage_Handler(void) {
// 	printf("Exception: MemManage\n");
// 	while(1);
// }
// void BusFault_Handler(void) {
// 	printf("Exception: BusFault\n");
// 	while(1);
// }
// void UsageFault_Handler_c(uint32_t *pBaseStackFrame) {
// 	uint32_t *pUFSR = (uint32_t*) 0xE000ED2A;
// 	printf("Exception: UsageFault\n");
// 	printf("UFSR = %x\n", (*pUFSR) & 0xffff);
// 	printf("MSP = %p\n", pBaseStackFrame);
// 	printf("Value of R0 = %p\n", pBaseStackFrame[0]);
// 	printf("Value of R1 = %p\n", pBaseStackFrame[1]);
// 	printf("Value of R2 = %p\n", pBaseStackFrame[2]);
// 	printf("Value of R3 = %p\n", pBaseStackFrame[3]);
// 	printf("Value of R12 = %p\n", pBaseStackFrame[4]);
// 	printf("Value of LR = %p\n", pBaseStackFrame[5]);
// 	printf("Value of PC = %p\n", pBaseStackFrame[6]);
// 	printf("Value of XPSR = %p\n", pBaseStackFrame[7]);
// 	while(1);
// }

// __attribute__ ((naked)) void UsageFault_Handler(void) {
// 	// we extracted the value of MSP which happens to be
// 	// base addr of the stack frame (saved during the exception
// 	// entry from thread to handler mode)
// 	__asm volatile ("MRS r0, MSP");
// 	__asm ("B UsageFault_Handler_c");
// }
