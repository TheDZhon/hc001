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
void handle_PWM ();

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;

    wcycle_init ();

    __bis_SR_register (GIE);

    while (true) {
        handle_dht22 ();
        handle_PWM ();
	}
}

void handle_dht22 () {
	if (has_new_dht_data) {
		has_new_dht_data = false;

		int dht22_status = dht22.decode();
		if (dht22_status == SUCCESS_STATUS) {
			dht22.humidity(&hint, &hdec);
			dht22.temperature(&tint, &tdec);
			sprintf (sendbuf, "[DHT] H:%d.%d%  T:%d.%d\n", hint, hdec, tint, tdec);
			wcycle_send (sendbuf);
		} else {
			wcycle_send ("DHT error");
		}
	}
}

void handle_PWM () {
	if (has_new_rx_data_uart) {
		has_new_rx_data_uart = false;

		if (readbuf == 'd') {
			wcycle_dht_ctl (DHT_START);
			return;
		}

		sprintf (sendbuf, "[FAN]:%d\n", readbuf);

		wcycle_send (sendbuf);

		wcycle_pwm_ctl (readbuf);
	}
}

