#include "../include/mem/mem.h"
#include "../include/types.h"


void memw(u8* address, u8 byte){
    *(address) = byte;
    return;
}

u8 memr(u8* address){
    return *(address);
}