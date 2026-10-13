#ifndef TERMINAL_H
    #define TERMINAL_H
    #include "../../../kernel/src/include/types.h"

    void next_entry(int clear);

        unsigned char prompt();
        void init_builtin_commands();
        char** clear();
        char** help();
        char** uname(char** arguments);
        char** lspci();
        char** lside();
        char** read_sector(char** arguments);
        char** write_sector(char** arguments);

        struct builtinCommands {
            char*           commands[9];
            unsigned int*   builtin_adr[9];
            char            needs_args[9];
            unsigned int    arg_count_min[9];
            unsigned int    arg_count_max[9];
            char*           help[9];
        };

        enum commands_rcode{
            OK = 0,
            Unknown = 1,
            Takes_No_Arg = 2,
            Missing_Arg = 3,
            Too_Many_Arg = 4,
            Unknown_Arg = 5,
            Invalid_option = 6,
            Missing_option = 7,
        };

        struct command {
            unsigned int* com_adr;
            char full_command               [4096];
            char command                    [64];
            char* arguments                 [1024];
            enum commands_rcode rcode;
        };

        char** echo(char** arguments);
        struct command parse(char* full_command);

        void com_err(struct command Com);
        void exec_err(char** err);
        void free_args();
        u8 check_args(char** arguments, u8 mode);
        char chk_arg_count(char** arguments, char arg_max, char arg_min);
        extern void exec(unsigned int* com_adr, char** arguments);
        extern volatile unsigned int position;

#endif