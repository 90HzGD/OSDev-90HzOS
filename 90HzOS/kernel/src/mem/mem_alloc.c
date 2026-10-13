#include "../include/mem/mem_alloc.h" 
#include "../include/kernel.h"
#include "../include/types.h"
#include "../include/vga/stdio.h"

extern struct avail_RAM initRAMstruct;

    char* malloc(unsigned int size){
        // Invalid Size
        if (size == 0){
            return 0x0;
        }
        // Makes size % 4
        size += 4 - (size % 4);
        size /= 4;
        unsigned int ALLOC_IDX = 0;
        unsigned int size_offset = 0;
        unsigned int* ret = 0;
        unsigned char status;
        unsigned int free_size;
        do {
            if (*(HEAP_START + ALLOC_IDX + size_offset) != 1 && *(HEAP_START + ALLOC_IDX + size_offset + 1) >= size * 4){
                status = *(HEAP_START + ALLOC_IDX + size_offset);
                free_size = *(HEAP_START + ALLOC_IDX + size_offset + 1);
                *(HEAP_START + ALLOC_IDX + size_offset) = 1;
                *(HEAP_START + ALLOC_IDX + size_offset + 1) = size * 4;
                ret = (HEAP_START + ALLOC_IDX + size_offset + 2);
                size_offset += size;

                if (status == 0){
                    *(HEAP_START + ALLOC_IDX + size_offset + 1) = 0;
                    if (*(&status + (free_size / 4)+1) == 0){
                        *(HEAP_START + ALLOC_IDX + size_offset + 2) = *(&status + (free_size / 4)+1) + size*4;
                    }
                }
                else {
                    *(HEAP_START + ALLOC_IDX + size_offset + 2) = 2;
                    unsigned int remain_RAM = get_remain_heap_RAM(0);
                    *(HEAP_START + ALLOC_IDX + size_offset + 3) = remain_RAM;
                }
                break;
        
            }
            size_offset += *(HEAP_START + ALLOC_IDX + size_offset + 1)/4;
            ++ALLOC_IDX;

        } while (ret == 0);
        return (char*)ret;
    }

    void free(unsigned int* ALLOC_ADR){
        if (ALLOC_ADR - 2 == initRAMstruct.heap_begin){
            return;
        }
        enum boolean last = *(ALLOC_ADR + ((*(ALLOC_ADR - 1))/4)) == 2;
        unsigned int* prev_adr;
        unsigned int* size_ptr;
        if (last){
            prev_adr = get_previous_bloc(ALLOC_ADR);
            if (*(prev_adr - 2) == 0){
                *(prev_adr - 2) = 2;
                *(prev_adr - 1) = get_remain_heap_RAM(0);
            }
            else {
                *(ALLOC_ADR - 2) = 2;
                *(ALLOC_ADR - 1) = get_remain_heap_RAM(0);
            }
            return;
        }
        else {
            prev_adr = get_previous_bloc(ALLOC_ADR);
            size_ptr = ALLOC_ADR - 1;
            if (*(prev_adr - 2) == 0){
                size_ptr = prev_adr - 1;
                *(size_ptr) += *(ALLOC_ADR - 1)+1;
            }
            else {
                *(ALLOC_ADR - 2) = 0;
            }
            unsigned int* next_adr = get_next_bloc(ALLOC_ADR);
            if (*(next_adr - 2) == 0){
                *(size_ptr) += *(next_adr - 1);
            }
            return;
        }
    }

    void write_string(unsigned char* string, unsigned int* STR_TARGET){
        for (unsigned int i = 0; *string != 0; ++i){
            *((unsigned char*)STR_TARGET + i) = *string;
            ++string;
        }
        return;
    }

    unsigned int get_remain_heap_RAM(unsigned char mode){
        unsigned char status = *(HEAP_START);
        unsigned int size_offset = 0;
        unsigned int size_offset_unused = 0;
        unsigned int ALLOC_IDX = 0;
        for (unsigned int i = 0; status != 2; ++i){
            status = *(HEAP_START + ALLOC_IDX + size_offset);
            size_offset += *(HEAP_START + ALLOC_IDX + size_offset + 1)/4;
            if (status == 0){
                size_offset_unused += *(HEAP_START + ALLOC_IDX + size_offset + 1);
            }
            ALLOC_IDX += 1;
        }
        unsigned int remain_RAM = initRAMstruct.total_RAM;
        remain_RAM -= (remain_RAM % 4);
        if (mode == 0){
            remain_RAM -= size_offset * 4 - ALLOC_IDX+1 - (unsigned int)HEAP_START;
        }
        else if (mode == 1){
            remain_RAM -= (size_offset * 4 - ALLOC_IDX+1 - (unsigned int)HEAP_START) + size_offset_unused;
        }
        remain_RAM -= (remain_RAM % 4);
        return remain_RAM;
    }

    unsigned int* get_previous_bloc(unsigned int* bloc){
        unsigned int* current_adr = initRAMstruct.heap_begin;
        unsigned int* old_adr = current_adr;
        int i = 0;         // Temp for no loop
        do {
            ++i;
            old_adr = current_adr;
            ++current_adr;
            current_adr += *(current_adr)/4;
            ++current_adr;
        } while (current_adr != bloc - 2 && i < 0);
        current_adr = old_adr+2;
        return current_adr;
    }

    unsigned int* get_next_bloc(unsigned int* bloc){
        return (bloc-1)+(*(bloc-1)/4)+3;
    }

    void init_bloc(unsigned int* ALLOC_ADR){
        for (unsigned int i = 0; i != *(ALLOC_ADR - 1)/4; ++i){
            *(ALLOC_ADR + i) = 0;
        }
        return;
    }

    char* alloc_str(char* str){
        unsigned int i = 0, len = 0;
        if (!*str){
            return 0x00;
        }
        for (;*(str + len) != 0; ++len){
            continue;
        }
        char* alloc = malloc(sizeof(char) * len + 1);
        if (!alloc){
            return 0x00;
        }
        for (;i != len; ++i){
            *(alloc + i) = *(str + i);
        }
        *(alloc + len) = 0;
        return alloc;
    }

    void free_str(char** str_adr){
        for (unsigned int i = 0; *(str_adr + i) != 0 && i != 1024; ++i){
            free((unsigned int*)*(str_adr + i));
            *(str_adr + i) = 0;
        }
        return;
    }