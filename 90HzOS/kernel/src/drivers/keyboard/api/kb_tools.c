#include "../../../include/string.h"
#include "../../../include/drivers/keyboard/kb_tools.h"
#include "../../../include/types.h"
#include "../../../include/vga/stdio.h"

    const char scan_codes_arr[256] = {
        0,
        '\x1b',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '0',
        '-',
        '=',
        '\x08',
        '\t',
        'q',
        'w',
        'e',
        'r',
        't',
        'y',
        'u',
        'i',
        'o',
        'p',
        '[',
        ']',
        '\r',
        0,
        'a',
        's',
        'd',
        'f',
        'g',
        'h',
        'j',
        'k',
        'l',
        ';',
        '\'',
        '`',
        0,
        '\\',
        'z',
        'x',
        'c',
        'v',
        'b',
        'n',
        'm',
        ',',
        '.',
        '/',
        0,
        '*',
        0,
        ' ',
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        '7',
        '8',
        '9',
        '-',
        '4',
        '5',
        '6',
        '+',
        '1',
        '2',
        '3',
        '0',
        '.',
        0,
        0,
        '\x1b',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '0',
        '-',
        '=',
        '\x08',
        '\t',
        'q',
        'w',
        'e',
        'r',
        't',
        'y',
        'u',
        'i',
        'o',
        'p',
        '[',
        ']',
        '\r',
        0,
        'a',
        's',
        'd',
        'f',
        'g',
        'h',
        'j',
        'k',
        'l',
        ';',
        '\'',
        '`',
        0,
        '\\',
        'z',
        'x',
        'c',
        'v',
        'b',
        'n',
        'm',
        ',',
        '.',
        '/',
        0,
        '*',
        0,
        ' ',
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        '7',
        '8',
        '9',
        '-',
        '4',
        '5',
        '6',
        '+',
        '1',
        '2',
        '3',
        '0',
        '.',
        0,
        0,
    };

    char shifted_keys_arr[256];
    char letters[27] = "qwertyuiopasdfghjklzxcvbnm";

    void init_arrays(void){
        // Shifted

        *(shifted_keys_arr + '1') = '!';
        *(shifted_keys_arr + '2') = '@';
        *(shifted_keys_arr + '3') = '#';
        *(shifted_keys_arr + '4') = '$';
        *(shifted_keys_arr + '5') = '%';
        *(shifted_keys_arr + '6') = '^';
        *(shifted_keys_arr + '7') = '&';
        *(shifted_keys_arr + '8') = '*';
        *(shifted_keys_arr + '9') = '(';
        *(shifted_keys_arr + '0') = ')';
        *(shifted_keys_arr + '-') = '_';
        *(shifted_keys_arr + '=') = '+';
        *(shifted_keys_arr + '[') = '{';
        *(shifted_keys_arr + ']') = '}';
        *(shifted_keys_arr + ';') = ':';
        *(shifted_keys_arr + '\'') = '\"';
        *(shifted_keys_arr + '\\') = '|';
        *(shifted_keys_arr + ',') = '<';
        *(shifted_keys_arr + '.') = '>';
        *(shifted_keys_arr + '/') = '?';
        *(shifted_keys_arr + ' ') = ' ';
    } 

    unsigned char get_key(void){
        extern volatile unsigned char Scan_code;
        unsigned char value = Scan_code;
        return value;
    }

    unsigned char shift_key(unsigned char target){
        if (target >= 97 && target <= 122){
            target -= 32;
        }
        else if (target != '\r' && target != '\x08'){
            target = shifted_keys_arr[target];
        }
        return target;
    }

    void init_keys(void){
        init_arrays();
    }

    struct output transkey(char key){
        struct output output;
        extern volatile unsigned char extended;
        extern volatile unsigned char Ctrl_pressed;
        extern volatile unsigned char LeftShiftPressed;
        extern volatile unsigned char RightShiftPressed;
        extern volatile unsigned char ToogleCaps;
        extern volatile unsigned char Alt_pressed;
        extern volatile char Ctrl_combine[];
        extern volatile unsigned char combine_idx;
        if (!extended){
            if (Ctrl_pressed && key == 0x1D){
                output.char1 = 0;                   // Left Ctrl + None
                output.char2 = 0;
                output.Ctrlpressed = 1;
                output.ifchar = 0;
                output.released = 1;
                return output;
            }
            else {
                output.Ctrlpressed = 0;
            }

            if (Alt_pressed && key == 0x38){
                if (!Ctrl_pressed){
                    output.char1 = 0;
                }
                output.Altpressed = 1;
                output.Ctrlpressed = Ctrl_pressed;
                output.ifchar = 0;
                output.released = 0;
                return output;
            }
            unsigned char Scan_code = key;
            int ifchar = 0;
            int release = 0;
            if (((Scan_code >= 2 && Scan_code < 14) || (Scan_code >= 16 && Scan_code < 28) || (Scan_code >= 30 && Scan_code < 42) || (Scan_code >= 43 && Scan_code < 54) || (Scan_code == 55) || (Scan_code == 0x39) || (Scan_code >= 71 && Scan_code < 87)) && Scan_code < 89){
                ifchar = 1;
                release = 0;
            }
            else if(((Scan_code >= 2+89 && Scan_code < 14+0x80) || (Scan_code >= 16+0x80 && Scan_code < 28+0x80) || (Scan_code >= 30+0x80 && Scan_code < 42+0x80) || (Scan_code >= 43+0x80 && Scan_code < 54+0x80) || (Scan_code == 55+0x80) || (Scan_code == 0xb9) || (Scan_code >= 71+0x80 && Scan_code < 87+0x80)) && Scan_code < 89+0x80) {
                ifchar = 1;
                release = 1;
            }
            else{
                ifchar = 0;
                if (Scan_code >= 0x81){
                    release = 1;
                }
                else {
                    release = 0;
                }
                output.char1 = scan_codes_arr[Scan_code];
            }
            if (ifchar){
                output.char1 = scan_codes_arr[Scan_code];
            }
            if (ToogleCaps ^ (LeftShiftPressed || RightShiftPressed)){
                unsigned char shifted_key = shift_key(output.char1);
                output.char1 = shifted_key;
            }        
            if(Ctrl_pressed && key != 0x1D){
                if (combine_idx >= 5){
                    output.char1 = Ctrl_combine[0];
                    output.char2 = Ctrl_combine[1];
                    output.char3 = Ctrl_combine[2];
                    output.char4 = Ctrl_combine[3];
                    output.char5 = Ctrl_combine[4];
                    output.char6 = Ctrl_combine[5];
                    output.Altpressed = Alt_pressed;
                    output.Ctrlpressed = 1;
                    output.ifchar = 0;
                    output.released = 1;
                    return output;
                }
                if (!release){
                    Ctrl_combine[combine_idx++] = output.char1;
                    Ctrl_combine[combine_idx] = 0;
                }
                else {
                    if (combine_idx > 0){
                        Ctrl_combine[combine_idx-1] = 0;
                        combine_idx--;
                    }
                }
                output.char1 = Ctrl_combine[0];
                output.char2 = Ctrl_combine[1];
                output.char3 = Ctrl_combine[2];
                output.char4 = Ctrl_combine[3];
                output.char5 = Ctrl_combine[4];
                output.char6 = Ctrl_combine[5];
                output.Altpressed = Alt_pressed;
                output.Ctrlpressed = 1;
                output.ifchar = 0;
                output.released = 0;
                return output;
            }
            else if (Alt_pressed && key != 0x38 && (!Ctrl_pressed)){
                output.Altpressed = 1;
                output.Ctrlpressed = 0;
            }
            output.released = release;
            output.ifchar = ifchar;
            output.extended = 0;
        }
        else {
            output = extended_char(key);
            extended = 0;
        }
        return output;
    }

    struct output extended_char(unsigned char scan_code){
        unsigned char released = (scan_code > 0x90);
        struct output output;
        output.extended = 1;
        output.released = released;
        switch (scan_code){
            case 0x35:case 0xB5:
                output.char1 = '/';
                break;
            case 0x48:case 0xC8:
                output.char1 = 'U';
                break;
            case 0x4B:case 0xCB:
                output.char1 = 'L';
                break;
            case 0x4D:case 0xCD:
                output.char1 = 'R';
                break;
            case 0x50:case 0xD0:
                output.char1 = 'D';
                break;
            case 0x1C:case 0x9C:
                output.char1 = '\r';
                break;
            default:
                output.char1 = 0;
                break;
        }
        if (output.char1 != '/'){
            output.ifchar = 0;
        }
        else {
            output.ifchar = 1;
        }
        output.Ctrlpressed = 0;
        output.Altpressed = 0;
        return output;
}

void cin(char* output_address){
    extern volatile unsigned int position;
    u8 Oldkey = 0, key = 0;
    struct output trans_key;
    char full_input[4096];
    full_input[0] = 0;
    unsigned int input_pos = 0;
    unsigned int prompt_pos = position;
    override_str(full_input, 4096);
    override_str(output_address, 4096);
    while (1){
        Oldkey = key;
        key = get_key();
        if (key == Oldkey || key == 0){
            continue;
        }
        trans_key = transkey(key);
        if (trans_key.char1 == '\r' || trans_key.char1 == '\t'){
            if (trans_key.char1 == '\r' && !trans_key.released){
                while (!trans_key.released){
                    key = get_key();
                    trans_key = transkey(key);
                }
                strcpy(full_input, output_address);
                return;
            }
            else {
                continue;
            }
        }
        if (trans_key.char1 == '\x08' && !trans_key.released){
            unsigned int len = length(full_input);
            if (len == 0){
                continue;
            }
            input_pos -= 1;
            full_input[len-1] = '\0';
            --position;
            print_char(0, 0x0F, &position);
            position = prompt_pos;
            printf("%s", full_input, 0);
            continue;
        }
        if (!trans_key.released && trans_key.char1 != 0 && !trans_key.extended){
            *(full_input + input_pos) = trans_key.char1;
            input_pos += 1;
            position = prompt_pos;
            *(full_input + input_pos) = 0;
            printf("%s", full_input);
            continue;
        }
    }

}

struct arrow_info get_arrow_keys(){
    u8 key = get_key();
    struct output translated_key = transkey(key);
    struct arrow_info ret;
    ret.key = 0;
    ret.released = 0;
    if (translated_key.extended){
        if (translated_key.char1 == 'D' || translated_key.char1 == 'U' || translated_key.char1 == 'L' || translated_key.char1 == 'R') {ret.released = translated_key.released; ret.key = translated_key.char1;}
    }
    if (translated_key.char1 == '\r') ret.enter = 1;
    else ret.enter = 0;
    return ret;
}