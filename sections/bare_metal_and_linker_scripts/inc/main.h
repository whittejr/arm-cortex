/**
 * @file main.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2026-04-01
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

#define MAX_TASKS			4
#define TASK_RUNNING_STATE  0x00
#define TASK_BLOCKED_STATE  0xFF

/* some stack memory calculations */
#define SIZE_TASK_STACK		1024U
#define SIZE_SCHED_STACK	1024U

#define SRAM_START 			0x20000008U // start of the sram
#define SRAM_SIZE  			( (192) * (1024) )
#define SRAM_END			( (SRAM_START) + (SRAM_SIZE) )

#define T1_STACK_START		SRAM_END
#define T2_STACK_START		( (SRAM_END) - (1 * SIZE_TASK_STACK) )
#define T3_STACK_START		( (SRAM_END) - (2 * SIZE_TASK_STACK) )
#define T4_STACK_START		( (SRAM_END) - (3 * SIZE_TASK_STACK) )
#define SCHED_STACK_START	( (SRAM_END) - (4 * SIZE_TASK_STACK) )

/* systick config */
#define TICK_HZ				1000U
#define	MSI_CLK				4000000U
#define SYSTICK_TIM_CLK		MSI_CLK

#define DUMMY_XPSR 0x01000000

typedef struct {
    uint32_t psp_value;
    uint32_t block_count;
    uint8_t current_state;
    void (*task_handler) (void);
}TCB_t;

TCB_t user_tasks[MAX_TASKS];


#endif // MAIN_H
