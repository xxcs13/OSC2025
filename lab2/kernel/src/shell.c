#include "shell.h"

char input_buffer[MAX_INPUT_LEN];
char filename_buffer[MAX_INPUT_LEN];
char size_str[MAX_INPUT_LEN];

void initramfs_callback(const char *node_name, const char *property_name, const void *property_value)
{
    if(strcmp(property_name,"linux,initrd-start")==0){
        initramfs_start = bswap32(*(unsigned long *)property_value);
        uart_puts("\n\rInitramfs start: ");
        uart_send_hex(initramfs_start);
        return;
    }
}

int main() { 
    uart_puts("\n\rWelcome to NYCU OSC 2025 Lab2!");
    init_allocator();
    fdt_traverse(dtb_addr,initramfs_callback);
    
    while (1) { 
        uart_puts("# ");

        input_string(input_buffer);

        if (strcmp(input_buffer, "help") == 0) {
            uart_puts("\n\r");
            uart_puts("help         :print this help menu\n\r");
            uart_puts("reboot       :reboot system\n\r");
            uart_puts("ls           :list the files \n\r");
            uart_puts("cat          :show the content\n\r");
            uart_puts("memAlloc     :allocate memory\n\r");
        } else if(strcmp(input_buffer, "reboot") == 0) {
            uart_puts("\n\rReboot\n\r");
            reset(100);
        } else if(strcmp(input_buffer, "ls") == 0) {
            cpio_ls(initramfs_start);
        } else if(strcmp(input_buffer, "cat") == 0) {
            uart_puts("\n\rEnter filename: ");
            input_string(filename_buffer);
            uart_puts("\n\r");
            cpio_cat(filename_buffer,initramfs_start);
        } else if(strcmp(input_buffer, "memAlloc") == 0) {
            uart_puts("\n\rEnter size: ");
            unsigned int size = input_int(size_str);
            uart_puts("\n\r");
            void* ptr = simple_alloc(size);
            uart_puts("Allocated memory at: ");
            uart_send_hex64((unsigned long)ptr);
            uart_puts("\n\r");
        } else {
            uart_puts("\n\rUnknown command\n\r");
        }
    }
}
