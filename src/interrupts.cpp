/**
 ** Work cycle helpers and auxiliaries.
 **
 ** Copyright (c) 2013, Eugene Mamin
 ** All rights reserved.
 ** Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are met:
 **
 ** 1. Redistributions of source code must retain the above copyright notice, this
 ** list of conditions and the following disclaimer.
 ** 2. Redistributions in binary form must reproduce the above copyright notice,
 ** this list of conditions and the following disclaimer in the documentation
 ** and/or other materials provided with the distribution.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 ** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ** ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 ** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 ** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 ** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ** The views and conclusions contained in the software and documentation are those
 ** of the authors and should not be interpreted as representing official policies,
 ** either expressed or implied, of the FreeBSD Project
 **/

#include "definitions.h"
#include "dht_lib.h"

extern DHT dht22;

extern char sensorsdatabuf[STR_BUF_SZ];
extern char speedbuf;

extern bool has_new_rx_data_uart;
extern bool has_new_dht_data;

int interrupt_cnt_int0 = 0;

/**
 ** int0
 ** Timer0 A0 interruption for first stage of DHT22 hand-shake.
 **/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_IH (void) {
	++interrupt_cnt_int0;

	if (interrupt_cnt_int0 == DHT_TIMER_CNT_STANDBY)
	{
		TA0CTL = TACLR;           // Clear timer counter

		P2DIR |= SNSR;            // DHT contact pin as OUT
		P2OUT &= ~SNSR;           // DHT contact pin LOW

		TA0CCR0 = DHT_TIMER_VAL;  // Restart timer counter
		TA0CTL = TASSEL_2 + MC_1; // SMCLK, UP
	}

	if (interrupt_cnt_int0 == DHT_TIMER_CNT_INIT) {
		interrupt_cnt_int0 = 0;   // Reset interrupts counter

		TA0CTL = TACLR;           // Clear timer
		TA0CCTL0 &= ~CCIE;        // Prevent int0

		P2DIR &= ~SNSR;           // DHT contact pin as IN
		P2IFG &= ~SNSR;           // DHT contact pin clear interrupts
		P2IES |= SNSR;            // DHT contact pin enable interrupts HIGH->LOW
		P2IE |= SNSR;             // DHT contact pin enable interrupts

		TA0CTL = TASSEL_2
				+ MC_2 + TAIE;    // Enable int1
	}
}

/**
 ** int1
 ** Timer A1 interruption for second stage of DHT22 hand-shake.
 **/
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_IH (void) {
    switch (TA0IV) {
		case TA0IV_TAIFG:
			P2IE &= ~SNSR;    // DHT contact pin disable interrupts
			TA0CTL = TACLR;   // Clear timer

			has_new_dht_data = true;

			//__bic_SR_register_on_exit (LPM0);

		    break;
		default:
			break;
    }
}

/**
 ** Sensor data pin interruption for values reading.
 **/
#pragma vector=PORT2_VECTOR
__interrupt void Port_2_IH (void) {
    P2IFG &= ~SNSR;                   // Clear interrupt flag

    dht22.handle_timer(TA0R);         // Analyze current data bit

    TA0CTL = TACLR;                   // Clear timer
    TA0CTL = TASSEL_2 + MC_2 + TAIE;  // Restart timer

    P1OUT ^= RED_LED;
}

/**
 ** TI USCI (UART) RX interruptions handler.
 **/
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR_HOOK (void) {
	speedbuf = UCA0RXBUF;

	if (speedbuf != UART_TERM_SYMB) {
		IFG2 &= ~UCA0RXIFG;

		P1DIR |= BIT0;
		P1OUT ^= BIT0;

		has_new_rx_data_uart = true;
	}
}

