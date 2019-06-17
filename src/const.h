#include "common.h"
/*FF40 - LCDC - LCD Control (R/W)
Bit 7 - LCD Display Enable
Bit 6 - Window Tile Map Display Select  (0=9800-9BFF, 1=9C00-9FFF)Bit 5 - Window Display Enable           (0=Off, 1=On) (0=Off,1=On)
Bit 4 - BG & Window Tile Data Select    (0=8800-97FF, 1=8000-8FFF
Bit 3 - BG Tile Map Display Select      (0=9800-9BFF, 1=9C00-9FFF)
Bit 2 - OBJ (Sprite) Size               (0=8x8, 1=8x16)
Bit 1 - OBJ (Sprite) Display Enable     (0=Off, 1=On)
Bit 0 - BG Display                      (0=Off, 1=On)*/
extern const Word LCDC_ADDRESS;

/*FF41 :
    Bit 6 - LYC=LY Coincidence Interrupt (1=Enable) (Read/Write)
    Bit 5 - Mode 2 OAM Interrupt         (1=Enable) (Read/Write)
    Bit 4 - Mode 1 V-Blank Interrupt     (1=Enable) (Read/Write)
    Bit 3 - Mode 0 H-Blank Interrupt     (1=Enable) (Read/Write)
    Bit 2 - Coincidence Flag  (0:LYC<>LY, 1:LYC=LY) (Read Only)
    Bit 1-0 - Mode Flag       
           0: During H-Blank
           1: During V-Blank
           2: During Searching OAM
           3: During Transferring Data to LCD Driver*/
extern const Word STAT_ADDRESS;

/*FF42 - SCY - Scroll Y     FF43 - SCX - Scroll X 

Specifies the position in the 256x256 pixels BG map (32x32 tiles) which is to be displayed at the upper/left LCD display position.

Values in range from 0-255 may be used for X/Y each

The video controller automatically wraps back to the upper (left)position in BG map when drawing exceeds the lower (right) border of the BG map area.*/
extern const Word SCY_ADDRESS,SCX_ADDRESS;

/* FF44 - LY - LCDC Y-Coordinate (R)
The LY indicates the vertical line to which the present data is transferred to the LCD Driver. 

The LY can take on any value between 0 through 153. 

The values between 144 and 153 indicate the V-Blank period. 

Writing will reset the counter.*/
extern const Word LY_ADDRESS;
/*
FF45 - LYC - LY Compare (R/W) 
The gameboy permanently compares the value of the LYC and LY 

 When both values are identical, the coincident bit in the STAT Bit2 becomes set
*/
extern const Word LYC_ADDRESS;

/*
FF47 - BGP - BG Palette Data (R/W) 
Bit 7-6 - Shade for Color Number 3 
Bit 5-4 - Shade for Color Number 2
Bit 3-2 - Shade for Color Number 1 
Bit 1-0 - Shade for Color Number 0
*/
extern const Word BGP_ADDRESS;
/*FF48 - OBP0 - Object Palette 0 Data (R/W)

 This register assigns gray shades for sprite palette 0. 
 It works exactly as BGP (FF47).

 Except that the lowest bits aren't used ,sprite data 00 is transparent
*/
extern const Word OBP0_ADDRESS;
/*FF49 - OBP1 - Object Palette 1 Data (R/W)

 This register assigns gray shades for sprite palette 1. 
 It works exactly as BGP (FF47).

 Except that the lowest bits aren't used ,sprite data 00 is transparent
*/
extern const Word OBP1_ADDRESS;
/*
FF4A - WY - Window Y Position (R/W) 
FF4B - WX - Window X Position minus 7 (R/W) 

Specifies the upper/left positions of the Window area. 
(

    The window is an alternate background area which can be displayed above of the normal background. 
    OBJs (sprites) may be still displayed above or behinf the window, just as for normal BG.
) 

The window becomes visible (if enabled) when positions are set in range WX=0-166, WY=0-143. 

A postion of WX=7, WY=0 locates the window at upper left, it is then completly covering normal background.
*/
extern const Word WY_ADDRESS, WX_ADDRESS;

extern const Word IE_ADDRESS;
/*
FF0F:
    Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
    Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
    Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
    Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
    Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)
*/
extern const Word IF_ADDRESS;


extern const int FLAG_ZERO;
extern const int FLAG_SUBTRACT;
extern const int FLAG_HALFCARRY;
extern const int FLAG_CARRY;