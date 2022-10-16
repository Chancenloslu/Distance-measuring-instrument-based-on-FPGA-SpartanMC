/*
_______________________________________________________________________________

 Copyright (c) 2012 TU Dresden, Chair for Embedded Systems
 (http://www.mr.inf.tu-dresden.de) All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 3. All advertising materials mentioning features or use of this software
    must display the following acknowledgement: "This product includes
    software developed by the TU Dresden Chair for Embedded Systems and
    its contributors."

 4. Neither the name of the TU Dresden Chair for Embedded Systems nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY TU DRESDEN CHAIR FOR EMBEDDED SYSTEMS AND
 CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
_______________________________________________________________________________
*/

#ifndef UART_LIGHT_H_
#define UART_LIGHT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Status Signale
#define UART_LIGHT_RX_EMPTY     (1<<0)
#define UART_LIGHT_RX_FULL      (1<<1)
#define UART_LIGHT_TX_EMPTY     (1<<2)
#define UART_LIGHT_TX_FULL      (1<<3)
#define UART_LIGHT_TX_IRQ_PRE   (1<<4)
#define	UART_LIGHT_TX_IRQ_FLAG	(1<<5)

// Interruptfreigabe fuer UART light
#define UART_LIGHT_RXIE         (1<<9)
#define UART_LIGHT_TXIE         (1<<10)


typedef struct {
    volatile uint18_t   status;		// read/write = Reset Tx Interrupt
    volatile uint18_t   rx_data;	// read = Reset Rx Interrupt
    volatile uint18_t   tx_data;	// write
} uart_light_regs_t;

void uart_light_send (uart_light_regs_t *uart, unsigned char value);
unsigned char uart_light_receive (uart_light_regs_t *uart);
int uart_light_receive_nb (uart_light_regs_t *uart, unsigned char *value);

#define declare_UART_LIGHT_FILE(uart) { \
	.base_addr = (void*) uart, \
	.send_byte = (fun_stdio_send_byte) uart_light_send, \
	.receive_byte = (fun_stdio_receive_byte) uart_light_receive, \
	.receive_byte_nb = (fun_stdio_receive_byte_nb) uart_light_receive_nb \
}

void __attribute__((error("stdio_uart_light_open is no longer supported. Declare a global variable FILE * stdout = &UART_LIGHT_*_FILE instead"))) stdio_uart_light_open(uart_light_regs_t * uart);

#ifdef __cplusplus
}
#endif

#endif /*UART_LIGHT_H_*/
