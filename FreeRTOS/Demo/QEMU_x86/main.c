/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://aws.amazon.com/freertos
 *
 */

/* Standard includes. */
#include <stdlib.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Standard demo includes, only necessary for the tick hook. */
#include "TimerDemo.h"
#include "QueueOverwrite.h"
#include "EventGroupsDemo.h"
#include "QueueSet.h"
#include "TaskNotify.h"
#include "IntQueue.h"

#include "platform_support.h"


/*-----------------------------------------------------------*/


extern void main_popping_tasks( void );


/* Prototypes for functions called from asm start up code. */
int main( void );
void CRT_Init( void );

/*
 * Prototypes for the standard FreeRTOS callback/hook functions implemented
 * within this file.
 */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );

/*
 * Perform any hardware/peripheral related initialisation necessary to run the
 * demo.
 */
static void prvSetupHardware( void );


/*
 * Helper functions used when an assert is triggered.  The first periodically
 * displays an assert message, and the second clears the assert message when the
 * function called by the configASSERT() macro is exited.
 */
static void prvDisplayAssertion( const char * pcFile, unsigned long ulLine );
static void prvClearAssertionLine( void );

/*-----------------------------------------------------------*/

int main( void )
{	
	/* Init the UART, GPIO, etc. */
	prvSetupHardware();

	main_popping_tasks();

	return 0;
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h.

	Force an assert. */
	configASSERT( xTaskGetTickCount() == 0 );
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.

	Increase the size of the stack allocated to the offending task.

	Force an assert. */
	configASSERT( pxTask == NULL );
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	volatile unsigned long xFreeHeapSpace;

	/* This is just a trivial example of an idle hook.  It is called on each
	cycle of the idle task.  It must *NOT* attempt to block.  In this case the
	idle task just queries the amount of FreeRTOS heap that remains.  See the
	memory management section on the http://www.FreeRTOS.org web site for memory
	management options.  If there is a lot of heap memory free then the
	configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
	RAM. */
	xFreeHeapSpace = xPortGetFreeHeapSize();

	/* Remove compiler warning about xFreeHeapSpace being set but never used. */
	( void ) xFreeHeapSpace;
}
/*-----------------------------------------------------------*/

static void prvDisplayAssertion( const char * pcFile, unsigned long ulLine )
{
extern void vMilliSecondDelay( uint32_t DelayTime );
const uint32_t ul500ms = 500UL;

	/* Display assertion file and line. Don't use the gated g_printf just in
	the assert was triggered while the gating semaphore was taken.  Always print
	on line 23. */
	UngatedMoveToScreenPosition( 23, 2 );
	printf( ANSI_COLOR_RED );
	printf( "ASSERT: File = %s, Line = %u\n\r", pcFile, ulLine );
	printf( ANSI_COLOR_RESET );
	printf( ANSI_SHOW_CURSOR );
	vMilliSecondDelay( ul500ms );
}
/*-----------------------------------------------------------*/

static void prvClearAssertionLine( void )
{
	UngatedMoveToScreenPosition( 23, 1 );
	printf( ANSI_COLOR_RESET );
	printf( ANSI_CLEAR_LINE );
	printf( ANSI_HIDE_CURSOR );
}
/*-----------------------------------------------------------*/

void vAssertCalled( const char * pcFile, unsigned long ulLine )
{
volatile uint32_t ul = 0;

	( void ) pcFile;
	( void ) ulLine;

	taskENTER_CRITICAL();
	{
		/* Set ul to a non-zero value or press a key to step out of this
		function in order to inspect the location of the assert(). */

		/* Clear any pending key presses. */
		while( consoleGetchar() != 0 )
		{
			/* Nothing to do here - the key press is just discarded. */
		}

		do
		{
		   prvDisplayAssertion(pcFile, ulLine);
		} while ( ( ul == pdFALSE ) && ( consoleGetchar() == 0 ) );

		prvClearAssertionLine();
	}
	taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	return;
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	vPrintBanner();
}
/*-----------------------------------------------------------*/


void CRT_Init( void )
{
extern uint32_t __bss_start[];
extern uint32_t __bss_end[];
extern uint32_t __data_vma[];
extern uint32_t __data_lma[];
extern uint32_t __data_start[];
extern uint32_t __data_end[];
uint32_t x = 255;
size_t xSize;

	/* Zero out bss. */
	xSize = ( ( size_t ) __bss_end ) - ( ( size_t ) __bss_start );
	memset( ( void * ) __bss_start, 0x00, xSize );

	/* Copy initialised variables. */
	xSize = ( ( size_t ) __data_end ) - ( ( size_t ) __data_start );
	memcpy( ( void * ) __data_vma, __data_lma, xSize );

	/* Ensure no interrupts are pending. */
	do
	{
		portAPIC_EOI = 0;
		x--;
	} while( x > 0 );
}