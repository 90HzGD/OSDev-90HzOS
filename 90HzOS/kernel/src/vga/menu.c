#include "../include/vga/menu.h"
#include "../include/vga/stdio.h"
#include "../include/string.h"
#include "../include/drivers/keyboard/kb_tools.h"

char menu_count = 0;    // Max 2
char highlight_color[2];    
char text_color[2];
char** menu_options[2];
u8* menu_VRAMStart[2];
char** options0;
char** options1;
char max_len[2];
char menu_mode[2];
char menu_idx[2];
u32 options_count[2];
u32 select_idx = 0;
char handle = 1;

void init_values(){                     // execute this first before creating menu!
    override_str(highlight_color, 2);
    override_str(text_color, 2);
    override_str(max_len, 2);
    override_str(menu_mode, 2);
    override_str(menu_idx, 2);
    override_str((char*)menu_options, 2);
    override_str((char*)options0, 1);
    override_str((char*)options1, 1);
    override_str((char*)menu_VRAMStart, 2);
    *options_count = 0;
    *(options_count + 1) = 0;
    return;
}

void create_menu(char** options, char mode){
    // Setting Default Values
    if (menu_count >= 2){
        return;
    }
    
    *(menu_idx + menu_count) = 0;
    *(menu_mode + menu_count) = mode;
    *(highlight_color + menu_count) = 0x70;
    *(text_color + menu_count) = 0x0F;
    *(menu_VRAMStart + menu_count) = get_VRAMpos();
    select_idx = 0;
    char** options_ptr;
    if (menu_count == 0) options_ptr = (char**)options0;
    else options_ptr = (char**)options1;

    if (mode == 0) printf("\n\033\x70%s\n\033\x0F", *options);
    else printf("\n\033\x70 %s \033\x0F", *options);
    *options_ptr = *options;
    u8 len = 0;

    for (u8 i = 1; *(options + i) != 0; ++i){

        len = length(*(options + i));
        if (len > *(max_len + menu_count)) *(max_len + menu_count) = len;
        *(options_ptr + i) = *(options + i);
        *(options_ptr + i + 1) = 0;

        if (mode == 0) printf("%s\n", *(options+i));
        else printf(" %s ", *(options+i));
        *(options_count + menu_count) = i;
    }
    *(menu_options) = options_ptr;
    ++menu_count;
    return;
}

void update_selected(char menuID, u32 idx){
    extern volatile unsigned int position;
    u8* local_pos = get_VRAMpos();
    position -= (((u32)local_pos) - ((u32)*(menu_VRAMStart + menuID)))-80;
    *(menu_idx + menuID) = idx;
    for (u32 i = 0; *(*(menu_options + menuID) + i) != 0; ++i){

        if (i == idx){
            select_idx = i;
            if (*(menu_mode + menuID) == 0) printf("\033%c%s\n\033%c", *(highlight_color + menuID), *(*(menu_options + menuID) + i), *(text_color + menuID));
            
            else printf("\033%c%s\033%c  ", *(highlight_color + menuID), *(*(menu_options + menuID) + i), *(text_color + menuID));
        }  

        else if (*(menu_mode + menuID) == 0) printf("\033%c%s\033\x0F\n", *(text_color + menuID), *(*(menu_options + menuID) + i));

        else printf("\033%c%s\033\x0F  ", *(text_color + menuID), *(*(menu_options + menuID) + i));
    }
    return;
}

void edit_highlight(char menuID, char color){
    *(highlight_color + menuID) = color;
    update_selected(menuID, select_idx);
    return;
}

void edit_color(char menuID, char color){
    *(text_color + menuID) = color;
    update_selected(menuID, select_idx);
    return;
}

int handle_menu(char menuID){     // Do not use for custom controls
    u32 selected_idx = *(menu_idx + menuID);
    struct arrow_info key = get_arrow_keys();
    if (key.released) {handle = 1;}

    if (!key.enter){
        if (*(menu_mode + menuID) == 0 && !key.released && handle){
            switch (key.key){
                case 'U':
                    if (selected_idx != 0) *(menu_idx + menuID) -= 1;
                    else *(menu_idx + menuID) = *(options_count + menuID);
                    update_selected(menuID, *(menu_idx + menuID));
                    handle = 0;
                    break;
                case 'D':
                    if (selected_idx < *(options_count + menuID)) *(menu_idx + menuID)+= 1;
                    else *(menu_idx + menuID) = 0;
                    update_selected(menuID, *(menu_idx + menuID));
                    handle = 0;
                    break;
            }
        }
        else if (*(menu_mode + menuID) == 1 && !key.released && handle){
            switch (key.key){
                case 'L':
                    if (selected_idx != 0) *(menu_idx + menuID) -= 1;
                    else *(menu_idx + menuID) = *(options_count + menuID);
                    update_selected(menuID, *(menu_idx + menuID));
                    handle = 0;
                    break;
                case 'R':
                    if (selected_idx < *(options_count + menuID)) *(menu_idx + menuID)+= 1;
                    else *(menu_idx + menuID) = 0;
                    update_selected(menuID, *(menu_idx + menuID));
                    handle = 0;
                    break;
            }
        }
    }
    else if (!key.released && handle){
        return *(menu_idx + menuID);
    }
    return -1;
}
