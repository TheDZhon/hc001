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

#include "work_cycle.h"
#include "definitions.h"

#include  <string.h>
#include  <stdint.h>
#include  <stdio.h>

#define PWM_STEP (PWM_PERIOD / 100)

extern uint16_t last_succ_speed;

void initClocks ();
void initFlash ();
void initPins ();
void initUART ();
void initPWM ();
void initDHT ();

char readFlash ();
void writeFlash (char val);

void wcycle_init ()
{
	initPins ();
	initClocks ();
	initFlash ();
	initUART ();
	initPWM ();
	initDHT ();

	wcycle_pwm_ctl (readFlash());
}

void wcycle_send (const char * str)
{
	while (*str) {
		while (!(IFG2&UCA0TXIFG))
			;              // USCI_A0 TX buffer ready?
		UCA0TXBUF = *str++;
	}
}

void reset_timer ()
{
	TA0CTL = TACLR;
}

void wcycle_dht_read ()
{
	reset_timer ();

	TA0CCR0 = DHT_TIMER_VAL;  // Set timer value
	TA0CCTL0 |= CCIE;         // Allow int0
	TA0CTL = TASSEL_2 + MC_1; // SMCLK and UP
}

void wcycle_pwm_ctl (unsigned char t)
{
	if (t > 100) { t = 100; }

	if (t == TA1CCR1) { return; }

	writeFlash (t);
	TA1CCR1 = unsigned (t) * PWM_STEP;

	last_succ_speed = t;
}

void initPins () {
    P1DIR |= (RED_LED + GREEN_LED);
    P1OUT &= ~(RED_LED + GREEN_LED);

    P2DIR |= PWM_FAN_DC;
    P2SEL |= PWM_FAN_DC;

	P1SEL2 |= (UART_RX + UART_TX);
	P1SEL |= (UART_RX + UART_TX);
}

void initUART () {
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 = UCSSEL_2 + UCSWRST;
    UCA0MCTL = UCBRF_0 + UCBRS_6;
    UCA0BR0 = 130;
    UCA0BR1 = 6;
    UCA0CTL1 &= ~UCSWRST;

    IFG2 &= ~UCA0RXIFG;
    IE2 |= UCA0RXIE;
}

void initPWM () {
    TA1CCR0 = PWM_PERIOD;
    TA1CCR1 = 0;
    TA1CCTL1 = OUTMOD_7;
    TA1CTL = TASSEL_1 + MC_1;
}

void initDHT () {
	TA0CTL = TACLR;

	P2DIR |= SNSR;
	P2OUT |= SNSR;
}

void initClocks () {
    BCSCTL2 = SELM_0 + DIVM_0 + DIVS_0;
    if (CALBC1_16MHZ != 0xFF) {
        __delay_cycles(100000);
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_16MHZ;     /* Set DCO to 16MHz */
        DCOCTL = CALDCO_16MHZ;
    }
    BCSCTL1 |= XT2OFF + DIVA_0;
    BCSCTL3 = XT2S_0 + LFXT1S_2 + XCAP_1;
}

void initFlash () {
	FCTL2 = FWKEY + FSSEL0 + (FN0 + FN1 + FN2 + FN3 + FN5);             // MCLK/48 for Flash Timing Generator
}

char readFlash () {
	const char * flash_ptr = (const char*)FLASH_OFFSET;
	return *flash_ptr;
}

void writeFlash (char v) {
	_DINT();

	FCTL1 = FWKEY + ERASE; // ALLOW RESET
	FCTL3 = FWKEY; // CLEAR LOCK

	char * flash_ptr = (char*)FLASH_OFFSET;

	*flash_ptr = 0; // RESET

	FCTL1 = FWKEY + WRT; // SET WRITE

	*flash_ptr = v;

	FCTL1 = FWKEY; // CLAR WRITE
	FCTL3 = FWKEY + LOCK; // SET LOCK

	_EINT();
}
