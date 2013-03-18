#include  <msp430g2553.h>

#include  <stdio.h>

#include "definitions.h"
#include "dht_lib.h"
#include "work_cycle.h"

DHT dht22;
char strbuf[STR_BUF_SZ];
int debounce_counter = 0;

int main(void) {
    uint16_t hint, hdec, tint, tdec;

	WDTCTL = WDTPW + WDTHOLD;

    wcycle_init ();

    while (true) {
        __bis_SR_register(LPM0 + GIE);

        int dht22_status = dht22.decode();

        sprintf (strbuf, "=> [%d] ", dht22_status);
        wcycle_send (strbuf);

        dht22.humidity(&hint, &hdec);
        dht22.temperature(&tint, &tdec);

        sprintf (strbuf, "H:%d,%d%%, T:%d,%dC\r\n", hint, hdec, tint, tdec);
        wcycle_send (strbuf);

        P1OUT &= ~RED_LED;

        debounce_counter = 0;

        P1IFG |= BTN;
        P1IE |= BTN;
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void) {
    if (debounce_counter == (DEBOUNCE_CNT - 2)) {
        TA0CTL = TACLR;
        P2OUT &= ~SNSR;

        TA0CCR0 = 20000;
        TA0CTL = TASSEL_2 + MC_1;
    }

    if (debounce_counter == (DEBOUNCE_CNT - 1)) {
    	TA0CCR0 = 30;
    	TA0CTL = TASSEL_2 + MC_1;
    }

    if (debounce_counter == DEBOUNCE_CNT) {
        TA0CTL = TACLR;
        TA0CCTL0 &= ~CCIE;

        P2DIR &= ~SNSR;
        P2IFG &= ~SNSR;
        P2IES |= SNSR;
        P2IE |= SNSR;

        TA0CTL = TASSEL_2 + MC_2 + TAIE;
    }

    debounce_counter++;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1(void) {
    switch (TA0IV) {
		case TA0IV_TAIFG:
			P2IE &= ~SNSR;
			TA0CTL = TACLR;

	        P2DIR |= SNSR;
	        P2OUT |= SNSR;

		    TA0CCR0 = TIMER_DHT;
		    TA0CCTL0 |= CCIE;
		    TA0CTL = TASSEL_2 + MC_1;

			__bic_SR_register_on_exit (CPUOFF);
			break;
		default:
			break;
    }

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void) {
		P1IFG &= ~BTN;
		P1IE &= ~BTN;

		P1OUT &= ~RED_LED;
		P1OUT &= ~GREEN_LED;

        P2DIR |= SNSR;
        P2OUT |= SNSR;

		TA0CCR0 = TIMER_DHT;
		TA0CCTL0 |= CCIE;
		TA0CTL = TASSEL_2 + MC_1;
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2 (void) {
    P2IFG &= ~SNSR;

    dht22.handle_timer(TA0R);

    TA0CTL = TACLR;
    TA0CTL = TASSEL_2 + MC_2 + TAIE;

    P1OUT ^= RED_LED;
}
