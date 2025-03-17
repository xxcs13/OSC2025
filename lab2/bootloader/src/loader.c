#include "loader.h"

unsigned int size = 0;
unsigned char header[8];

void load_kernel() {
        
    for(int i = 0; i < 4; i++) {
        header[i] = uart_recv();
    }
    size = *(unsigned int*)&header[0];
    
    uart_puts("\n\rKernel size: ");
    uart_send_int(size);
    uart_puts(" bytes\n");

    receive_kernel(size);
    
    uart_puts("\n\rDone\r");
    jump_to_kernel();
}

void jump_to_kernel() {
    typedef void (*kernel_func)(uint64_t);
    kernel_func kernel_entry = (kernel_func)KERNEL_ADDR;
    kernel_entry((unsigned long)dtb_addr);
}

void receive_kernel(int size) {
    char *kernel = (char *)KERNEL_ADDR; 
    int count = 0;
    const int TIMEOUT_MAX = 20000000;   // 6s
    
    unsigned long saved_dtb_addr = dtb_addr;                    // Save the current dtb_addr value for recovery after receiving the kernel image.
    for(unsigned int i=0; i < size; i++) {
        count = 0;
        while(!uart_is_readable() && count < TIMEOUT_MAX) {
            count++;
        }
        if(count >= TIMEOUT_MAX) {
            return;
        }
        kernel[i] = uart_recv();
        if((i % 1024) == 0) {
            uart_puts("."); 
        }
    }
    dtb_addr = saved_dtb_addr;
}

