#include "definitions.h"
#include "dht_lib.h"

extern DHT dht22;

extern char sendbuf[STR_BUF_SZ];
extern char readbuf;

extern bool has_new_rx_data_uart;
extern bool has_new_dht_data;

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_IH (void) {
	static int interrupt_cnt = 0;

	if (interrupt_cnt < DHT_TIMER_CNT) {
		++interrupt_cnt;

		return;
	}

    if (interrupt_cnt == DHT_TIMER_CNT) {
    	interrupt_cnt = 0;

        TA0CTL = TACLR;
        TA0CCTL0 &= ~CCIE;

        P2DIR &= ~SNSR;
        P2IFG &= ~SNSR;
        P2IES |= SNSR;
        P2IE |= SNSR;

        TA0CCR0 = DHT_TIMER_VAL;
        TA0CTL = TASSEL_2 + MC_2 + TAIE;
    }
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_IH (void) {
	static int interrupt_cnt = 0;

	if (interrupt_cnt < DHT_TIMER_CNT) {
		++interrupt_cnt;
		return;
	}

	interrupt_cnt = 0;

    switch (TA0IV) {
		case TA0IV_TAIFG:
			P2IE &= ~SNSR;
			TA0CTL = TACLR;

		    has_new_dht_data = true;

			//__bic_SR_register_on_exit (LPM0);
			break;
		default:
			break;
    }
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2_IH (void) {
    P2IFG &= ~SNSR;

    dht22.handle_timer(TA0R);

    TA0CTL = TACLR;
    TA0CTL = TASSEL_2 + MC_2 + TAIE;

    P1OUT ^= RED_LED;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR_HOOK (void) {
	readbuf = UCA0RXBUF;

	if (readbuf != '\n') {
		IFG2 &= ~UCA0RXIFG;

		P1DIR |= BIT0;
		P1OUT ^= BIT0;

		has_new_rx_data_uart = true;
	}
}

