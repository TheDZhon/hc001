#include "work_cycle.h"
#include "definitions.h"

#include  <string.h>
#include  <stdint.h>
#include  <stdio.h>

#define PWM_STEP (PWM_PERIOD / 10)

void initClocks ();
void initPins ();
void initDHT ();
void initUART ();
void initPWM ();

void wcycle_init ()
{
	initPins ();
	initClocks ();
	initUART ();
	initPWM ();
	initDHT ();
}

void wcycle_send (const char * str)
{
	while (*str) {
		while (!(IFG2&UCA0TXIFG))
			;              // USCI_A0 TX buffer ready?
		UCA0TXBUF = *str++;
	}
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
    P1DIR |= (RED_LED + GREEN_LED);
    P1OUT &= ~(RED_LED + GREEN_LED);

    P2DIR |= PWM_FAN_DC;
    P2SEL |= PWM_FAN_DC;

    P2DIR |= SNSR;
    P2OUT |= SNSR;

	P1SEL2 = BIT1 + BIT2;
	P1SEL = BIT1 + BIT2;
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
	TA0CCR0 = TIMER_DHT;
	TA0CCTL0 |= CCIE;
	TA0CTL = TASSEL_2 + MC_1;
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
