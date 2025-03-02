#include "peripherals/gpio.h"

#define AUX_ENABLES     ((volatile unsigned int *)(PBASE + 0x00215004)) // Auxiliary enables
#define AUX_MU_IO_REG   ((volatile unsigned int *)(PBASE + 0x00215040)) // Mini Uart I/O Data
#define AUX_MU_IER_REG  ((volatile unsigned int *)(PBASE + 0x00215044)) // Mini Uart Interrupt Enable
#define AUX_MU_IIR_REG  ((volatile unsigned int *)(PBASE + 0x00215048)) // Mini Uart Interrupt Identify
#define AUX_MU_LCR_REG  ((volatile unsigned int *)(PBASE + 0x0021504C)) // Mini Uart Line Control
#define AUX_MU_MCR_REG  ((volatile unsigned int *)(PBASE + 0x00215050)) // Mini Uart Modem Control
#define AUX_MU_LSR_REG  ((volatile unsigned int *)(PBASE + 0x00215054)) // Mini Uart Line Status
#define AUX_MU_MSR_REG  ((volatile unsigned int *)(PBASE + 0x00215058)) // Mini Uart Modem Status
#define AUX_MU_SCRATCH  ((volatile unsigned int *)(PBASE + 0x0021505C)) // Mini Uart Scratch
#define AUX_MU_CNTL_REG ((volatile unsigned int *)(PBASE + 0x00215060)) // Mini Uart Extra Control
#define AUX_MU_STAT_REG ((volatile unsigned int *)(PBASE + 0x00215064)) // Mini Uart Extra Status
#define AUX_MU_BAUD_REG ((volatile unsigned int *)(PBASE + 0x00215068)) // Mini Uart Baudrate

void uart_init();
char uart_recv();
void uart_send(char c);
void uart_puts(const char* str);
void uart_hex(unsigned int d);
