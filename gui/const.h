#include "../common.h"

extern const Word LCDC_ADDRESS;

extern const Word SCY_ADDRESS;
extern const Word SCX_ADDRESS;

extern const Word LYC_ADDRESS;
extern const Word LY_ADDRESS;

extern const Word BGP_ADDRESS;
extern const Word OBP0_ADDRESS;
extern const Word OBP1_ADDRESS;

extern const Word WY_ADDRESS;
extern const Word WX_ADDRESS;

extern const Word IE_ADDRESS;
/*
    Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
    Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
    Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
    Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
    Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)
*/
extern const Word IF_ADDRESS;
/*
FF41 :
    Bit 6 - LYC=LY Coincidence Interrupt (1=Enable) (Read/Write)
    Bit 5 - Mode 2 OAM Interrupt         (1=Enable) (Read/Write)
    Bit 4 - Mode 1 V-Blank Interrupt     (1=Enable) (Read/Write)
    Bit 3 - Mode 0 H-Blank Interrupt     (1=Enable) (Read/Write)
    Bit 2 - Coincidence Flag  (0:LYC<>LY, 1:LYC=LY) (Read Only)
    Bit 1-0 - Mode Flag       
           0: During H-Blank
           1: During V-Blank
           2: During Searching OAM
           3: During Transferring Data to LCD Driver
*/
extern const Word STAT_ADDRESS;

extern const int FLAG_ZERO;
extern const int FLAG_SUBTRACT;
extern const int FLAG_HALFCARRY;
extern const int FLAG_CARRY;

extern const int MODE_VRAM;
extern const int MODE_OAM;
extern const int MODE_HBLANK;
extern const int MODE_VBLANK;
