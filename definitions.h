#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include "msp430g2553.h" // MCU-specific header

// Numeric constants
#define DEBOUNCE_CNT      60
#define TIMER_DHT         50000
#define PWM_PERIOD        800
#define STR_BUF_SZ        20
#define DHT_SIG_BUF_SZ    42
#define DHT_DATA_BUF_SZ   5
#define DHT_ONE_MIN_DUR   1600
#define DHT_ONE_MAX_DUR   1920

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

// PWM Control
#define PWM_CTL_T         char
#define PWM_STOP          (0x00)
#define PWM_START         (0x01)
#define PWM_SPEED_UP      (0x02)
#define PWM_SPEED_DOWN    (0x03)

#endif /* DEFINITIONS_H_ */
