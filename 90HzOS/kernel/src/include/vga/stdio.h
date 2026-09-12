#ifndef STDIO_H
    #include "../types.h"
    #include "../drivers/ports/ports.h"
    #define STDIO_H
    #define VGA_BEGIN 0xB8000
    #define MOVE_GRID_BEGIN_CHAR 0xB80A0
    #define MOVE_GRID_BEGIN_ATT 0xB80A1
    #define MOVE_GRID_END 0xB8000 + (80*24)*2

    void clear_screen();
    void printf(char* string, ...);
    void print_integer(int integer, volatile unsigned int* position);
    void print_uinteger(unsigned int uinteger, volatile unsigned int* position);
    void print_hex(const unsigned int* ptr, volatile unsigned int* position, u8 mode);
    void change_color(const char color, volatile unsigned int *position);
    void move_grid(unsigned int count);
    void print_char(volatile const unsigned char displayed_char, const char attributes, volatile unsigned int *position);
    void print_string(volatile const char* string, const char attributes, volatile unsigned int* position);
    void set_BGcolor(const char color);
    u8* get_VRAMpos();
    extern void update_cursor(u8 pos_low, u8 pos_high);

#endif