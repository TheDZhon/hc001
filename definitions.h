#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include "msp430g2553.h" // MCU-specific header

// Numeric constants
#define PWM_PERIOD        800
#define STR_BUF_SZ        32

#define DHT_TIMER_VAL     65535
#define DHT_TIMER_CNT     5
#define DHT_SIG_BUF_SZ    42
#define DHT_DATA_BUF_SZ   5
#define DHT_ZERO_MAX_DUR  1500
#define DHT_ONE_MAX_DUR   2000

#define SUCCESS_STATUS    0

// Connections
#define SNSR              BIT5
#define RED_LED           BIT0
#define GREEN_LED         BIT6
#define PWM_FAN_DC        BIT2
#define BTN               BIT3

// UART
#define UART_RX           BIT1
#define UART_TX           BIT2
#define UART_TERM_SYMB    '\n'

// DHT22 Control
#define DHT_CTL_T         char
#define DHT_STOP          'H'
#define DHT_START         'R'

// PWM Control
#define PWM_CTL_T         char
#define PWM_STOP          'A'
#define PWM_START         'D'
#define PWM_SPEED_UP      'W'
#define PWM_SPEED_DOWN    'S'

#endif /* DEFINITIONS_H_ */
