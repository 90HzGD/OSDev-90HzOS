#include "include/string.h" 
#include "include/vga/stdio.h"
#include "include/types.h"

    void reverse_string(char* string){
        char* string_cpy = 0;
        unsigned int str_len = length(string);
        strcpy(string, string_cpy);
        for (unsigned int i = 0; i != str_len; ++i){
            *(string + i) = *(string_cpy + str_len - 1);
        }
    }

    void strcpy(volatile char* string, volatile char* target){
        for (u8 i = 0; *(string + i) != 0; ++i){
            *(target + i) = *(string + i);
            *(target + i + 1) = 0;
        }
        return;
    }
    
    unsigned int length(volatile const char* string){
        int length = 0;
        for (unsigned int i=0; *(string+i)!='\0'; ++i){
            length += 1;
        }
        return length;
    }    

    unsigned int compare_string(volatile const char* string1, volatile const char* string2){
        unsigned int len1 = length(string1);
        unsigned int len2 = length(string2);
        if (len1 != len2){
            return 0;
        }
        else {
            for (unsigned int i=0; i<len1; ++i){
                if (string1[i] != string2[i]){
                    return 0;
                }
                continue;
            }
        }
        return 1;
    }

    unsigned char in_str_arr(char** str, char* target){
        unsigned int found = 0;
        for (unsigned int i = 0; *(str + i) != 0; ++i){
            found = compare_string(*(str + i), target);
            if (found){
                break;
            }
        }
        return found;
    }

    unsigned char search_str_arr(char** str, char* target){
        int arr_idx = -1;
        for (unsigned int found;*(str + arr_idx + 1) != 0; ++arr_idx){
            found = compare_string(*(str + arr_idx + 1), target);
            if (found){
                break;
            }
        }
        return arr_idx + 1;
    }

void override_str(char* str, unsigned int count){
    for (u32 i = 0; i != count; ++i){
        *(str+ i) = 0;
    }
    return;
}

void cut_space(char* str){
    u8 space = 0;
    for (u32 i = 0; *(str + i) != 0; ++i){
        if (*(str + i) == 0x20){
            if (!space){
                space = 1;
            }
            else {
                *(str + i) = 0;
                break;
            }
        }
    }
}