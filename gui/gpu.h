#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "const.h"
#include "../common.h"
#include "../MMU.h"
#include "../AddressSpace.h"

//todo :
// change the var name
// complete the merge  from gpu
//template <Word offset, Word length>
class GPU : public AddressSpace
{
    std::array<Byte, length> bytes;

public:
    /***********************************************/
    //this is the certain format of the addressSpace
    //std::vector<Byte> registerGPU;
    bool accepts(Word address) override
    {
        //ff44
        return (address >= offset && address < offset + length);
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
        else
            return bytes[address - offset];
    }
    void setByte(Word address, Byte value) override
    {
        if (address == 0xFF00)
            keyColumn = value &= 0x30; //get the high 4 bit of the value
        else
            bytes[address - offset] = value;
    }
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
    //draw the video ram and set it tohe surface windows
    void draw(int yLine);
    //set the joypad interrupt
    void interruptJoypad();
    //to compare the 0xff44 0xff45 to judge if it's a interrupt
    void setLCYInterrupt();
    /***********************************************/
    //these are some func used to do the IF and BitOperation

    //request the IF
    void setInterruptFlag(int bit, bool _bool);
    //set certain pos in &byte
    void setBit(Byte &byte, int pos, bool value)
    {
        byte ^= (-value ^ byte) & (1 << pos);
    }
    //get certain pos in &byte
    bool getBit(int pos, Byte byte) { return (byte >> pos) & 1; }

    /***********************************************/

    //some var used to scan
    int currentLine = 0;
    int currentMode = 0;
    int counter = 0;
    //two byte to store joypad information
    Byte joypadC0 = 0x0F;
    Byte joypadC1 = 0x0F;
    //judge if it's direction or select
    Byte keyColumn = 0x00;

private:
    SDL_Window *win;
    SDL_Surface *surface;
    int windowWidth;
    int windowHeight;
    int inerClock = 0;
    const int colorMap[4]={255,200,100,0};
    
};
