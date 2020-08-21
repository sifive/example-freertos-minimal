/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

/******************************************************************************
 *
 * Minimal Application for starting developping an freeRTOS program 
 *
 ******************************************************************************/

/* Standard includes. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Freedom metal includes. */
#include <metal/platform.h>

#include <metal/uart.h>
#include <metal/interrupt.h>
#include <metal/clock.h>
#include <metal/led.h>

/*-----------------------------------------------------------*/
/*
 * Functions:
 * 		- prvSetupHardware: Setup Hardware according CPU and Board.
 */
static void prvSetupHardware( void );

/*-----------------------------------------------------------*/
struct metal_cpu cpu0;

#ifdef metal_led_ld0red
#define led0_red metal_led_ld0red
#else
#define led0_red metal_led_none
#endif
#ifdef metal_led_ld0blue
#define led0_blue metal_led_ld0blue
#else
#define led0_blue metal_led_none
#endif
#ifdef metal_led_ld0green
#define led0_green metal_led_ld0green
#else
#define led0_green metal_led_none
#endif

/*-----------------------------------------------------------*/
int main( void )
{
	const char * const pcMessage = "FreeRTOS Demo start\r\n";
	const char * const pcMessageEnd = "FreeRTOS Demo end\r\n";

	prvSetupHardware();
	write( STDOUT_FILENO, pcMessage, strlen( pcMessage ) );

	/* At this point, you can create queue,semaphore, task requested for your application */

	/* Start the tasks and timer running. */
	/* Here No task are defined, so if we start the Scheduler 2 tasks will running (Timer and Idle) */
	/* This task will run forever without doing nothing, it is why the following line is commented */
	//vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the Idle and/or
	timer tasks to be created. 
	or task have stoppped the Scheduler */

	write( STDOUT_FILENO, pcMessageEnd, strlen( pcMessageEnd ) );

}

/*-----------------------------------------------------------*/
static void prvSetupHardware( void )
{
	const char * const pcWarningMsg = "At least one of LEDs is null.\n";

	// This demo will toggle LEDs colors so we define them here
	// Enable each LED
	metal_led_enable(led0_red);
	metal_led_enable(led0_green);
	metal_led_enable(led0_blue);

	// All Off
	metal_led_on(led0_red);
	metal_led_on(led0_green);
	metal_led_on(led0_blue);
}
/*-----------------------------------------------------------*/


void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();

	// Red light on
	metal_led_off(led0_red);

	_exit(1);
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();

	write( STDOUT_FILENO, "ERROR Stack overflow on func: ", 30 );
	write( STDOUT_FILENO, pcTaskName, strlen( pcTaskName ) );

	// Red light on
	metal_led_off(led0_red);

	_exit(1);
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* The tests in the full demo expect some interaction with interrupts. */
}
/*-----------------------------------------------------------*/

void vAssertCalled( void )
{
	taskDISABLE_INTERRUPTS();

	// Red light on
	metal_led_off(led0_red);

	_exit(1);
}
