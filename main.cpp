#include  <msp430g2553.h>
#include  <stdio.h>
#include  <string.h>
#include  <stdint.h>

#include "dht_lib.h"

DHT dht22;

const unsigned kStrBufSz = 32;
char strbuf[kStrBufSz];

// debouncing counter
int debounce_cnt = 0;

void initPins ();
void initUART ();
void initClocks ();

void sendString(char*);

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;

    initPins ();
    initUART ();
    initClocks ();

    uint16_t hint, hdec, tint, tdec;

    while (true) {
        __bis_SR_register(CPUOFF + GIE);

        int dht22_status = dht22.decode();

        sprintf (strbuf, "=> [%d] ", dht22_status);
        sendString (strbuf);

        dht22.humidity(&hint, &hdec);
        dht22.temperature(&tint, &tdec);

        sprintf (strbuf, "H:%d,%d%%, T:%d,%dC\r\n", hint, hdec, tint, tdec);
        sendString (strbuf);

        // disable red LED
        P1OUT &= ~BIT0;

        // reset debounce pause
        debounce_cnt = 0;

        P1IFG |= BIT3;
        P1IE |= BIT3;
    }
}

// Timer0 A0 interrupt handler
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void) {
    // 60 x 50000 = 3s

    if (debounce_cnt == 59) {
        // Clear timer
        TA0CTL = TACLR;

        // Set sensor LOW.
        P2OUT &= ~BIT5;

        // 20ms UP timer start (sensor handshake)
        TA0CCR0 = 20000;
        TA0CTL = TASSEL_2 + MC_1;
    }

    if (debounce_cnt == 60) {
    	// Set sensor HIGH for 30us
    	TA0CCR0 = 30;
    	TA0CTL = TASSEL_2 + MC_1;
    }

    // After 20 ms + 30 us handshake
    if (debounce_cnt == 61) {
        // Clear A0 timer, disable this interruption
        TA0CTL = TACLR;
        TA0CCTL0 &= ~CCIE;

        // Set P2.5 as input, enable interruption
        P2DIR &= ~BIT5;
        P2IFG &= ~BIT5;
        P2IES |= BIT5;
        P2IE |= BIT5;

        // Continuous A1 start for sensor answer or timeout
        TA0CTL = TASSEL_2 + MC_2 + TAIE;
    }

    debounce_cnt++;
}

// Timer0 A1 interrupt handler
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1(void) {
    // Overflow flag checking
    switch (TA0IV) {
		case TA0IV_TAIFG:
			// Disable interruption P2.5
			// Clear timer, enable CPU for UART report
			P2IE &= ~BIT5;
			TA0CTL = TACLR;

	        P2DIR |= BIT5;
	        P2OUT |= BIT5; // Set to high

		    TA0CCR0 = 50000;
		    TA0CCTL0 |= CCIE;
		    TA0CTL = TASSEL_2 + MC_1;

			__bic_SR_register_on_exit (CPUOFF);
			break;
		default:
			break;
    }

}

// Button P1.3 interrupt handler
#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void) {
		// disable interruption and clear flag
		P1IFG &= ~BIT3;
		P1IE &= ~BIT3;

		// disable LEDs
		P1OUT &= ~BIT0;
		P1OUT &= ~BIT6;

        P2DIR |= BIT5;
        P2OUT |= BIT5; // Set to high

		// Start timer for UP mode and allow interruption #0
		// limited by 50000
		TA0CCR0 = 50000;
		TA0CCTL0 |= CCIE;
		TA0CTL = TASSEL_2 + MC_1;
}

// DHT sensor interrupt handler
#pragma vector=PORT2_VECTOR
__interrupt void Port_2 (void) {
    // Clear interruption flag
    P2IFG &= ~BIT5;
    // Copy duration to dht22 object
    dht22.handle_timer(TA0R);
    // Clear and restart timer
    TA0CTL = TACLR;
    TA0CTL = TASSEL_2 + MC_2 + TAIE;
    // Blink red LED
    P1OUT ^= BIT0;
}

void initPins () {
    // Light off for activity RED led
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    // Light on for ready GREEN led
    P1DIR |= BIT6;
    P1OUT |= BIT6;

    // Enable P1.3 button as measurements trigger
    P1DIR &= ~BIT3;
    P1OUT |= BIT3;
    P1REN |= BIT3;
    P1IFG &= ~BIT3;
    P1IES |= BIT3;
    P1IE |= BIT3;

    // Enable P1.2 as UART
    P1SEL |= BIT2;
    P1SEL2 |= BIT2;
}

void initUART () {
    // UART init code from GRACE
    // BR 9600
    UCA0CTL1 |= UCSSEL_2;
    UCA0BR0 = 104;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;
    UCA0CTL1 &= ~UCSWRST;
}

void initClocks () {
    // Clocks settings for Calibrated 1MHz
    BCSCTL2 = SELM_0 + DIVM_0 + DIVS_0;
    if (CALBC1_1MHZ != 0xFF) {
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_1MHZ;
        DCOCTL = CALDCO_1MHZ;
    }
    BCSCTL1 |= XT2OFF + DIVA_0;
    BCSCTL3 = XT2S_0 + LFXT1S_2 + XCAP_1;
}

void sendString(char * text) {
    int l = strlen (text);
	for (int i = 0; i < l; ++i) {
        while (!(IFG2 & UCA0TXIFG))
            ; // wait for snd bufs ready
        UCA0TXBUF = text[i]; // send next symbol
    }
}
