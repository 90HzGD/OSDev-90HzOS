#ifndef KERNEL_H
    #define KERNEL_H

    #define VRAM_CHAR_ADR           0xB8000
    #define VRAM_ATT_ADR            0xB8001
    #define VGA_SCREEN_WIDTH        80
    #define VGA_SCREEN_HEIGHT       25

        #define KB_COMMAND              0x64
        #define KB_DATA                 0x60

        #define BASE_PTR_INITRAM        0x4004
        #define LENGTH_INITRAM          0x5004

        #define KRNL_NAME "90HzOS Custom Kernel"
        #define KRNL_RELEASE "90HzOS Kernel [UNDER CONSTRUCTION]"
        #define KRNL_VER "UNDER CONTRUCTION"
        #define KRNL_ARCHITECTURE "x86"
        #define OS_NAME "90HzOS"
        #define KRNL_BITS 32

        struct avail_RAM {
            unsigned int* baseptr[64];
            unsigned int  length[64];
            unsigned int* heap_begin;
            unsigned int total_RAM;
        };

        enum Return_codes_main {
            _OK = 0,
        };

    extern unsigned int kernel_end;
    extern void update_stackptr(unsigned int base_address);
    #define HEAP_START (unsigned int*)&kernel_end
        
    void init_heap(unsigned int total_RAM);
    enum Return_codes_main init_RAM();

    unsigned char handle_kb();
    extern char* Get_CPU_name(void);

#endif