/*
	Write a program to ADD, SUBTRACT, MULTIPLY and DIVIDE 2 operands
	using SVC HANDLER and return the result to the THREAD MODE code
	and print the result. THREAD MODE code should pass 2 operands via
	the stack frame. USE THE SVC NUMBER TO DECIDE THE OPERATION

		Service Number | Operation
		36				 Addition
		37				 Subtraction
		38			 	 Multiplication
		39				 Division
*/

#include <stdint.h>
#include <stdio.h>

int32_t add_numbers(int32_t x, int32_t y) {
	int32_t res;
	__asm volatile("SVC #36 ");
	__asm volatile("MOV %0,R0": "=r"(res)::);
	return res;

}
int32_t sub_numbers(int32_t x, int32_t y) {
	int32_t res;
	__asm volatile("SVC #37 ");
	__asm volatile("MOV %0,R0": "=r"(res)::);
	return res;
}
int32_t mul_numbers(int32_t x, int32_t y) {
	int32_t res;
	__asm volatile("SVC #38 ");
	__asm volatile("MOV %0,R0": "=r"(res)::);
	return res;
}
int32_t div_numbers(int32_t x, int32_t y) {
	int32_t res;
	__asm volatile("SVC #39 ");
	__asm volatile("MOV %0,R0": "=r"(res)::);
	return res;
}

extern void initialise_monitor_handles(void); // semihosting cfg

int main(void) {
    initialise_monitor_handles(); // semihosting

	int32_t res;
	
	res = add_numbers(40, -90);
	printf("add result = %ld\n", res);

	res = sub_numbers(25, 150);
	printf("sub result = %ld\n", res);

	res = mul_numbers(374, 890);
	printf("mul result = %ld\n", res);

	res = div_numbers(67, -3);
	printf("div result = %ld\n", res);
	
	while(1);
}

// handler mode
__attribute__ ((naked)) void SVC_Handler(void) {
	__asm volatile("MRS R0, MSP");
	__asm volatile("B SVC_Handler_c");
}

void SVC_Handler_c(uint32_t *pBaseStackFrame) {
	printf("in SVC Handler\n");

	int32_t arg0, arg1, res;

	// 1. get the value of the MSP
	uint8_t *pReturn_addr = (uint8_t*) pBaseStackFrame[6];

	// 2. decrement the return addr by 2 to point to
	// opcode of the SVC instruction in the program memory
	pReturn_addr -= 2;

	// extract the SVC number (LSByte of the opcode)
	uint8_t svc_number = *pReturn_addr;
	printf("svc number is %d\n", svc_number);

	arg0 = pBaseStackFrame[0];
	arg1 = pBaseStackFrame[1];

	switch (svc_number) {
		case 36:
			res = arg0 + arg1;
			break;
		case 37:
			res = arg0 - arg1;
			break;
		case 38:
			res = arg0 * arg1;
			break;
		case 39:
			res = arg0 / arg1;
			break;
		default:
			printf("invalid svc code\n");
			break;
	}
	pBaseStackFrame[0] = res;
}
