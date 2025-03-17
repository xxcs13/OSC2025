#include "cpio.h"


void cpio_ls(unsigned long initramfs_start) {
    uart_puts("\n\rList files:\n");
    char *initrd_addr = (char *)initramfs_start;

    
    while (1) {
        struct cpio_newc_header *header = (struct cpio_newc_header *)initrd_addr;

        // check magic number
        char *magic = "070701";
        int magic_match = 1;
        for(int i = 0; i < 6; i++) {
            if(header->c_magic[i] != magic[i]) {
                magic_match = 0;
                break;
            }
        }
        
        if(!magic_match) {
            uart_puts("magic number mismatch\n\r");
            break;
        }

        int namesize = h2d(header->c_namesize);
        int filesize = h2d(header->c_filesize);

        char *name = initrd_addr + sizeof(struct cpio_newc_header);
        if(strcmp(name,"TRAILER!!!") == 0) {
            break;
        }
        uart_puts(name);
        // calculate aligned offset
        unsigned int name_offset = sizeof(struct cpio_newc_header) + namesize;
        unsigned int file_offset = filesize;

        // align to 4 bytes
        name_offset = (name_offset + 3) & ~3;
        file_offset = (file_offset + 3) & ~3;

        // move to next header
        initrd_addr += name_offset + file_offset;
        uart_puts("\n\r");
    }
}

void cpio_cat(char *filename,unsigned long initramfs_start) {
    char *initrd_addr = (char *)initramfs_start;

    while (1) {
        struct cpio_newc_header *header = (struct cpio_newc_header *)initrd_addr;

        // check magic number
        char *magic = "070701";
        int magic_match = 1;
        for(int i = 0; i < 6; i++) {
            if(header->c_magic[i] != magic[i]) {
                magic_match = 0;
                break;
            }
        }
        
        if(!magic_match) {
            uart_puts("Magic number mismatch\n\r");
            break;
        }

        int namesize = h2d(header->c_namesize);
        int filesize = h2d(header->c_filesize);

        char *name = initrd_addr + sizeof(struct cpio_newc_header);
        if(strcmp(name,"TRAILER!!!") == 0) {
            uart_puts("No such file\n\r");
            break;
        }
        // calculate aligned offset
        unsigned int name_offset = sizeof(struct cpio_newc_header) + namesize;
        unsigned int file_offset = filesize;

        // align to 4 bytes
        name_offset = (name_offset + 3) & ~3;
        file_offset = (file_offset + 3) & ~3;

        if(strcmp(name,filename) == 0) {
            char *filedata = initrd_addr + name_offset;
            uart_puts(filedata);
            break;
        }
        // move to next header
        initrd_addr += name_offset + file_offset;
    }
}
