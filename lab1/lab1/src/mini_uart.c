#include "peripherals/mini_uart.h"
#include "peripherals/gpio.h"

// Function to initialize the mini UART
void uart_init() {
    // Configure GPIO pins 14 and 15 to ALT5 for mini UART
    unsigned int selector;  //p.92,p102
    selector = *GPFSEL1;
    selector &= ~(7 << 12); // Clean GPIO 14
    selector |= 2 << 12;    // Set ALT5 for GPIO 14
    selector &= ~(7 << 15); // Clean GPIO 15
    selector |= 2 << 15;    // Set ALT5 for GPIO 15
    *GPFSEL1 = selector;

    // Disable GPIO pull up/down
    *GPPUD = 0; //p.101
    for (int i = 0; i < 150; i++) { asm volatile("nop"); } // Wait 150 cycles
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    for (int i = 0; i < 150; i++) { asm volatile("nop"); } // Wait 150 cycles
    *GPPUDCLK0 = 0;

    // Enable mini UART
    *AUX_ENABLES        |= 1;
    *AUX_MU_CNTL_REG     = 0;       // Disable transmitter and receiver during configuration
    *AUX_MU_IER_REG      = 0;       // Disable interrupt
    *AUX_MU_LCR_REG      = 3;       // Set the data size to 8 bit(p.14)
    *AUX_MU_MCR_REG      = 0;       // Don't need auto flow control
    *AUX_MU_BAUD_REG     = 270;     // Set baud rate to 115200
    *AUX_MU_IIR_REG      = 6;       // Clear FIFO and enable FIFO(p.13)
    *AUX_MU_CNTL_REG     = 3;       // Enable the transmitter and receiver
}

// Function to receive a character from the UART
char uart_recv() {
    char tmp;
    while (!(*AUX_MU_LSR_REG & 0x01)) {
        asm volatile("nop");
        // Wait until data is ready
    }
    tmp = (char)(*AUX_MU_IO_REG);

    return tmp == '\r' ? '\n' : tmp;
}

// Function to send a character to the UART
void uart_send(char c) {
    while (!(*AUX_MU_LSR_REG & 0x20)) {
        asm volatile("nop");
        // Wait until transmitter is empty
    }
    *AUX_MU_IO_REG = c;
}

// Function to display a string via UART
void uart_puts(const char* str) {
    int i;
    for(i = 0; str[i] != '\0'; i++){
        if(str[i] == '\n')
            uart_send('\r');
        uart_send((char)str[i]);
    }
}

// Function to display a binary value in hexadecimal via UART
void uart_hex(unsigned int d) {
    const char hex_chars[] = "0123456789ABCDEF";
    uart_puts("0x");
    for (int i = 28; i >= 0; i -= 4) {
        uart_send(hex_chars[(d >> i) & 0xF]);
    }
}
