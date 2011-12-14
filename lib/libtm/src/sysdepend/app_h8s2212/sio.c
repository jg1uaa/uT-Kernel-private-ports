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

#define MSTP_CRB	((_UB*)(0x00fffde8UL + 1UL))
#define PB_DR		((_UB*)(0x00fffe3aUL + 0xd0UL))
#define SCI2_SMR	((_UB*)(0x00ffff88UL))
#define SCI2_BRR	((_UB*)(0x00ffff89UL))
#define SCI2_SCR	((_UB*)(0x00ffff8aUL))
#define SCI2_TDR	((_UB*)(0x00ffff8bUL))
#define SCI2_SSR	((_UB*)(0x00ffff8cUL))
#define SCI2_RDR	((_UB*)(0x00ffff8dUL))
#define SCI2_SCMR	((_UB*)(0x00ffff8eUL))


#define CPUCLK		(4000000UL)			/* CPU clock : 4MHz */
#define SYSCLK		(CPUCLK)			/* System clock (Hz) */
#define RS_BAUD_RATE	(9600)				/* 9.6kbps */

void Sci2AsyncMode( void );
char Sci2AsyncTx(unsigned char Data);
char Sci2AsyncRx(unsigned char *pData);



int sio_send_frame(const unsigned char* buf, int size)
{
	int i;

	*MSTP_CRB &= (unsigned char)~(1U << 5);
	*PB_DR &= ~(1UL << 0);
	*SCI2_SCR |= (1UL << 5);		/* TE=1 */
	for(i = 0; i < size; i++) {
		Sci2AsyncTx(buf[i]);
	}
	while ((*SCI2_SSR & (1U << 2)) == 0){};
	*SCI2_SCR &= ~(1UL << 5);		/* TE=0 */
	*PB_DR |= (1UL << 0);
	*MSTP_CRB |= (1U << 5);

	return i;
}

int sio_recv_frame(unsigned char* buf, int size)
{
	int i;

	*MSTP_CRB &= (UB)~(1U << 5);
	*PB_DR &= ~(1UL << 0);
	*SCI2_SCR |= (1UL << 4);		/* TR=1 */
	for(i = 0; i < size; i++) {
		while(Sci2AsyncRx(&buf[i]) != 0) {
			;
		}
	}
	*PB_DR |= (1UL << 0);
	*SCI2_SCR &= ~(1UL << 4);		/* TR=0 */
	*MSTP_CRB |= (1U << 5);

	return i;
}

void sio_nop(void) {
	Asm("nop\n\tnop\n\tnop\n\tnop");
}

void sio_init( void )
{
	unsigned char Brr;
	unsigned long dly;

	*MSTP_CRB &= (UB)~(1U << 5);
	*PB_DR &= ~(1UL << 0);

	*SCI2_SCR = 0x00;			/* TE = 0 RE = 0 */
	*SCI2_SCMR = 0x00;			/* LSB first */
	*SCI2_SMR = 0x00;			/* Data8 Stop1 */
	Brr = (UB)((1000000 / 64 * 2) / RS_BAUD_RATE * (CPUCLK / 1000 / 1000) +.5);
	*SCI2_BRR = Brr;

	dly = SYSCLK / RS_BAUD_RATE;
	while ((dly--) != 0){ sio_nop(); }

	*PB_DR |= (1UL << 0);
	*MSTP_CRB |= (1U << 5);
}

char Sci2AsyncTx(unsigned char Data)
{
	while ((*SCI2_SSR & (1U << 7)) == 0){};		/* TDRE */
	*SCI2_TDR = Data;
	*SCI2_SSR &= ~(1U << 7);

	return 1;
}

char Sci2AsyncRx(unsigned char *pData)
{
	if ((*SCI2_SSR & (1U << 4)) != 0){	/* FER */
		*SCI2_SSR &= ~(1U << 4);
		return 1;
	}
	if ((*SCI2_SSR & (1U << 3)) != 0){	/* PER */
		*SCI2_SSR &= ~(1U << 3);
		return 2;
	}
	if ((*SCI2_SSR & (1U << 5)) != 0){	/* ORER */
		*SCI2_SSR &= ~(1U << 5);
		return 3;
	}

	if ((*SCI2_SSR & (1U << 6)) != 0){	/* RDRF */
		*pData = *SCI2_RDR;
		*SCI2_SSR &= ~(1U << 6);
		return 0;
	}

	return -1;
}

