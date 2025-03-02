#include "peripherals/mailbox.h"
#include "peripherals/shell.h"

void help() {
    uart_puts("help        : print this help menu\r\n");
    uart_puts("hello       : print Hello World!\r\n");
    uart_puts("mailbox     : print hardware's information\r\n");
}

void hello() {
    uart_puts("Hello World!\r\n");
}

void mailbox() {
    uart_puts("Mailbox info:\r\n");
    get_board_revision();
    get_ARM_memory();
}


Command commands[] = {
    {"help", help},
    {"hello", hello},
    {"mailbox", mailbox},
};

#define NUM_COMMANDS (sizeof(commands) / sizeof(commands[0]))

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

void read_command(char *buffer) {
    int index = 0;
    char c;
    
    // Clear the buffer
    buffer[0] = '\0';
    
    while (1) {
        c = uart_recv();
        if (c == 26) { // ASCII code for Ctrl+Z
            uart_puts("\r\n");
            buffer[0] = '\0';
            return;
        }
        uart_send(c);   // Echo the character back to the user
        
        if (c == '\r' || c == '\n') {
            // End of command
            //uart_puts("\r\n");
            buffer[index] = '\0';
            break;
        }
        
        // Add character to buffer
        buffer[index++] = c;
    }
}

void process_command(char *buffer) {
    int i;
    int found = 0;
    
    // Skip processing if buffer is empty
    if (buffer[0] == '\0') {
        return;
    }
    
    // Check if command exists
    for (i = 0; i < NUM_COMMANDS; i++) {
        if (strcmp(buffer, commands[i].command) == 0) {
            commands[i].function();
            found = 1;
            break;
        }
    }
    
    // Handle unknown command
    if (!found) {
        uart_puts("Unknown command: ");
        uart_puts(buffer);
        uart_puts("\r\n");
        uart_puts("Type 'help' for available commands\r\n");
    }
}

void main() {
    char buffer[256];
    
    uart_init();
    uart_puts("\r\nWelcome to OSC2025 Lab1!\r\n");
    
    while (1) {
        uart_puts("# ");
        read_command(buffer);
        process_command(buffer);
    }
}