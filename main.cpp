#include  <stdio.h>

#include <msp430g2553.h>

#include "definitions.h"
#include "dht_lib.h"
#include "work_cycle.h"

DHT dht22;

char sendbuf[STR_BUF_SZ];
char readbuf;

bool has_new_rx_data_uart = false;
bool has_new_dht_data = false;

uint16_t hint, hdec, tint, tdec;

void handle_dht22 ();
void handle_UART ();

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;

    wcycle_init ();

    __bis_SR_register (GIE);

    while (true) {
        handle_dht22 ();
        handle_UART ();
	}
}

void handle_dht22 () {
	if (has_new_dht_data) {
		has_new_dht_data = false;

		int dht22_status = dht22.decode();
		if (dht22_status == SUCCESS_STATUS) {
			dht22.humidity(&hint, &hdec);
			dht22.temperature(&tint, &tdec);
			sprintf (sendbuf, "[DHT] H:%d.%d%  T:%d.%d\n\r", hint, hdec, tint, tdec);
		} else {
			sprintf (sendbuf, "[DHT] I/O error:%d\n\r", dht22_status);
		}
		wcycle_send (sendbuf);
	}
}

void handle_UART () {
	if (has_new_rx_data_uart) {
		char rb_copy = readbuf;
		has_new_rx_data_uart = false;

		int dht_status = wcycle_dht_ctl (rb_copy);
		int pwm_status = wcycle_pwm_ctl (rb_copy);
	}
}

