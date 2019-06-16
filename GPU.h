// gpu, manage graphic
#pragma once
#include <array>
#include "const.h"
#include "common.h"
#include "AddressSpace.h"
#include "Logger.h"
//
// change the var name
class GPU : public AddressSpace
{

public:
    /***********************************************/
    //this is the certain format of the addressSpace
    void addTime(int clock);
    void init(bool useSprite);

    bool accepts(Word address) override;
    Byte getByte(Word address) override;
    void setByte(Word address, Byte value) override;
    static GPU* getGPU(){
        static GPU gpu;
        return &gpu;
    }

    //it's the main cycle of the gpu

    //choose the mode and the status matched with the mode

    //some var used to Scroll
    //two byte to store joypad information
    //judge if it's direction or select
protected:
    GPU(){};
private:
    void setMode(Byte mode);

    void doDMA(Byte dma);
    void drawBg(uint32_t*, Byte bgWinDataLow, Byte bgMapHigh);
    void drawSprite(uint32_t*, Byte spriteLarge);
    void draw(int);
    //to compare the 0xff44 0xff45 to judge if it's a interrupt
    static inline Byte getGrayCode(Byte colorCode, Byte reg);
    Byte currentMode = 0;

    //some const mode number:
    uint64_t innerClock = 0;
    bool useSprite = true;
    const static int MODE_HBLANK = 0;
    const static int MODE_VBLANK = 1;
    const static int MODE_OAM = 2;
    const static int MODE_VRAM = 3;


    //the reg in the gpu:
    Byte regLcdControl = 0x91;
    Byte regLcdStatus = 0;
    Byte regDMA = 0;
    Byte regBGP = 0xE4;
    Byte regOBP0 = 0xE4;
    Byte regOBP1 = 0xE4;
    Byte regScrollX = 0;
    Byte regScrollY = 0;
    Byte regLineY = 0;
    Byte regLineYC = 0;
    Byte regWindowX = 0;
    Byte regWindowY = 0;

    //about the ram of the video and sprite:
    const static Word offsetVram = 0x8000;
    const static Word lengthVram = 0x9800 - 0x8000;

    const static Word offsetChr = 0x9800;
    const static Word lengthChr = 0xA000 - 0x9800;

    const static Word offsetOam = 0xFE00;
    const static Word lengthOam = 0xFEA0 - 0xFE00;

    std::array<Byte, lengthVram> bytesVRam{};
    std::array<Byte, lengthChr> bytesChr{};
    std::array<Byte, lengthOam> bytesOam{};

#ifndef NLOG
    void display();
#endif
};

