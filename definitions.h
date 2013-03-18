#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

// Numeric constants
#define DEBOUNCE_CNT      60
#define TIMER_DHT         50000
#define PWM_PERIOD        50
#define STR_BUF_SZ        32
#define DHT_SIG_BUF_SZ    42
#define DHT_DATA_BUF_SZ   5
#define DHT_ONE_MIN_DUR   100
#define DHT_ONE_MAX_DUR   120

// Connections
#define SNSR              BIT5
#define RED_LED           BIT0
#define GREEN_LED         BIT6
#define PWM_FAN_DC        BIT2
#define BTN               BIT3

// UART
#define RXD               BIT1
#define TXD               BIT2

// PWM Control
#define PWM_CTL_T         unsigned
#define PWM_STOP          (0x0000)
#define PWM_START         (0x0001)
#define PWM_SPEED_UP      (0x0002)
#define PWM_SPEED_DOWN    (0x0003)

#endif /* DEFINITIONS_H_ */
