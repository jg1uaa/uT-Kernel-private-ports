/*
 *----------------------------------------------------------------------
 *    micro T-Kernel
 *
 *    Copyright (C) 2006-2011 by Ken Sakamura. All rights reserved.
 *    micro T-Kernel is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Version:   1.01.01
 *    Released by T-Engine Forum(http://www.t-engine.org) at 2011/12/12.
 *
 *----------------------------------------------------------------------
 */

#include <basic.h>

#define	MSTCRH		((_UB *)0x00ffe01c)
#define	MSTPH		(1 << 1)

#define	SMR		((_UB *)0x00ffffb8)	// SCI #1
#define	BRR		((_UB *)0x00ffffb9)
#define	SCR		((_UB *)0x00ffffba)
#define	TDR		((_UB *)0x00ffffbb)
#define	SSR		((_UB *)0x00ffffbc)
#define	RDR		((_UB *)0x00ffffbd)
#define	SCMR		((_UB *)0x00ffffbe)

/* SSR */
#define	TDRE		(1 << 7)
#define	RDRF		(1 << 6)
#define	OER		(1 << 5)
#define	FER		(1 << 4)
#define	PER		(1 << 3)
#define	TEND		(1 << 2)

/* SCR */
#define	TE		(1 << 5)
#define	RE		(1 << 4)

#define	SYSCLK		(CPU_CLOCK)		/* System clock (Hz) */
#define	UARTCLK		(SYSCLK / 4)		/* UART clock */
#define	RS_BAUD_RATE	(38400UL)		/* 38.4kbps */

char Sci2AsyncTx(unsigned char Data);
char Sci2AsyncRx(unsigned char *pData);

int sio_send_frame(const unsigned char* buf, int size)
{
	int i;

	for(i = 0; i < size; i++) Sci2AsyncTx(buf[i]);

	return i;
}

int sio_recv_frame(unsigned char* buf, int size)
{
	int i;

	for(i = 0; i < size; i++) while(Sci2AsyncRx(&buf[i]));

	return i;
}

void sio_init( void )
{
	unsigned long dly;

	*MSTCRH &= ~MSTPH;			// SCI power-up	
	*SCMR = 0x00;				// SmartCard I/F disable

	*SCR = 0x00;				// TE=0, RE=0
	*SMR = 0x01;				// 8N1, system clock / 4

	*BRR = (UARTCLK + 16 * RS_BAUD_RATE) / (32 * RS_BAUD_RATE) - 1;

	dly = SYSCLK / (2 * RS_BAUD_RATE);
	while (dly--){ Asm("nop"); }

	*SCR = TE | RE;
}

char Sci2AsyncTx(unsigned char Data)
{
	while (!(*SSR & TDRE));
	*TDR = Data;
	*SSR &= ~TDRE;				// SCI needs this procedure
	while (!(*SSR & TEND));

	return 1;
}

char Sci2AsyncRx(unsigned char *pData)
{
	char	ssr = *SSR;

	if (ssr & FER) {
		*SSR &= ~FER;
		return 1;
	}
	if (ssr & PER) {
		*SSR &= ~PER;
		return 2;
	}
	if (ssr & OER) {
		*SSR &= ~OER;
		return 3;
	}
	if (ssr & RDRF) {
		*pData = *RDR;
		*SSR &= ~RDRF;			// SCI needs this procedure
		return 0;
	}

	return -1;
}

