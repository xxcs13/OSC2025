#include "fdt.h"

unsigned long initramfs_start = 0;


/* Devicetree .dtb Structure:

+-----------------------------------------------------------+
|   alignment - 8 bytes,        fdt_header_t                |
+-----------------------------------------------------------+
|                       (free space)                        |
+-----------------------------------------------------------+
|   alignment - 8 bytes,        memory reservation block    |
+-----------------------------------------------------------+
|                       (free space)                        |
+-----------------------------------------------------------+
|   alignment - 4 bytes,        structure block             |
+-----------------------------------------------------------+
|                       (free space)                        |
+-----------------------------------------------------------+
|   no alignment requirement,   strings block               |
+-----------------------------------------------------------+
|                       (free space)                        |
+-----------------------------------------------------------+
*/

/* structure block:

1. All tokens shall be aligned on 4 bytes after the token’s data. 
2. FDT_END token marks the end of the structure block.

    tree node:

    +-------------------------------------------+
    |   (optional) FDT_NOP, any number          |
    +-------------------------------------------+
    |   FDT_BEGIN_NODE                          |
    +-------------------------------------------+
    |   node name, null-terminated name string  |
    +-------------------------------------------+
    |   properties of the node                  |
    +-------------------------------------------+
    |   child nodes                             |
    +-------------------------------------------+
    |   (optional) FDT_NOP, any number          |
    +-------------------------------------------+
    |   FDT_END_NODE                            |
    +-------------------------------------------+
*/

uint32_t bswap32(uint32_t x) {
    return __builtin_bswap32(x);
}

void fdt_traverse(void *fdt,fdt_callback callback)
{
    struct fdt_header *header = (struct fdt_header *)fdt;

    if(bswap32(header->magic) != FDT_MAGIC){
        return;
    }
    char *struct_block = (char *)fdt + bswap32(header->off_dt_struct);
    char *strings_block = (char *)fdt + bswap32(header->off_dt_strings);
    char *current_node=NULL;
    char *current_property=NULL;
    char *current_value=NULL;
    uint32_t len=0;;
    uint32_t name_offset=0;
    while(1){
        uint32_t tag = bswap32(*(uint32_t *)struct_block);
        struct_block += 4;

        if(tag == FDT_BEGIN_NODE){
            current_node = (char *)struct_block;//node name
            len = align(strlen(current_node)+1,4);//skip node name, add +1 to skip \0
            struct_block += len;
        }else if(tag == FDT_PROP){
            len = bswap32(*(uint32_t *)(struct_block));
            struct_block += 4;
            name_offset = bswap32(*(uint32_t *)(struct_block));
            struct_block += 4;

            current_property = (char *)strings_block+name_offset;
            current_value = (char *)struct_block;
            if(callback){
                callback(current_node,current_property,current_value);
            }
            struct_block += align(len,4);
        }else if(tag == FDT_NOP){
            continue;
        }else if(tag == FDT_END_NODE){
        }else if(tag == FDT_END){
            return;
        }else{
            return;
        }
    }
}
