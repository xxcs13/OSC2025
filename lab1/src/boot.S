.section ".text.boot"
.globl _start
_start:
    mrs     x0, mpidr_el1        // Move Register from System
    and     x0, x0, #0xFF        // Using AND operation to calculate the value of x0 with 0xFF. Check processor id
    cbz     x0, master           // Compare and Branch on Zer0. If x0 == 0 (indicating this is the master core), then jump to master to continue executing initialization; otherwise, execute the next line b proc_hang and enter an infinite wait
    b       proc_hang           

proc_hang: 
    wfe                          // Wait for event
    b       proc_hang            // Infinite loop, keeping the non-core process in wfe state

master:
    adr     x0, bss_begin        // Store bss_begin (the starting address of the BSS segment) into x0
    adr     x1, bss_end
    sub     x1, x1, x0           // Calculate the size of the BSS section (bss_end - bss_begin) and store the result in x1.x1 = x1 - x0
    bl      memzero              // bl (branch with link) jumps to memzero 

    mov     sp, x0               // stack pointer = x0
    bl      main                // Jumps to shell
    b       proc_hang          

memzero:                         // Used to clear the BSS section.Starting from bss_begin, write 1 byte each time (via strb) until bss_end
    cbz     x1, memzero_done     // Compare and Branch on Zero, if x1 (BSS length) is 0, jump to memzero_done
    strb    wzr, [x0], #1        // Store Register Byte, x0 += 1 , Store 0 (wzr) to the address that x0 points to, then x0 += 1(point to the next address)
    sub     x1, x1, #1           // x1 -= 1 indicates how many bytes need to be cleared
    b       memzero              // Loop continues until x1 becomes 0

memzero_done:
    ret
