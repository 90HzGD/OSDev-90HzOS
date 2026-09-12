#ifndef KB_DATA_H
    #define KB_DATA_H

    void init_arrays(void);

    struct output {
        unsigned char char1;
        unsigned char char2;
        unsigned char char3;
        unsigned char char4;
        unsigned char char5;
        unsigned char char6;
        unsigned char Altpressed;
        unsigned char Ctrlpressed;
        unsigned char ifchar;
        unsigned char released;
        unsigned char extended;
    };

    struct arrow_info {
        unsigned char key;
        unsigned char released;
    };
    
#endif