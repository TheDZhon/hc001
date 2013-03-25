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
#define DHT_STOP          (0x00)
#define DHT_START         (0x01)

// PWM Control
#define PWM_CTL_T         char
#define PWM_STOP          (0x00)
#define PWM_START         (0x01)
#define PWM_SPEED_UP      (0x02)
#define PWM_SPEED_DOWN    (0x03)

#endif /* DEFINITIONS_H_ */
