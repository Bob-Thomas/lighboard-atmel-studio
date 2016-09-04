#include "asf.h" //uart.h etc. included here

#define UART_SERIAL_BAUDRATE        9600
#define UART_SERIAL_CHANNEL_MODE   UART_MR_CHMODE_NORMAL
#define UART_SERIAL_MODE         UART_MR_PAR_NO

/* =============== UART1 =============== */ //(UART0 is defined but not UART1)
#define PINS_UART1          (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1)
#define PINS_UART1_FLAGS    (PIO_PERIPH_A | PIO_DEFAULT)
#define PINS_UART1_MASK     (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1)
#define PINS_UART1_PIO      PIOB
#define PINS_UART1_ID       ID_PIOB
#define PINS_UART1_TYPE     PIO_PERIPH_A
#define PINS_UART1_ATTR     PIO_DEFAULT

void uart_custom_init(void);
void sendViaUart(uint8_t data);