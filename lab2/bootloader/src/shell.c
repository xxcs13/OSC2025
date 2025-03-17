#include "shell.h"

char buffer[MAX_INPUT_LEN];

int main() {
    uart_init();
    while (1) { 
        int i = 0;
        uart_puts("\r\n# ");

        while (1) {
            char c = uart_recv();
            if (c == '\n' || c == '\r') {
                break;
            }
            uart_sendc(c);
            if (i < MAX_INPUT_LEN - 1) {
                buffer[i++] = c;
            }
        }

        buffer[i] = '\0';
        
        if (strcmp(buffer, "help") == 0) {
            uart_puts("\r\nhelp        : print this help menu\n\r");
            uart_puts("load        : load kernel\n\r");
            uart_puts("reboot      : reboot system\n\r");
        }else if(strcmp(buffer, "load")==0){
            load_kernel();
        }else if(strcmp(buffer, "reboot") == 0) {
            uart_puts("\n\rReboot\n\r");
            reset(100);
        }else {
            uart_puts("\n\rUnknown command\r");
        }
    }
}
