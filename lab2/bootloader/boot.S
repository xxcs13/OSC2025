.section ".text.boot"
.globl _start

_start:
    mrs x5, mpidr_el1
    and x5, x5, #0xFF       // keep only the lowest 8 bits(cheack core ID)
    cbz x5, master          // if x5=0(main core), jump to master 
    b waiting

relocate:                   // Check if the _start address is already at the 0x60000.
    adr x10, _start         // get the address of _start
    mov x11, #0x60000       // define the address of bootloader
    cmp x10, x11            // check if the address of _start is the same as the address of bootloader
    b.eq skipr              // if the address of _start is the same as the address of bootloader, if same then skip the relocation

    mov x5, #0x80000        // define the source address
    mov x1, #0x60000        // define the target address
    mov x2, #0x2000         // define the size of the relocation

    nop
    nop
    nop

relocation:                 
    ldr x3, [x5], #8        // load the data(8 bytes) from x5(source address) to x3, and increment x5 by 8 (point to the next 8 bytes)
    str x3, [x1], #8        // Write the data from x3 to x1(target address), and increment x1 by 8 (pointing to the next 8 bytes)
    subs x2, x2, #8         // subtract the size of the relocation, indicates that 8 bytes of data have been processed
    b.gt relocation         // if the size of the relocation is greater than 0, repeat the relocation

                            // x30 is the Link Register (LR), usually used to store return addresses
    mov x30, #0x60000       // define the address of bootloader
    br x30                  // jump to the address of bootloader

skipr:
    ldr x1, =dtb_addr       // get the address of dtb_addr and store in x1
    str x0, [x1]            // store x0(address of dtb) to dtb_addr
    b main

master:
    adr x5, bss_begin
    adr x1, bss_end
    sub x1, x1, x5          // x1 = x1 - x5
    bl memzero

    mov sp, #0x400000       // why 0x4?, cause t is usually located in the high memory area, it avoids conflicts with code and data segments.
    bl relocate          

memzero:
    str xzr, [x5], #8       // store 0 to x5, then increment x5 by 8
    subs x1, x1, #8         // subtract 8 from x1, indicates that 8 bytes of data have been processed
    b.gt memzero            // if x1>0, repeat memzero
    ret

waiting:
    wfe
    b waiting

.section ".data"           // store initialized global and static variables
.global dtb_addr           // dtb_addr can be referenced in other files
dtb_addr:                  // store the address of the device tree
    .quad 0                // A 64-bit (8-byte) value is stored in the dtb_addr location with 
                           // with an initial value of 0.
                           // later be updated to the actual DTB address in the program.