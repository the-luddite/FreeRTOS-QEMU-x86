/*--------------------------------------------------------------------
 Copyright(c) 2015 Intel Corporation. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in
 the documentation and/or other materials provided with the
 distribution.
 * Neither the name of Intel Corporation nor the names of its
 contributors may be used to endorse or promote products derived
 from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 --------------------------------------------------------------------*/

/*-----------------------------------------------------------------------
 * Any required includes
 *------------------------------------------------------------------------
 */
// #include "multiboot.h"
#include "platform_support.h"

/*-----------------------------------------------------------------------
 * Any required local definitions
 *------------------------------------------------------------------------
 */
#ifndef NULL
	#define NULL (void *)0
#endif

#define MUTEX_WAIT_TIME	(( TickType_t ) 8 )

/*-----------------------------------------------------------------------
 * Function prototypes
 *------------------------------------------------------------------------
 */
extern void *memcpy( void *pvDest, const void *pvSource, unsigned long ulBytes );

/*-----------------------------------------------------------------------
 * Global variables
 *------------------------------------------------------------------------
 */
uint32_t bootinfo = 1UL;
uint32_t bootsign = 1UL;

/*-----------------------------------------------------------------------
 * Static variables
 *------------------------------------------------------------------------
 */
static SemaphoreHandle_t semPrintfGate = 0;

/*------------------------------------------------------------------------
 * GDT default entries (used in GDT setup code)
 *------------------------------------------------------------------------
 */
static struct sd gdt_default[NGDE] =
{
	/*   sd_lolimit  sd_lobase   sd_midbase  sd_access   sd_hilim_fl sd_hibase */
	/* 0th entry NULL */
	{            0,          0,           0,         0,            0,        0, },
	/* 1st, Kernel Code Segment */
	{       0xffff,          0,           0,      0x9a,         0xcf,        0, },
	/* 2nd, Kernel Data Segment */
	{       0xffff,          0,           0,      0x92,         0xcf,        0, },
	/* 3rd, Kernel Stack Segment */
	{       0xffff,          0,           0,      0x92,         0xcf,        0, },
	/* 4st, Boot Code Segment */
	{       0xffff,          0,           0,      0x9a,         0xcf,        0, },
	/* 5th, Code Segment for BIOS32 request */
	{       0xffff,          0,           0,      0x9a,         0xcf,        0, },
	/* 6th, Data Segment for BIOS32 request */
	{       0xffff,          0,           0,      0x92,         0xcf,        0, },
};

extern struct sd gdt[];	/* Global segment table (defined in startup.S) */

/*------------------------------------------------------------------------
 * Set segment registers (used in GDT setup code)
 *------------------------------------------------------------------------
 */
void setsegs()
{
	extern int	__text_end;
	struct sd	*psd;
	uint32_t	np, ds_end;

	ds_end = 0xffffffff/PAGE_SIZE; 		/* End page number */

	psd = &gdt_default[1];				/* Kernel code segment */
	np = ((int)&__text_end - 0 + PAGE_SIZE-1) / PAGE_SIZE;	/* Number of code pages */
	psd->sd_lolimit = np;
	psd->sd_hilim_fl = FLAGS_SETTINGS | ((np >> 16) & 0xff);

	psd = &gdt_default[2];				/* Kernel data segment */
	psd->sd_lolimit = ds_end;
	psd->sd_hilim_fl = FLAGS_SETTINGS | ((ds_end >> 16) & 0xff);

	psd = &gdt_default[3];				/* Kernel stack segment */
	psd->sd_lolimit = ds_end;
	psd->sd_hilim_fl = FLAGS_SETTINGS | ((ds_end >> 16) & 0xff);

	psd = &gdt_default[4];				/* Boot code segment */
	psd->sd_lolimit = ds_end;
	psd->sd_hilim_fl = FLAGS_SETTINGS | ((ds_end >> 16) & 0xff);

	memcpy(gdt, gdt_default, sizeof(gdt_default));
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------------------
  * Debug serial port display update functions
  *------------------------------------------------------------------------
  */
 static void vCreatePrintfSemaphore( void )
 {
	if (semPrintfGate == 0)
	{
		semPrintfGate = xSemaphoreCreateRecursiveMutex();
		vQueueAddToRegistry( ( QueueHandle_t ) semPrintfGate, "g_printf_Mutex" );
	}
 }
 /*-----------------------------------------------------------*/

 void ClearScreen(void)
 {
	g_printf(ANSI_CLEAR_SB);
	g_printf(ANSI_CLEAR_SCREEN);
 }
 /*-----------------------------------------------------------*/

 void MoveToScreenPosition(uint8_t row, uint8_t col)
 {
	g_printf("%c[%d;%dH", (char) 0x1B, row, col);
 }
 /*-----------------------------------------------------------*/

 void UngatedMoveToScreenPosition(uint8_t row, uint8_t col)
 {
	printf("%c[%d;%dH", (char) 0x1B, row, col);
 }
/*-----------------------------------------------------------*/

 void SetScreenColor(const char *color)
 {
	 g_printf("%s", color);
 }
 /*-----------------------------------------------------------*/

 void g_printf(const char *format, ...)
 {

	 if (semPrintfGate == 0)
		 vCreatePrintfSemaphore();

	 if (xSemaphoreTakeRecursive(semPrintfGate, MUTEX_WAIT_TIME))
	 {
	     va_list arguments;
	     va_start(arguments,format);
	     print(0, format, arguments);
	     xSemaphoreGiveRecursive(semPrintfGate);
	 }
 }
 /*-----------------------------------------------------------*/

 void g_printf_rcc(uint8_t row, uint8_t col, const char *color, const char *format, ...)
 {
	 if (semPrintfGate == 0)
		 vCreatePrintfSemaphore();

	 if (xSemaphoreTakeRecursive(semPrintfGate, MUTEX_WAIT_TIME ))
	 {
		//  UngatedMoveToScreenPosition(row, col);
		 printf("%s",color);
	     va_list arguments;
	     va_start(arguments,format);
	     print(0, format, arguments);
		 xSemaphoreGiveRecursive(semPrintfGate);
	 }
}
 /*-----------------------------------------------------------*/

 void vPrintBanner( void )
 {
	/* Print an RTOSDemo Loaded message */
	ClearScreen();
	g_printf_rcc(1, 2, DEFAULT_BANNER_COLOR,
	"%c[1mHELLO from the FreeRTOS kernel!\n%c[0m",
	(char) 0x1B, (char) 0x1B );
	printf(ANSI_HIDE_CURSOR);
 }
 /*-----------------------------------------------------------*/


int consoleGetchar( void )
{
    return 0;
}
/*-----------------------------------------------------------*/


void vInitialiseTimerForIntQueueTest( void )
{
	/* In current state the demo doesn't support virtualized HPET */
	return;
}
/*-----------------------------------------------------------*/

static inline uint64_t rdtsc(void)
{
	uint32_t lo, hi;
	uint64_t tsc;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
	tsc = hi;
	tsc <<= 32;
	tsc |= lo;
	return tsc;
}
/*-----------------------------------------------------------*/

void vMicroSecondDelay(uint32_t DelayTime)
{
	uint64_t diff_in_us = 0;
	uint64_t cpufreq_in_mhz = 400;
	uint64_t tsc_start = rdtsc();
	uint64_t tsc_current = tsc_start;

	do
	{
		diff_in_us = ((tsc_current - tsc_start) / cpufreq_in_mhz);
		tsc_current = rdtsc();
	}
	while (diff_in_us < (uint64_t) DelayTime);
}
/*-----------------------------------------------------------*/

void vMilliSecondDelay(uint32_t DelayTime)
{
	vMicroSecondDelay (DelayTime * 1000);
}
/*-----------------------------------------------------------*/