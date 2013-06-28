/**
 ** General project definitions.
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

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include "msp430g2553.h"     // MCU-specific header

// Numeric constants
#define PWM_PERIOD           800
#define STR_BUF_SZ           32

// DHT constants
#define DHT_TIMER_VAL        10000
#define DHT_TIMER_CNT        5000
#define DHT_SIG_BUF_SZ       42
#define DHT_DATA_BUF_SZ      5
#define DHT_ZERO_MAX_DUR     1500
#define DHT_ONE_MAX_DUR      2200

// DHT statuses
#define DHT_SUCCESS_STATUS   0
#define DHT_CRC_ERROR        -1
#define DHT_LINK_ERROR       -2

// Connections
#define SNSR                 BIT5
#define RED_LED              BIT0
#define GREEN_LED            BIT6
#define PWM_FAN_DC           BIT2
#define BTN                  BIT3

// UART
#define UART_RX              BIT1
#define UART_TX              BIT2
#define UART_TERM_SYMB       '|'

#endif /* DEFINITIONS_H_ */
