#ifndef STRING_H
    #define STRING_H

    void reverse_string(char* string);
    void strcpy(volatile char* string, volatile char* target);
    unsigned int length(volatile const char* string);
    unsigned int compare_string(volatile const char* string1, volatile const char* string2);
    unsigned int length_arrptr(char** string);
    unsigned char in_str_arr(char** str, char* target);
    unsigned char search_str_arr(char** str, char* target);
    void override_str(char* str, unsigned int count);
    void cut_space(char* str);

#endif