/**
  ******************************************************************************
  * @file      startup_stm32f10x_md.S
  * @author    MCD Application Team
  * @version   V3.5.0
  * @date      11-March-2011
  * @brief     STM32F10x Medium Density Devices vector table for GCC toolchain.
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler
  *                - Set the vector table entries with the exceptions ISR address
  *                - Configure the clock system
  *                - Branches to __main in the C library (which eventually
  *                  calls main()).
  *            After Reset the CortexM3 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  */

.syntax unified
.cpu cortex-m3
.thumb

/* Stack and Heap sizes */
.equ Stack_Size, 0x00000400
.equ Heap_Size,  0x00000200

/* Stack memory */
.section .stack
.align 3
.global __StackTop
.global __StackLimit
.global __initial_sp
__StackLimit:
.space Stack_Size
.size __StackLimit, . - __StackLimit
__StackTop:
.size __StackTop, 0
__initial_sp:   /* provide alias for compatibility */
.word __StackTop

/* Heap memory */
.section .heap
.align 3
.global __HeapBase
.global __HeapLimit
.global __heap_base
.global __heap_limit
__HeapBase:
.space Heap_Size
.size __HeapBase, Heap_Size
__HeapLimit:
.size __HeapLimit, 0
__heap_base:   /* provide alias */
.word __HeapBase
__heap_limit:  /* provide alias */
.word __HeapLimit

/* Vector Table */
.section .isr_vector,"a",%progbits
.align 2
.global __Vectors
.global __Vectors_End
.global __Vectors_Size
__Vectors:
    .word __StackTop            /* Top of Stack */
    .word Reset_Handler         /* Reset Handler */
    .word NMI_Handler
    .word HardFault_Handler
    .word MemManage_Handler
    .word BusFault_Handler
    .word UsageFault_Handler
    .word 0                     /* Reserved */
    .word 0                     /* Reserved */
    .word 0                     /* Reserved */
    .word 0                     /* Reserved */
    .word SVC_Handler
    .word DebugMon_Handler
    .word 0                     /* Reserved */
    .word PendSV_Handler
    .word SysTick_Handler

    /* External Interrupts */
    .word WWDG_IRQHandler
    .word PVD_IRQHandler
    .word TAMPER_IRQHandler
    .word RTC_IRQHandler
    .word FLASH_IRQHandler
    .word RCC_IRQHandler
    .word EXTI0_IRQHandler
    .word EXTI1_IRQHandler
    .word EXTI2_IRQHandler
    .word EXTI3_IRQHandler
    .word EXTI4_IRQHandler
    .word DMA1_Channel1_IRQHandler
    .word DMA1_Channel2_IRQHandler
    .word DMA1_Channel3_IRQHandler
    .word DMA1_Channel4_IRQHandler
    .word DMA1_Channel5_IRQHandler
    .word DMA1_Channel6_IRQHandler
    .word DMA1_Channel7_IRQHandler
    .word ADC1_2_IRQHandler
    .word USB_HP_CAN1_TX_IRQHandler
    .word USB_LP_CAN1_RX0_IRQHandler
    .word CAN1_RX1_IRQHandler
    .word CAN1_SCE_IRQHandler
    .word EXTI9_5_IRQHandler
    .word TIM1_BRK_IRQHandler
    .word TIM1_UP_IRQHandler
    .word TIM1_TRG_COM_IRQHandler
    .word TIM1_CC_IRQHandler
    .word TIM2_IRQHandler
    .word TIM3_IRQHandler
    .word TIM4_IRQHandler
    .word I2C1_EV_IRQHandler
    .word I2C1_ER_IRQHandler
    .word I2C2_EV_IRQHandler
    .word I2C2_ER_IRQHandler
    .word SPI1_IRQHandler
    .word SPI2_IRQHandler
    .word USART1_IRQHandler
    .word USART2_IRQHandler
    .word USART3_IRQHandler
    .word EXTI15_10_IRQHandler
    .word RTCAlarm_IRQHandler
    .word USBWakeUp_IRQHandler
__Vectors_End:

.equ __Vectors_Size, __Vectors_End - __Vectors

/* Reset Handler */
.section .text.Reset_Handler
.weak Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    /* Call SystemInit */
    ldr r0, =SystemInit
    blx r0
    /* Jump to main */
    ldr r0, =main
    bx r0
.size Reset_Handler, . - Reset_Handler

/* Default exception/interrupt handlers */
/* Weak aliases to default infinite loop */
.macro DEFAULT_HANDLER handler_name
    .weak \handler_name
    .thumb_set \handler_name, Default_Handler
.endm

DEFAULT_HANDLER NMI_Handler
DEFAULT_HANDLER HardFault_Handler
DEFAULT_HANDLER MemManage_Handler
DEFAULT_HANDLER BusFault_Handler
DEFAULT_HANDLER UsageFault_Handler
DEFAULT_HANDLER SVC_Handler
DEFAULT_HANDLER DebugMon_Handler
DEFAULT_HANDLER PendSV_Handler
DEFAULT_HANDLER SysTick_Handler

DEFAULT_HANDLER WWDG_IRQHandler
DEFAULT_HANDLER PVD_IRQHandler
DEFAULT_HANDLER TAMPER_IRQHandler
DEFAULT_HANDLER RTC_IRQHandler
DEFAULT_HANDLER FLASH_IRQHandler
DEFAULT_HANDLER RCC_IRQHandler
DEFAULT_HANDLER EXTI0_IRQHandler
DEFAULT_HANDLER EXTI1_IRQHandler
DEFAULT_HANDLER EXTI2_IRQHandler
DEFAULT_HANDLER EXTI3_IRQHandler
DEFAULT_HANDLER EXTI4_IRQHandler
DEFAULT_HANDLER DMA1_Channel1_IRQHandler
DEFAULT_HANDLER DMA1_Channel2_IRQHandler
DEFAULT_HANDLER DMA1_Channel3_IRQHandler
DEFAULT_HANDLER DMA1_Channel4_IRQHandler
DEFAULT_HANDLER DMA1_Channel5_IRQHandler
DEFAULT_HANDLER DMA1_Channel6_IRQHandler
DEFAULT_HANDLER DMA1_Channel7_IRQHandler
DEFAULT_HANDLER ADC1_2_IRQHandler
DEFAULT_HANDLER USB_HP_CAN1_TX_IRQHandler
DEFAULT_HANDLER USB_LP_CAN1_RX0_IRQHandler
DEFAULT_HANDLER CAN1_RX1_IRQHandler
DEFAULT_HANDLER CAN1_SCE_IRQHandler
DEFAULT_HANDLER EXTI9_5_IRQHandler
DEFAULT_HANDLER TIM1_BRK_IRQHandler
DEFAULT_HANDLER TIM1_UP_IRQHandler
DEFAULT_HANDLER TIM1_TRG_COM_IRQHandler
DEFAULT_HANDLER TIM1_CC_IRQHandler
DEFAULT_HANDLER TIM2_IRQHandler
DEFAULT_HANDLER TIM3_IRQHandler
DEFAULT_HANDLER TIM4_IRQHandler
DEFAULT_HANDLER I2C1_EV_IRQHandler
DEFAULT_HANDLER I2C1_ER_IRQHandler
DEFAULT_HANDLER I2C2_EV_IRQHandler
DEFAULT_HANDLER I2C2_ER_IRQHandler
DEFAULT_HANDLER SPI1_IRQHandler
DEFAULT_HANDLER SPI2_IRQHandler
DEFAULT_HANDLER USART1_IRQHandler
DEFAULT_HANDLER USART2_IRQHandler
DEFAULT_HANDLER USART3_IRQHandler
DEFAULT_HANDLER EXTI15_10_IRQHandler
DEFAULT_HANDLER RTCAlarm_IRQHandler
DEFAULT_HANDLER USBWakeUp_IRQHandler

/* Default infinite loop handler */
.section .text.Default_Handler
.weak Default_Handler
.type Default_Handler, %function
Default_Handler:
    b .
.size Default_Handler, . - Default_Handler

/* User stack and heap initialization (optional) */
/* Provide __use_two_region_memory if needed */
/* For microlib compatibility */
.ifdef __MICROLIB
    .global __initial_sp
    .global __heap_base
    .global __heap_limit
.else
    .global __user_initial_stackheap
    __user_initial_stackheap:
        ldr r0, =__HeapBase
        ldr r1, =(__StackLimit + Stack_Size)
        ldr r2, =(__HeapBase + Heap_Size)
        ldr r3, =__StackLimit
        bx lr
.endif

.end
