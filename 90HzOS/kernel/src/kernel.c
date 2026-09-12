#include "include/kernel.h"
#include "include/drivers/keyboard/ps2.h"
#include "include/vga/stdio.h"
#include "include/drivers/ports/ports.h"
#include "include/types.h"
#include "../../prog/src/include/terminal.h"
#include "include/string.h"
#include "include/mem/mem_alloc.h"
#include "include/drivers/ATA/atapio.h"
#include "include/vga/menu.h"

struct avail_RAM initRAMstruct;
char extended_key;

enum Return_codes_main main(){
    unsigned int* main_ptr = (unsigned int*)&main;
    unsigned int main_pointer = (unsigned int)main_ptr;

    clear_screen();

    char string[] = "In main C func in kernel:";
    printf("\033\x07[\033\x0EPASS\033\x07]\033\x0F %s\033\x06 %p\033\x00\n", string, main_pointer);

    init_RAM();
    init_idt();
    kb_init();
    enable_int();

    unsigned int* test_malloc  = (unsigned int*)malloc(sizeof(unsigned int));     /* Malloc Test */
    unsigned int* test_malloc0 = (unsigned int*)malloc(4);                        //             //
    unsigned int* test_malloc1 = (unsigned int*)malloc(sizeof(unsigned int));     /*_____________*/
    *test_malloc0 = 125125;
    *test_malloc1 = 8596;
    *test_malloc = 2000000000;
    if (test_malloc0 == 0x00 || test_malloc == 0x00 || test_malloc1 == 0x00){
        printf("\033\x07[\033\x04WARN\033\x07]\033\x0F Malloc Error.");
    }
    else {
        init_bloc(test_malloc);
        init_bloc(test_malloc0);
        init_bloc(test_malloc1);
        free(test_malloc);
        free(test_malloc0);
        free(test_malloc1);
    }

    strcpy("Initialized keyboard.\n", string);
    printf("\033\x07[\033\x0EPASS\033\x07]\033\x0F %s", string);

    clear_screen();
    printf("\033%cSource code: github.com/90HzGD/Open-source-OSDev-90HzOS\n\033\x0F", 0x01);

    next_entry(0);

    return _OK;
}

unsigned char handle_kb(){
    extern volatile unsigned char extended;
    unsigned char keycode = 0;
    unsigned char status = inb(KB_COMMAND);
    if (!extended_key){
        if (status & 1){
            keycode = inb(KB_DATA);
        }
        if (keycode == 0xE0) {extended = 1; extended_key = 1;}
        else extended = 0;
        
        outb(PIC1_COMMAND, PIC_EOI);
        return keycode;
    }
    else {
        extended_key = 0;
        if (status & 1){
            keycode = inb(KB_DATA);
        }
        outb(PIC1_COMMAND, PIC_EOI);
        return keycode;
    }
}


enum Return_codes_main init_RAM(){
    enum Return_codes_main initRAMrcode = _OK;
    printf("\033\x0FSetting Usable RAM Segments:\n");

    // Set avail_RAM struct
    unsigned int*  baseptr      =  (unsigned int*)  BASE_PTR_INITRAM;
    unsigned int*  len_ptr      =  (unsigned int*)  LENGTH_INITRAM;

    unsigned int*  base_ptr_val =  (unsigned int*)*baseptr; 
    unsigned int   length       =  *(len_ptr);
    unsigned int   total_len    =  0;
    unsigned int** arrBaseptr   =  initRAMstruct.baseptr;
    unsigned int*  arrLenptr    =  initRAMstruct.length;

    for (unsigned int i = 0; i != *((unsigned int*)0x4000); ++i){
        length                  = *(len_ptr + i);

        base_ptr_val            =  (unsigned int*)*baseptr;
        if (base_ptr_val == 0 && i > 0){
            break;
        }
        *(arrBaseptr + i)       =  base_ptr_val;
        *(arrLenptr  + i)       =  length;
        *(arrBaseptr + i + 1)   =  0;
        *(arrLenptr  + i + 1)   =  0;
        ++(baseptr);
        total_len += length;
    }

    for (unsigned int i = 0; i != *((unsigned int*)0x4000); ++i){
        printf("\t\033\6Chunk #%u : %p length: %u\tBytes\033\x0F\n", i, *(arrBaseptr + i), *(arrLenptr + i));
    }
    printf("Total Usable RAM:\033\x06 %u bytes\033\x0F\n", total_len);
    printf("\033\x07[\033\x0EPASS\033\x07]\033\x0F Set up Usable RAM Segments\n");

    // Heap start

    initRAMstruct.heap_begin    = (unsigned int*)&kernel_end;
    initRAMstruct.total_RAM     = total_len - kernel_end;
    printf("Set Kernel Heap to: \033\x06%p\033\x0F\n", initRAMstruct.heap_begin);
    init_heap(total_len);

    return initRAMrcode;
}

void init_heap(unsigned int total_RAM){
    *(HEAP_START) = 1;
    *(HEAP_START + 1) = 8;
    write_string((unsigned char*)"ENTRY", (HEAP_START + 2));
    *(HEAP_START + 4) = 2;
    total_RAM -= (total_RAM % 4);
    *(HEAP_START + 5) = total_RAM - (sizeof(unsigned int) * 4);
    return;
}