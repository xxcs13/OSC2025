.section ".text.boot"
.globl _start
_start:
    mrs x5, mpidr_el1
    and x5, x5,#0xFF            // keep only the lowest 8 bits(cheack core ID)
    cbz x5, master              // if x5=0(main core), jump to master
    bne waiting

memzero:
    str xzr, [x5], #8           // tore 0 to x5, then increment x5 by 8
    subs x1, x1, #8             // subtract 8 from x1, indicates that 8 bytes of data have been processed
    b.gt memzero                // if x1>0, repeat memzero
    ret

master:
    adr    x5, bss_begin
    adr    x1, bss_end
    sub    x1, x1, x5           // x1 = x1 - x5
    bl     memzero

                                // Save dtb_addr to global variable
    ldr x1, =dtb_addr           // get the address of dtb_addr and store in x1
    str x0, [x1]                // store x0(address of dtb) to dtb_addr

    mov    sp, #0x400000        // why 0x4?, cause t is usually located in the high memory area, it avoids conflicts with code and data segments.
    bl     main

waiting:
    wfe
    b _start

.section ".data"                // store initialized global and static variables
.global dtb_addr                // dtb_addr can be referenced in other files
dtb_addr: .dc.a 0x0             // Initialize dtb_addr to 0x0

