#define PBASE                                          0x3F000000  // Physical base address of the GPIO registers 
#define GPFSEL1     ((volatile unsigned int *)(PBASE + 0x00200004)) // GPIO Function Select 1
#define GPSET0      ((volatile unsigned int *)(PBASE + 0x0020001C)) // GPIO Pin Output Set 0
#define GPCLR0      ((volatile unsigned int *)(PBASE + 0x00200028)) // GPIO Pin Output Clear 0
#define GPPUD       ((volatile unsigned int *)(PBASE + 0x00200094)) // GPIO Pin Pull-up/down Enable
#define GPPUDCLK0   ((volatile unsigned int *)(PBASE + 0x00200098)) // GPIO Pin Pull-up/down Enable Clock 0