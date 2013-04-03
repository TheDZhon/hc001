/**
 ** Main.
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

#include  <stdio.h>

#include "definitions.h"
#include "dht_lib.h"
#include "work_cycle.h"

DHT dht22;

char sendbuf[STR_BUF_SZ];
char readbuf;

bool has_new_rx_data_uart = false;
bool has_new_dht_data = false;

uint16_t hint, hdec, tint, tdec;

// Read sensor data and send.
void handle_dht22 ();
// Handle received control data.
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
		if (dht22_status == DHT_SUCCESS_STATUS) {
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

