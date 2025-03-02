#include "peripherals/mailbox.h"


int mailbox_call(unsigned int *mailbox) {
    unsigned int r = (((unsigned int)((unsigned long)mailbox) & ~0xF) | (8 & 0xF)); // Combine address with channel number
    
    // Wait until we can write to the mailbox
    while (*MAILBOX_STATUS & MAILBOX_FULL) {
        // do nothing
    }

    // Write the address of our message to the mailbox with channel identifier
    *MAILBOX_WRITE = r;

    // Wait for the response
    while (1) {
        // Wait until we can read from the mailbox
        while (*MAILBOX_STATUS & MAILBOX_EMPTY) {
            // do nothing
        }

        // Check if the response is for us
        if (r == *MAILBOX_READ) {
            return mailbox[1] == REQUEST_SUCCEED;
        }
    }
}

void get_board_revision(){
    unsigned int mailbox[7];
    mailbox[0] = 7 * 4; // buffer size in bytes
    mailbox[1] = REQUEST_CODE;
    // tags begin
    mailbox[2] = GET_BOARD_REVISION; // tag identifier
    mailbox[3] = 4; // maximum of request and response value buffer's length.
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer
    // tags end
    mailbox[6] = END_TAG;
  
    mailbox_call(mailbox); // message passing procedure call
  
    uart_puts("Board revision: ");
    uart_hex(mailbox[5]);
    uart_puts("\r\n"); 
}

void get_ARM_memory(){
    unsigned int mailbox[8];
    mailbox[0] = 8 * 4; // buffer size in bytes
    mailbox[1] = REQUEST_CODE;
    // tags begin
    mailbox[2] = GET_ARM_MEMORY; // tag identifier
    mailbox[3] = 8; // maximum of request and response value buffer's length.
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer
    mailbox[6] = 0; // value buffer
    // tags end
    mailbox[7] = END_TAG;
  
    mailbox_call(mailbox); // message passing procedure call
  
    uart_puts("ARM memory base address: ");
    uart_hex(mailbox[5]);
    uart_puts("\r\n"); 
    uart_puts("ARM memory size: ");
    uart_hex(mailbox[6]);
    uart_puts("\r\n"); 
}