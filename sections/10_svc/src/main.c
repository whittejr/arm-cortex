/* Write a program to execute an SVC instruction from THREAD MODE,
   implement the SVC HANDLER to print the SVC NUMBER used. Also,
   increment the SVC NUMBER BY 4 and return it to THREAD MODE
   and PRINT IT

Hints:
	1: Write a main() function vwhere you should execute the SVC
	   instruction wuth an argument. like SVC #0x5
	2: Implement the SVC HANDLER
	3: In the SVC HANDLER, extract the SVC NUMBER and print it
	4: Increment the SVC NUMBER by 4 and return it to THREAD MODE
*/

#include <stdio.h>

extern void initialise_monitor_handles(void); // semihosting cfg

int main(void) {
    initialise_monitor_handles(); // semihosting

	__asm("SVC #25");
	// register uint32_t data __asm("r0");
	uint32_t data;
	__asm volatile("MOV %0,R0": "=r"(data) ::);
	printf("data = %ld\n", data);

	while(1);
	return 0;
}

// handler mode
__attribute__ ((naked)) void SVC_Handler(void) {
	__asm volatile("MRS R0, MSP");
	__asm volatile("B SVC_Handler_c");
}

void SVC_Handler_c(uint32_t *pBaseStackFrame) {
	printf("in SVC Handler\n");

	// 1. get the value of the MSP
	uint8_t *pReturn_addr = (uint8_t*) pBaseStackFrame[6];

	// 2. decrement the return addr by 2 to point to
	// opcode of the SVC instruction in the program memory
	pReturn_addr -= 2;

	// extract the SVC number (LSByte of the opcode)
	uint8_t svc_number = *pReturn_addr;
	printf("svc number is %d\n", svc_number);

	svc_number += 4;
	pBaseStackFrame[0] = svc_number;
}
