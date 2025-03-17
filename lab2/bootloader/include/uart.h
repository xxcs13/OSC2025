#ifndef UART_H
#define UART_H


#define PERIPHERAL_BASE                       0x3F000000
#define AUX_BASE           (PERIPHERAL_BASE + 0x215000)
#define GPIO_BASE          (PERIPHERAL_BASE + 0x200000)


#define GPFSEL1            ((volatile unsigned int*)(GPIO_BASE + 0x04))
#define GPPUD              ((volatile unsigned int*)(GPIO_BASE + 0x94))
#define GPPUDCLK0          ((volatile unsigned int*)(GPIO_BASE + 0x98))


#define AUX_ENABLES        ((volatile unsigned int*)(AUX_BASE + 0x04))
#define AUX_MU_IO          ((volatile unsigned int*)(AUX_BASE + 0x40))
#define AUX_MU_IER         ((volatile unsigned int*)(AUX_BASE + 0x44))
#define AUX_MU_IIR         ((volatile unsigned int*)(AUX_BASE + 0x48))
#define AUX_MU_LCR         ((volatile unsigned int*)(AUX_BASE + 0x4C))
#define AUX_MU_MCR         ((volatile unsigned int*)(AUX_BASE + 0x50))
#define AUX_MU_LSR         ((volatile unsigned int*)(AUX_BASE + 0x54))
#define AUX_MU_MSR         ((volatile unsigned int*)(AUX_BASE + 0x58))
#define AUX_MU_SCRATCH     ((volatile unsigned int*)(AUX_BASE + 0x5C))
#define AUX_MU_CNTL        ((volatile unsigned int*)(AUX_BASE + 0x60))
#define AUX_MU_STAT        ((volatile unsigned int*)(AUX_BASE + 0x64))
#define AUX_MU_BAUD        ((volatile unsigned int*)(AUX_BASE + 0x68))

void uart_init(void);
void uart_sendc(char c);
char uart_recv(void);
void uart_puts(const char* s);
void uart_send_hex(unsigned int hex);
void uart_send_int(int num);
int uart_read_int();
int uart_is_readable();
#endif