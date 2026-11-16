/*
 * FreeRTOS configuration for:
 *   MCU:       STM32F103C8T6 (Cortex-M3, 72 MHz, 20 KB SRAM)
 *   Toolchain: GCC / STM32CubeIDE
 *   Drivers:   CMSIS only (no HAL, no SPL)
 *
 * Intended as a starting point for the FreeRTOS Kernel 10.x
 * GCC ARM_CM3 port.
 *
 * IMPORTANT:
 * 1. Make sure the project uses the GCC/ARM_CM3 FreeRTOS port.
 * 2. The startup vector table must resolve SVC_Handler,
 *    PendSV_Handler and SysTick_Handler to the FreeRTOS handlers.
 * 3. configCPU_CLOCK_HZ must match the actual CPU clock after
 *    SystemInit().
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>
#include <stddef.h>

/*-----------------------------------------------------------
 * Scheduler
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION                    1
#define configUSE_TIME_SLICING                  1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0

#define configCPU_CLOCK_HZ                      ((uint32_t)72000000UL)
#define configTICK_RATE_HZ                      ((TickType_t)1000)

#define configMAX_PRIORITIES                    5
#define configMINIMAL_STACK_SIZE                ((uint16_t)128)
#define configMAX_TASK_NAME_LEN                 16

/* FreeRTOS heap managed by heap_4.c. 8 KB is a conservative
 * starting point for the STM32F103C8T6 (20 KB SRAM). */
#define configTOTAL_HEAP_SIZE                   ((size_t)(8U * 1024U))

#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1

/*-----------------------------------------------------------
 * Memory allocation
 *----------------------------------------------------------*/

#define configSUPPORT_STATIC_ALLOCATION         0
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configAPPLICATION_ALLOCATED_HEAP        0

/*-----------------------------------------------------------
 * Hooks and diagnostics
 *----------------------------------------------------------*/

#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configUSE_MALLOC_FAILED_HOOK            0
#define configCHECK_FOR_STACK_OVERFLOW          0

#define configUSE_TRACE_FACILITY                0
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/*-----------------------------------------------------------
 * Synchronisation and task features
 *----------------------------------------------------------*/

#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configUSE_TASK_NOTIFICATIONS            1

#define configUSE_QUEUE_SETS                    0
#define configUSE_APPLICATION_TASK_TAG          0

/* Software timers are disabled in this minimal configuration.
 * Set to 1 and add timers.c/timers.h if you need them. */
#define configUSE_TIMERS                        0

/*-----------------------------------------------------------
 * API inclusion
 *----------------------------------------------------------*/

#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_eTaskGetState                   1

/*-----------------------------------------------------------
 * Cortex-M3 NVIC interrupt priorities
 *
 * STM32F1 implements 4 priority bits:
 *   logical priority 0  = highest urgency
 *   logical priority 15 = lowest urgency
 *
 * configKERNEL_INTERRUPT_PRIORITY and
 * configMAX_SYSCALL_INTERRUPT_PRIORITY are the
 * 8-bit-shifted NVIC register values expected by the port.
 * 0xFF = lowest priority; 0xBF = logical priority 11.
 *----------------------------------------------------------*/

#define configPRIO_BITS                         4

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY        15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY   11

#define configKERNEL_INTERRUPT_PRIORITY \
    (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

/* FreeRTOS Cortex-M3 exception handlers.
 * These aliases match the standard STM32 CMSIS startup names. */
#define vPortSVCHandler                         SVC_Handler
#define xPortPendSVHandler                      PendSV_Handler
#define xPortSysTickHandler                     SysTick_Handler

/*-----------------------------------------------------------
 * Assertions
 *----------------------------------------------------------*/

#define configASSERT(x) \
    do { \
        if ((x) == 0) { \
            taskDISABLE_INTERRUPTS(); \
            for (;;) { } \
        } \
    } while (0)

#endif /* FREERTOS_CONFIG_H */
