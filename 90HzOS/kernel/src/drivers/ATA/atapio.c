#include "../../include/drivers/ATA/atapio.h"
#include "../../include/drivers/PCI/PCI.h"
#include "../../include/vga/stdio.h"
#include "../../include/drivers/ports/ports.h"

char ATAControllerExists(){
    struct PCIDev_Descriptor Device;
    for (unsigned short i = 0; i != BUS_COUNT; ++i){
        for (unsigned int j = 0; j != DEV_COUNT; ++j){
            for (u16 k = 0; k != 8; ++k){
            
                Device = GetDevInfo((u8)i, (u8)j, (u8)k);
                if (Device.VENDOR_ID == 0 || Device.VENDOR_ID == 0xFFFF){
                    continue;
                }
                else if (Device.CLASS == 1 && Device.SUBCLASS == 1){
                    return 1;
                }
            }
        }
    }
    return 0;
}

char GetATAControllerCount(){
    struct PCIDev_Descriptor Device;
    i8 count = 0;
    for (unsigned short i = 0; i != BUS_COUNT; ++i){
        for (unsigned int j = 0; j != DEV_COUNT; ++j){
            for (u16 k = 0; k != 8; ++k){
            
                Device = GetDevInfo((u8)i, (u8)j, (u8)k);
                if (Device.VENDOR_ID == 0|| Device.VENDOR_ID == 0xFFFF){
                    continue;
                }
                else if (Device.CLASS == 1 && Device.SUBCLASS == 1){
                    ++count;
                }
            }
        }
    }
    return count;
}

struct ATA_PCI_Controller Controllers[5];

u8* GetATA_PCI_Controller(){
    struct PCIDev_Descriptor Device;
    u8 count = 0;
    for (unsigned short i = 0; i != BUS_COUNT; ++i){
        for (unsigned int j = 0; j != DEV_COUNT; ++j){
            for (u16 k = 0; k != 8; ++k){
            
                Device = GetDevInfo((u8)i, (u8)j, (u8)k);
                if (Device.VENDOR_ID == 0 || Device.VENDOR_ID == 0xFFFF){
                    continue;
                }
                else if (Device.CLASS == 1 && Device.SUBCLASS == 1 && count < 5){
                    (Controllers+count)->BUS = i;
                    (Controllers+count)->DEV = j;
                    (Controllers+count)->FUNC = k;
                    ++count;
                }
            }
        }
    }
    (Controllers+count)->BUS = 0;
    (Controllers+count)->DEV = 0xFF;
    (Controllers+count)->FUNC = 0xFF;
    return (u8*)&Controllers;
}

void Get_ATA_PCILoc(u8 CtrollerID, u8* PCILoc){
    u8* Controllers = GetATA_PCI_Controller();
    *(PCILoc) = *(Controllers+(CtrollerID*3));
    *(PCILoc+1) = *(Controllers+(CtrollerID*3)+1);
    *(PCILoc+2) = *(Controllers+(CtrollerID*3)+2);
}

struct ATA_BARs Get_ATA_BARs(u8* BUS_ID, u8* DEV_ID, u8* FUNC_ID, u8 CtrllerID){
    u16 pci_command = (u16)read_PCI(*BUS_ID, *DEV_ID, *FUNC_ID, 0x04);

    if (!(pci_command & 0x01)) {
        pci_command |= 0x01; // Enable I/O port mapping exec
        pci_command |= 0x04; // Enable Bus Mastering
        write_PCI(*BUS_ID, *DEV_ID, *FUNC_ID, 0x04, pci_command);
    }

    struct ATA_BARs ATA_regs;
    u8 mode = (u8)read_PCI(*BUS_ID, *DEV_ID, *FUNC_ID, 0x09); // Legacy Mode
    if ((mode & 1) == 0x00 && CtrllerID < 2){
        if (CtrllerID == 0){
            ATA_regs.BAR0 = 0x1F0;
            ATA_regs.BAR1 = 0x3F6;
        }
        else if (CtrllerID == 1){
            ATA_regs.BAR0 = 0x1E8;
            ATA_regs.BAR1 = 0x3E6;
        }
    }
    else {                                                                  // Native Mode
        ATA_regs.BAR0 = (u16)(read_PCI(*BUS_ID, *DEV_ID, *FUNC_ID, 0x10) & 0xFFFFFFFC);
        ATA_regs.BAR1 = (u16)(read_PCI(*BUS_ID, *DEV_ID, *FUNC_ID, 0x14) & 0xFFFFFFFC)+2;
    }
    if ((mode & 4) == 0x00 && CtrllerID < 2){
        if (CtrllerID == 0){
            ATA_regs.BAR2 = 0x170;
            ATA_regs.BAR3 = 0x376;
        }
        else if (CtrllerID == 1){
            ATA_regs.BAR2 = 0x168;
            ATA_regs.BAR3 = 0x366;
        }
    }
    else {
        ATA_regs.BAR2 = (u16)(read_PCI(*BUS_ID, *DEV_ID, *FUNC_ID, 0x18) & 0xFFFFFFFC);
        ATA_regs.BAR3 = (u16)(read_PCI(*BUS_ID, *DEV_ID, *FUNC_ID, 0x1C) & 0xFFFFFFFC)+2;
    }
    return ATA_regs;
}

struct ATA_BARs ATAPorts;
u8* ATAControllers;

int selected_drive = -1;  // Illegal value

unsigned char SelectDrive(u16 Control_ports, u16 alternate_status, u8 val, u8 CtrllerID, u8 DiskID){
    u8 Old_selection = selected_drive;
    ATAControllers = GetATA_PCI_Controller();
    ATAPorts = Get_ATA_BARs(ATAControllers+CtrllerID*3, ATAControllers+CtrllerID*3+1, ATAControllers+CtrllerID*3+2, CtrllerID);

    outb((Control_ports + 6), val);
    for (u8 i = 0; i != 15; ++i){           // +400ns delay
        inb(alternate_status);
    }
    u8 output = inb(alternate_status);
    if (output == 0xFF || output == 0x00){
        selected_drive = Old_selection;
        return 0xFF;
    }
    { 
        u32 safe_timeout = 5000000;
        u8 status = 0;
        while (safe_timeout--) {
            status = inb(Control_ports + 7);
            if (!(status & 0x80)) {
                return 0x00;
            }
        }
    }
    selected_drive = DiskID;
    return 0x00;
}

char IdentifyATADrive(struct ATA_BARs ATARegs, u8 DriveIdx, char* Output){
    u8 ret = 1;             // 0 == does not exist; 1 == ATA; 2 == Legacy mode SATA (AHCI = OFF); 3 = Other
    char poll_output = 1;
    char DriveID;
    u16 ComPort = ATARegs.BAR0;
    u16 CtrlPort = ATARegs.BAR1;
    switch (DriveIdx){
        case 0:case 2:case 4:case 6:
            DriveID = 0xA0;
            break;
        case 1:case 3:case 5:case 7:
            DriveID = 0xB0;
            break;
    }
    if ((DriveIdx >= 2 && DriveIdx < 4)){
        ComPort = ATARegs.BAR2;
        CtrlPort = ATARegs.BAR3;
    }
    outb((ComPort+6), DriveID);
    outb((ComPort+2), 0x00);
    outb((ComPort+3), 0x00);
    outb((ComPort+4), 0x00);
    outb((ComPort+5), 0x00);
    outb((ComPort+7), IDENTIFY_COMMAND);       // Identify Command

    for (u8 i = 0; i != 15; ++i){
        inb(CtrlPort);  // safe waiting
    }

    *Output = inb((CtrlPort));
    if (*Output != 0 && (u8)*Output != 0xFF){
        u32 timeout = 100000;
        while (--timeout){
            poll_output = inb(ComPort+7);
            if (poll_output == 0 || (u8)poll_output == 0xFF){
                *Output = 0; *(Output+1) = 0;
                return 0;
            }
            if (!(poll_output & 0x80)) { break; } // BSY cleared
        }
        if (timeout == 0){
            *Output = 0; *(Output + 1) = 0;
            return 0;
        }
        u8 LBA_mid = inb(ComPort+4); u8 LBA_high = inb(ComPort+5);
        if (LBA_mid || LBA_high){
            if (LBA_mid == 0x3C && LBA_high == 0xC3){
                ret = 2;
                goto DEBUG_READ;
            }
            else{
                return 3;
            }
        }
        else if ((inb(ComPort+7) & 0x01)){
            *Output = 0; *(Output + 1) = 0;
            return 0;
        }
        else {
            DEBUG_READ:
            timeout = 100000;
            poll_output = 1;
            do {
                poll_output = inb(ComPort+7);
                if(poll_output & 0x08){ break; }
            } while (--timeout);

            if (timeout == 0){
                *Output = 0; *(Output + 1) = 0;
                return 0;
            }

            u16 i = 0;
            u16 word = 0;
            while (i < 256){
                word = inw(ComPort);
                *(Output + i*2) = (u8)(word >> 8);
                *(Output + i*2 + 1) = (u8)word;
                ++i;
            }
        }
    }
    return ret;
}

void read_sectors(u8 diskID, u32 LBA28, u8 sec_count, char* Output){
    u8 disk_status;
    struct ATA_BARs ATAreg;
    u8 ControllerID = diskID/4;
    diskID %= 4;
    u8* PCILoc = {0};           // 0 in it for no GCC warn
    Get_ATA_PCILoc(ControllerID, PCILoc);
    ATAreg = Get_ATA_BARs(PCILoc, PCILoc+1, PCILoc+2, ControllerID);
    u16 ComPort = ATAreg.BAR0;
    u16 CtrlPort = ATAreg.BAR1;

    if ((diskID >= 2 && diskID < 4)){
        ComPort = ATAreg.BAR2;
        CtrlPort = ATAreg.BAR3;
    }

    u8 drive = diskID%2;
    if (selected_drive != diskID){
        if (drive == 0) SelectDrive(ComPort, CtrlPort, 0xA0, ControllerID, diskID);
        else SelectDrive(ComPort, CtrlPort, 0xB0, ControllerID, diskID);
    }

    {
        u32 mode = 0;
        mode = (0xE0 | (drive << 4) | ((LBA28 >> 24) & 0x0F));     // Set LBA mode 
        outb(ComPort+6, mode);
        outb(ComPort+2, sec_count);     // Max 256;
        outb(ComPort+3, (u8)LBA28);
        outb(ComPort+4, (u8)(LBA28 >> 8));
        outb(ComPort+5, (u8)(LBA28 >> 16));
        outb(ComPort+7, READ_COMMAND);
    }

    u32 timeout = 100000;
    while (--timeout){
        disk_status = inb(CtrlPort);
        if (disk_status == 0xFF){ return;}
        if (!(disk_status & 0x80) && disk_status & 0x08) { break; }   // BSY bit cleared
    }
    if (timeout == 0) return;
    for (u8 i = 0; i < sec_count; ++i){ 
        read_pio(ComPort, Output+512*i);
    }
}

void write_sectors(u8 diskID, u32 LBA28, u8 sec_count, char* data){
    u8 disk_status;
    struct ATA_BARs ATAreg;
    u8 ControllerID = diskID/4;
    diskID %= 4;
    u8* PCILoc = {0};           // 0 in it for no GCC warn
    Get_ATA_PCILoc(ControllerID, PCILoc);
    ATAreg = Get_ATA_BARs(PCILoc, PCILoc+1, PCILoc+2, ControllerID);
    u16 ComPort = ATAreg.BAR0;
    u16 CtrlPort = ATAreg.BAR1;

    if ((diskID >= 2 && diskID < 4)){
        ComPort = ATAreg.BAR2;
        CtrlPort = ATAreg.BAR3;
    }

    u8 drive = diskID%2;
    if (selected_drive != diskID){
        if (drive == 0) SelectDrive(ComPort, CtrlPort, 0xA0, ControllerID, diskID);
        else SelectDrive(ComPort, CtrlPort, 0xB0, ControllerID, diskID);
    }

    {
        u32 mode = 0;
        mode = (0xE0 | (drive << 4) | ((LBA28 >> 24) & 0x0F));     // Set LBA mode 
        outb(ComPort+6, mode);
        outb(ComPort+2, sec_count);     // Max 256;
        outb(ComPort+3, (u8)LBA28);
        outb(ComPort+4, (u8)(LBA28 >> 8));
        outb(ComPort+5, (u8)(LBA28 >> 16));
        outb(ComPort+7, WRITE_COMMAND);
    }

    u32 timeout = 100000;
    while (--timeout){
        disk_status = inb(CtrlPort);
        if (disk_status == 0xFF){ return;}
        if (!(disk_status & 0x80) && disk_status & 0x08) { break; }   // BSY bit cleared
    }
    if (timeout == 0) return;
    for (u8 i = 0; i < sec_count; ++i){ 
        write_pio(ComPort, data);
    }
}

void read_pio(u16 ComPort, char* Output){
    u16 i = 0;
    u16 word = 0;
    while (i < 256){
        word = inw(ComPort);
        *(Output + i*2) = (u8)word;
        *(Output + i*2 + 1) = (u8)(word >> 8);
        ++i;
    }
}

void write_pio(u16 ComPort, char* data){
    u16 word = 0;
    for (int i = 0; i < 256; ++i) {
        word = (u16)(data[i * 2] | (data[i * 2 + 1] << 8));
        outw(ComPort, word);
    }

}