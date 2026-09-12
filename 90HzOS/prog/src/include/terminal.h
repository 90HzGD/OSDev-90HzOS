#ifndef TERMINAL_H
    #define TERMINAL_H

    void next_entry(int clear);

        unsigned char prompt();
        void init_builtin_commands();
        char** clear();
        char** help();
        char** uname(char** arguments);
        char** lspci();
        char** lside();

        struct builtinCommands {
            char*           commands[7];
            unsigned int*   builtin_adr[7];
            char            needs_args[7];
            unsigned int    arg_count_min[7];
            unsigned int    arg_count_max[7];
            char*           help[7];
        };

        enum commands_rcode{
            OK = 0,
            Unknown = 1,
            Takes_No_Arg = 2,
            Missing_Arg = 3,
            Too_Many_Arg = 4,
            Unknown_Arg = 5,
        };

        struct command {
            unsigned int* com_adr;
            char full_command               [4096];
            char command                    [64];
            char* arguments                 [256];
            enum commands_rcode rcode;
        };

        char** echo(char** arguments);
        struct command parse(char* full_command);

        void com_err(struct command Com);
        void exec_err(char** err);
        extern void exec(unsigned int* com_adr, char** arguments);
        extern volatile unsigned int position;

#endif