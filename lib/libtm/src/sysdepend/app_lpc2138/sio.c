// --- public domain, no warranty.

#include <basic.h>

#define	uartBase		0xe000c000
#define	uartStep		0x04
#define	uartClock		(CPU_CLOCK / 4)	// Hz
#define	uartSpeed		38400		// bps
#define	uartDivisor		(uartClock / (uartSpeed * 16))
Inline	UW	uartRead(UW adr) {return *(_UB*)adr;}
Inline	void	uartWrite(UW adr, UW dat) {*(_UB*)adr = dat;}

#define	RHR		(uartBase + (0x00 * uartStep))	// r
#define	THR		(uartBase + (0x00 * uartStep))	//  w
#define	IER		(uartBase + (0x01 * uartStep))	// rw
#define	ISR		(uartBase + (0x02 * uartStep))	// r
#define	FCR		(uartBase + (0x02 * uartStep))	//  w
#define	LCR		(uartBase + (0x03 * uartStep))	// rw
#define	MCR		(uartBase + (0x04 * uartStep))	// rw
#define	LSR		(uartBase + (0x05 * uartStep))	// r

#define	DLL		(uartBase + (0x00 * uartStep))	// rw
#define	DLM		(uartBase + (0x01 * uartStep))	// rw

/* LPC23XX specific: pin selection, power control */
#define	PCONP		((_UW*)0xe01fc0c4)
#define	PINSEL0		((_UW*)0xe002c000)


int	sio_send_frame(const unsigned char *buf, int size)
{
	int	i;

	for (i = 0; i < size; i++) {
		while(!(uartRead(LSR) & 0x20));		// wait for Tx ready
		uartWrite(THR, *buf++);
	}

	return i;
}

int	sio_recv_frame(unsigned char* buf, int size)
{
	int	i, s, c;

	for (i = 0; i < size; i++) {
		while (1) {
			s = uartRead(LSR);		// get status
			if (s & 0x1f) {			// Rx ready
				c = uartRead(RHR);	// read from FIFO
				if (!(s & 0x1e)) {	// no error
					*buf++ = c;
					break;
				}
			}
		}
	}

	return i;
}

void	sio_init(void)
{
	*PCONP |= (1 << 3);		// UART0 power-up
	*PINSEL0 = (*PINSEL0 & ~(0x0f << 0)) | (0x05 << 0);
					// TXD0/RXD0 enable

	uartWrite(IER, 0x00);		// disable interrupt
	uartWrite(LCR, 0x80);		// set divisor
	uartWrite(DLM, 0xff);		// (avoid divisor=0)
	uartWrite(DLL, (uartDivisor >> 0) & 0xff);
	uartWrite(DLM, (uartDivisor >> 8) & 0xff);
	uartWrite(LCR, 0x03);		// data 8bit, stop 1bit, non-parity
	uartWrite(MCR, 0x03);		// RTS#, DTR# assert
	uartWrite(FCR, 0x07);		// FIFO enable and reset

	return;
}
