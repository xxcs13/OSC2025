#include "utils.h"

// Convert a hexadecimal string to a decimal integer
int h2d(const char *hex) {
    int result = 0;
    
    for(int i = 0; i < 8; i++) {
        char c = hex[i];
        if (c >= '0' && c <= '9') {
            result = (result << 4) | (c - '0');
        } else if (c >= 'a' && c <= 'f') {
            result = (result << 4) | (c - 'a' + 10);
        } else if (c >= 'A' && c <= 'F') {
            result = (result << 4) | (c - 'A' + 10);
        }
    }
    
    return result;
}

// Compare two strings
int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

// Align a number to the specified alignment
unsigned long align(unsigned long num, unsigned long alignment) {
    return (num + alignment - 1) & ~(alignment - 1);
}

// Read a string from UART input
void input_string(char *input_buffer) {
    int i = 0;
    while (1) {
        char c = uart_recv();
        if (c == '\n' || c == '\r') {
            break;
        }
        uart_sendc(c);
        if (i < MAX_INPUT_LEN - 1) {
            input_buffer[i++] = c;
        }
    }
    input_buffer[i] = '\0';
}

// Read an integer from UART input
unsigned int input_int(char *input_buffer) {
    input_string(input_buffer);
    return atoi(input_buffer);
}

// Convert a string to an integer
unsigned int atoi(const char *str) {
    unsigned int result = 0;
    while (*str) {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

// Calculate the length of a string
unsigned int strglen(const char *str) {
    unsigned int len = 0;
    while (*str++) {
        len++;
    }
    return len;
}
