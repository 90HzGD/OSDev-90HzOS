#ifndef MENU_H
    #define MENU_H
    #include "../types.h"

    void init_values();
    void create_menu(char** options, char mode);
    void update_selected(char menuID, u32 idx);
    void edit_highlight(char menuID, char color);
    void edit_color(char menuID, char color);
    int handle_menu(char menuID);
#endif