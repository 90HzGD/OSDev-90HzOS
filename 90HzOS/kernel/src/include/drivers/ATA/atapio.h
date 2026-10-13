#ifndef ATA_H
    #include "../../types.h"

    #define ATA_H
    #define READ_COMMAND 0x20
    #define WRITE_COMMAND 0x30
    #define IDENTIFY_COMMAND 0xEC

    struct __attribute__((packed))ATA_PCI_Controller {
        u8  BUS;
        u8  DEV;
        u8  FUNC;
    };

    struct ATA_BARs {
        u16 BAR0;
        u16 BAR1;
        u16 BAR2;
        u16 BAR3;
    };
    
    char  ATAControllerExists();
    char  GetATAControllerCount();
    u8* GetATA_PCI_Controller();
    struct ATA_BARs Get_ATA_BARs(u8* BUS_ID, u8* DEV_ID, u8* FUNC_ID, u8 CtrllerID);
    unsigned char SelectDrive(u16 Control_ports, u16 alternate_status, u8 val, u8 CtrllerID, u8 DiskID);
    char IdentifyATADrive(struct ATA_BARs ATARegs, u8 DriveIdx, char* Output);
    
    void read_sectors(u8 diskID, u32 LBA28, u8 sec_count, char* Output);
    void write_sectors(u8 diskID, u32 LBA28, u8 sec_count, char* data);
    void read_pio(u16 ComPort, char* Output);
    void write_pio(u16 ComPort, char* data);
    void Get_ATA_PCILoc(u8 CtrollerID, u8* PCI_Loc);

#endif