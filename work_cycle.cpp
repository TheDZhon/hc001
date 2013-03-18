#include "work_cycle.h"
#include "definitions.h"

#include "msp430g2553.h"

#include  <string.h>
#include  <stdint.h>
#include  <stdio.h>

#define PWM_STEP (PWM_PERIOD / 5)

void initClocks ();
void initPins ();
void initSPI ();

void wcycle_init ()
{
	initClocks ();
	initPins ();
	initUART ();
}

int wcycle_send (char * text) {
    int l = strlen (text);
    int i = 0;
	for (i = 0; i < l; ++i) {
        while (!(IFG2 & UCA0TXIFG))
            ; // wait for snd bufs ready
        UCA0TXBUF = text[i]; // send next symbol
    }
	return l;
}

int wcycle_pwm_ctl (PWM_CTL_T t)
{
	switch (t) {
		case PWM_STOP: TA1CCR1 = 0; break;
		case PWM_START: TA1CCR1 = PWM_PERIOD; break;
		case PWM_SPEED_UP: if ((TA1CCR1 + PWM_STEP) > PWM_PERIOD) { return -2; } TA1CCR1 += PWM_STEP; break;
		case PWM_SPEED_DOWN: if (TA1CCR1 < PWM_STEP) { return -3; } TA1CCR1 -= PWM_STEP; break;
		default: return -1;
	}

	return 0;
}

void initPins () {
    P1DIR |= RED_LED;
    P1OUT &= ~RED_LED;

    P1DIR |= GREEN_LED;
    P1OUT |= GREEN_LED;

    P2DIR |= PWM_FAN_DC;
    P2SEL |= PWM_FAN_DC;

    TA1CCR0 = PWM_PERIOD;
    TA1CCR1 = 0;

    TA1CCTL1 = OUTMOD_7;
    TA1CTL = TASSEL_1 + MC_1;

    P1DIR &= ~BTN;
    P1OUT |= BTN;
    P1REN |= BTN;
    P1IFG &= ~BTN;
    P1IES |= BTN;
    P1IE |= BTN;
}

void initSPI () {
	P1SEL2 |= (BIT1 + BIT2 + BIT4);
	P1SEL |= (BIT1 + BIT2 + BIT4);

	UCA0CTL1 |= UCSWRST;
	UCA0CTL0 = UCMSB + UCMST + UCMODE_0 + UCSYNC;
	UCA0CTL1 = UCSSEL_2 + UCSWRST;
	UCA0BR0 = 200; //5kbit/s
	UCA0CTL1 &= ~UCSWRST;
}

void initClocks () {
    BCSCTL2 = SELM_0 + DIVM_0 + DIVS_0;
    if (CALBC1_1MHZ != 0xFF) {
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_1MHZ;
        DCOCTL = CALDCO_1MHZ;
    }
    BCSCTL1 |= XT2OFF + DIVA_0;
    BCSCTL3 = XT2S_0 + LFXT1S_2 + XCAP_1;
}
