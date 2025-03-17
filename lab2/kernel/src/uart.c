#include <stdint.h>
#include "uart.h"

void uart_init(void) {
    register unsigned int selector;

    // init GPIO
    selector = *GPFSEL1;
    selector &= ~((7<<12) | (7<<15)); // clean gpio14, gpio15
    selector |= (2<<12) | (2<<15);    // set alt5 for gpio14, gpio15
    *GPFSEL1 = selector;

    // Disable GPIO pull up/down
    *GPPUD = 0; //p.101
    for (int i = 0; i < 150; i++) { asm volatile("nop"); } // Wait 150 cycles
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    for (int i = 0; i < 150; i++) { asm volatile("nop"); } // Wait 150 cycles
    *GPPUDCLK0 = 0;

    // init UART
    *AUX_ENABLES        |= 1;       // enable mini UART
    *AUX_MU_CNTL         = 0;       // Disable transmitter and receiver during configuration
    *AUX_MU_IER          = 0;       // Disable interrupt
    *AUX_MU_LCR          = 3;       // Set the data size to 8 bit(p.14)
    *AUX_MU_MCR          = 0;       // Don't need auto flow control
    *AUX_MU_BAUD         = 270;     // Set baud rate to 115200
    *AUX_MU_IIR          = 6;       // Clear FIFO and enable FIFO(p.13)
    *AUX_MU_CNTL         = 3;       // Enable the transmitter and receiver
}

char uart_recv(void) {
    while(!(*AUX_MU_LSR & 0x01)); // wait until can receive
    char c = (char)(*AUX_MU_IO);
    return c;
}


void uart_sendc(char c) {
    while(!(*AUX_MU_LSR & 0x20)){
        asm volatile("nop");
        // Wait until transmitter is empty
    }
    *AUX_MU_IO = c;
}


void uart_puts(const char* s) {
    while(*s) {
        uart_sendc(*s++);
    }
}

void uart_send_hex(unsigned int hex) {
    char temp[] = "0123456789ABCDEF";
    char output[9];
    output[8] = '\0';
    uart_puts("0x");
    for(int i = 7; i >= 0; i--) {
        output[i] = temp[hex & 0xF];
        hex >>= 4;
    }
    uart_puts(output);
}

void uart_send_hex64(unsigned long hex) {
    uart_puts("0x");
    for(int i = 60; i >= 0; i -= 4) {
        unsigned int digit = (hex >> i) & 0xF;
        if(digit < 10)
            uart_sendc('0' + digit);
        else
            uart_sendc('A' + digit - 10);
    }
}

int uart_read_int(void) {
    int num=0;
    char c;
    for(int i=0;i<4;i++){
        c = uart_recv();            //Uart each time transfer 1 byte(8 bits)
        num=num<<8;
        num+=(int)c;                //c is char, so need to convert to int
    }
    return num;
}

void uart_send_int(int num) {
    char output[12];
    int i = 0;
    if (num == 0) {
        uart_sendc('0');
        return;
    }
    if (num < 0) {
        uart_sendc('-');
        num = -num;
    }
    while (num > 0) {
        output[i++] = num % 10 + '0';
        num /= 10;
    }
    for (int j = i - 1; j >= 0; j--) {
        uart_sendc(output[j]);
    }
}

int uart_is_readable() {
    return (*AUX_MU_LSR & 0x01);
}