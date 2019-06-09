#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "MMU.h"
#include "const.h"
#include "common.h"
#include "AddressSpace.h"
#include "InterruptManager.h"
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
        else if (address >= offsetOam && address < offsetOam + lengthOam)
            return true;
        else if (address >= 0xFF40 && address <= 0xFF4B)
            return true;
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
                return regScrollX;
            case 0xFF43:
                return regScrollY;
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
    void setByte(Word address, Byte value) override
    {
        if (address == 0xFF00)
            keyColumn = value & 0x30; //get the high 4 bit of the value
        else if (address >= offsetVram && address < offsetVram + lengthVram)
        {
            bytesVram[address - offsetVram] = value;
        }
        else if (address >= offsetOam && address < offsetOam + lengthOam)
            bytesOam[address - offsetOam] = value;
        else
        {
            switch (address)
            {
            case 0xFF40:
                regLcdControl = value;
            case 0xFF41:
                regLcdStatus = value;
            case 0xFF42:
                regScrollX = value;
            case 0xFF43:
                regScrollY = value;
            case 0xFF44:
                regLineY = 0; //warn
            case 0xFF45:
                regLineYC = value;
            case 0xFF4A:
                regWindowX = value;
            case 0xFF4B:
                regWindowY = value;
            default:
                break;
            }
        }
    }
    //OAM  FE00-FE9F
    class SpriteInfo
    {
    public:
        Byte y;

        Byte x{0};

        Byte tile{0};

        Byte flags{0};

        SpriteInfo(int id);
    };
    /***********************************************/
    //these is the func used in software windows

    //create the window and init the private var
    void initWindow(int height, int width,
                    int pos_x, int pos_Y,
                    std::string title_window);
    //set the certain pixel's color
    void setPixelColor(int pos_x, int pos_y, int color);
    //fersh the windows
    void fresh() { SDL_UpdateWindowSurface(win); }
    //destory the surface
    void end();

    /***********************************************/

    //it's the main cycle of the gpu
    void addTime(int clock);
    //choose the mode and the status matched with the mode
    void setMode(int mode);
    //get the input and judge if it's quit or not
    bool getJoypad();
    std::vector<SpriteInfo> getSprites(int yline);
    //draw the video ram and set it tohe surface windows
    void draw(int yLine);
    //set the joypad interrupt
    void interruptJoypad();
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

    SDL_Window *win;
    SDL_Surface *surface;
    SDL_Event e;
    //the window info:

    int windowWidth, windowHeight;
    //clock counter:

    int inerClock = 0;
    //color map:

    const int colorMap[4] = {255, 200, 100, 0};
    //some const mode number:

    const static int MODE_OAM = 2;
    const static int MODE_VRAM = 3;
    const static int MODE_HBLANK = 0;
    const static int MODE_VBLANK = 1;
    //about the ram of the video and sprite:

    const static Word offsetVram = 0x8000;
    const static Word lengthVram = 0x9FFF - 0x8000;

    const static Word offsetOam = 0xFE00;
    const static Word lengthOam = 0xFE9F - 0xFE00;
    //the reg in the gpu:

    Byte regLcdControl;
    Byte regLcdStatus;
    Byte regScrollX;
    Byte regScrollY;
    Byte regLineY;
    Byte regLineYC;
    Byte regWindowX;
    Byte regWindowY;
    //tooo about interrupt:

    std::array<Byte, lengthVram> bytesVram;
    std::array<Byte, lengthOam> bytesOam;
};
extern GPU gpu;
