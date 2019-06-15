#pragma once
#include <string>
#include <array>
#include "const.h"
#include "common.h"
#include "SDL.h"
#include "AddressSpace.h"
#include "Logger.h"
//todo :
// change the var name
// complete the merge  from gpu
class GPU : public AddressSpace
{

public:
    /***********************************************/
    //this is the certain format of the addressSpace
    bool accepts(Word address) override;
    Byte getByte(Word address) override;
    void setByte(Word address, Byte value) override;
    void display();
    //it's the main cycle of the gpu
    void addTime(int clock);
    //choose the mode and the status matched with the mode

    //some var used to Scroll
    int currentLine = 0, currentMode = 0, counter = 0;
    //two byte to store joypad information
    //judge if it's direction or select

private:
    void setMode(Byte mode);

    void doDMA(Byte dma);
    void drawBg(uint32_t*, Byte bgWinDataLow, Byte bgMapHigh);
    void drawSprite(uint32_t*, Byte spriteLarge);
    void draw(int);
    //to compare the 0xff44 0xff45 to judge if it's a interrupt
    void setLCYInterrupt();

    //some sdl var and pointer:

    Byte getGrayCode(Byte colorCode, Byte reg){
        return static_cast<Byte>((reg >> (colorCode << 1)) & 0x03);
    }
    //the window info:

    int innerClock = 0;
    //color map:

    //some const mode number:

    const static int MODE_OAM = 2;
    const static int MODE_VRAM = 3;
    const static int MODE_HBLANK = 0;
    const static int MODE_VBLANK = 1;
    //about the ram of the video and sprite:

    const static Word offsetVram = 0x8000;
    const static Word lengthVram = 0x9800 - 0x8000;

    const static Word offsetChr = 0x9800;
    const static Word lengthChr = 0xA000 - 0x9800;

    const static Word offsetOam = 0xFE00;
    const static Word lengthOam = 0xFEA0 - 0xFE00;
    //the reg in the gpu:

    Byte regLcdControl = 0x91;
    Byte regLcdStatus = 0;

    Byte regDMA;
    Byte regBGP = 0xE4;
    Byte regOBP0 = 0xE4;
    Byte regOBP1 = 0xE4;
    Byte regScrollX = 0;
    Byte regScrollY = 0;
    Byte regLineY = 0;
    Byte regLineYC = 0;
    Byte regWindowX = 0;
    Byte regWindowY = 0;
    //tooo about interrupt:

    void doDMA(){
        //0xA0Bytes
    }

    std::array<Byte, lengthVram> bytesVram{};
    std::array<Byte, lengthChr> bytesChr{};
    std::array<Byte, lengthOam> bytesOam{};
};
extern GPU gpu;
