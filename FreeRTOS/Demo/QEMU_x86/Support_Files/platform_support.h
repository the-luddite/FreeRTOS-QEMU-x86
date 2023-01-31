#ifndef __PLATFORM_SUPPORT_H__
#define __PLATFORM_SUPPORT_H__

#ifdef __cplusplus
	extern "C" {
#endif

//---------------------------------------------------------------------
// Any required includes
//---------------------------------------------------------------------
#include "FreeRTOS.h"
#include "semphr.h"
#include "Qemu/debugcon_printf.h"

//---------------------------------------------------------------------
// Application main entry point
//---------------------------------------------------------------------
extern int main( void );

//---------------------------------------------------------------------
// Defines for GDT
//---------------------------------------------------------------------
#define	NGDE				8		/* Number of global descriptor entries	*/
#define FLAGS_GRANULARITY	0x80
#define FLAGS_SIZE			0x40
#define	FLAGS_SETTINGS		( FLAGS_GRANULARITY | FLAGS_SIZE )
#define	PAGE_SIZE			4096

struct __attribute__ ((__packed__)) sd
{
	unsigned short	sd_lolimit;
	unsigned short	sd_lobase;
	unsigned char	sd_midbase;
	unsigned char   sd_access;
	unsigned char	sd_hilim_fl;
	unsigned char	sd_hibase;
};

void setsegs();

//---------------------------------------------------------------------
// Debug serial port display update definitions
//---------------------------------------------------------------------
#define ANSI_CLEAR_SB			"\e[3J"
#define ANSI_CLEAR_LINE			"\x1b[2K"
#define ANSI_CLEAR_SCREEN		"\x1b[2J"
#define ANSI_COLOR_RED     		"\x1b[31m"
#define ANSI_COLOR_GREEN   		"\x1b[32m"
#define ANSI_COLOR_YELLOW  		"\x1b[33m"
#define ANSI_COLOR_BLUE    		"\x1b[34m"
#define ANSI_COLOR_MAGENTA 		"\x1b[35m"
#define ANSI_COLOR_CYAN    		"\x1b[36m"
#define ANSI_COLOR_RESET   		"\x1b[0m"
#define ANSI_COLOR_WHITE   		ANSI_COLOR_RESET

#define DEFAULT_SCREEN_COLOR	ANSI_COLOR_YELLOW
#define DEFAULT_BANNER_COLOR	ANSI_COLOR_CYAN

#define ANSI_HIDE_CURSOR		"\x1b[?25l"
#define ANSI_SHOW_CURSOR		"\x1b[?25h"

void ClearScreen(void);
void MoveToScreenPosition(uint8_t row, uint8_t col);
void UngatedMoveToScreenPosition(uint8_t row, uint8_t col);
void SetScreenColor(const char *);
void g_printf(const char *format, ...);
void g_printf_rcc(uint8_t row, uint8_t col, const char *color, const char *format, ...);
void vPrintBanner( void );
int consoleGetchar( void);
void vMicroSecondDelay(uint32_t DelayTime);
void vMilliSecondDelay(uint32_t DelayTime);

#ifdef __cplusplus
	} /* extern C */
#endif

#endif __PLATFORM_SUPPORT_H__