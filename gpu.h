#pragma once
#include "SDL.h"
#include <string>
#include <vector>
#include "MMU.h"
#include "const.h"
#include "common.h"
#include "AddressSpace.h"
#include "InterruptManager.h"
#include "SDLManager.h"
//todo :
// change the var name
// complete the merge  from gpu
class GPU : public AddressSpace
{

public:
    /***********************************************/
    //this is the certain format of the addressSpace
    bool accepts(Word address) override
    {
        if (address >= offsetVram && address < offsetVram + lengthVram)
            return true;
        else if (address >= offsetChr && address < offsetChr + lengthChr)
            return true;
        else if (address >= offsetOam && address < offsetOam + lengthOam)
            return true;
        else if (address >= 0xFF40 && address <= 0xFF45)
            return true;
        else if (address == 0xFF00 || address == 0xFF4A || address == 0xFF4B)
            return true;
        return false;
    }

    Byte getByte(Word address) override
    {
        if (address == 0xFF00)
        {
            if (keyColumn == 0x10)
                return joypadC0;
            else if (keyColumn == 0x20)
                return joypadC1;
        }
        else if (address >= offsetVram && address < offsetVram + lengthVram)
            return bytesVram[address - offsetVram];
        else if (address >= offsetChr && address < offsetChr + lengthChr)
            return bytesChr[address - offsetChr];
        else if (address >= offsetOam && address < offsetOam + lengthOam)
            return bytesOam[address - offsetOam];
        else
        {
            switch (address)
            {
                case 0xFF40:
                    return regLcdControl;
                case 0xFF41:
                    return regLcdStatus;
                case 0xFF42:
                    return regScrollY;
                case 0xFF43:
                    return regScrollX;
                case 0xFF44:
                    return regLineY;
                case 0xFF45:
                    return regLineYC;
                case 0xFF4A:
                    return regWindowX;
                case 0xFF4B:
                    return regWindowY;
                default:
                    break;
            }
        }
    }
    void setByte(Word address, Byte value) override {

        if (address == 0xFF00) {
            keyColumn = value & 0x30;
            return;//get the high 4 bit of the value
        } else if (address >= offsetVram && address < offsetVram + lengthVram) {
            bytesVram[address - offsetVram] = value;
            return;
        } else if (address >= offsetChr && address < offsetChr + lengthChr){
            bytesChr[address - offsetChr] = value;
            return;
        }
        else if (address >= offsetOam && address < offsetOam + lengthOam) {
            bytesOam[address - offsetOam] = value;
            return;
        }
        else
        {
            switch (address)
            {
                case 0xFF40:
                    regLcdControl = value;
                    return;
                case 0xFF41:
                    regLcdStatus = value;
                    return;
                case 0xFF42:
                    regScrollX = value;
                    return;
                case 0xFF43:
                    regScrollY = value;
                    return;
                case 0xFF44:
                    regLineY = 0; //warn
                    return;
                case 0xFF45:
                    regLineYC = value;
                    return;
                case 0xFF4A:
                    regWindowX = value;
                    return;
                case 0xFF4B:
                    regWindowY = value;
                    return;
                default:
                    break;
            }
        }
        throw WrongAddressException("GPU[write]", address);
    }

    /***********************************************/
    //these is the func used in software windows


    /***********************************************/

    //it's the main cycle of the gpu
    void addTime(int clock);
    //choose the mode and the status matched with the mode
    void setMode(int mode);

    void draw(int);
    //to compare the 0xff44 0xff45 to judge if it's a interrupt
    void setLCYInterrupt();
    /***********************************************/

    //these are some func used to do the IF and BitOperation
    void setBiT(Byte &byte, int pos, bool value)
    {
        byte ^= (-value ^ byte) & (1 << pos);
    }
    //request the IF
    //void setInterruptFlag(int bit, bool _bool);
    /***********************************************/

    //some var used to Scroll
    int currentLine = 0, currentMode = 0, counter = 0;
    //two byte to store joypad information
    Byte joypadC0 = 0x0F, joypadC1 = 0x0F;
    //judge if it's direction or select
    Byte keyColumn = 0x00;

private:
    //some sdl var and pointer:


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
    Byte regLcdStatus;
    Byte regScrollX = 0;
    Byte regScrollY = 0;
    Byte regLineY = 0;
    Byte regLineYC = 0;
    Byte regWindowX = 0;
    Byte regWindowY = 0;
    //tooo about interrupt:


    std::array<Byte, lengthVram> bytesVram{};
    std::array<Byte, lengthChr> bytesChr{};
    std::array<Byte, lengthOam> bytesOam{};
};
extern GPU gpu;
