#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "const.h"
#include "../common.h"
#include "../MMU.h"
#include "../AddressSpace.h"

template <Word offset, Word length>
class GPU : public AddressSpace
{
public:
    std::array<Byte, length> bytes;
    //std::vector<Byte> registerGPU;
    bool accepts(Word address) override
    {
        //ff44
        return (address >= offset && address < offset + length) || (address == 0xFF44);
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

    //create the window and init the private var
    void initWindow(int height, int width,
                    int pos_x, int pos_Y,
                    std::string title_window);

    //destory the surface
    void end();

    //set the certain pixel's color
    void setPixelColor(int pos_x, int pos_y, int color);

    //it's the main cycle of the gpu
    void addTime(int clock);

    //choose the mode and the status matched with the mode
    void setMode(int mode);

    //fersh the windows
    void fresh() { SDL_UpdateWindowSurface(win); };

    //to compare the 0xff44 0xff45 to judge if it's a interrupt
    void setLCYStatus();

    /*Bit 7 - Not used        Bit 6 - Not used
    Bit 5 - P15 Select Button keys (0=Select)
    Bit 4 - P14 Select Direction keys (0=Select) 
    Bit 3 - P13 Input Down  or Start(0=Pressed) (Read Only)
    Bit 2 - P12 Input Up    or Select (0=Pressed) (Read Only) 
    Bit 1 - P11 Input Left  or Button B (0=Pressed) (Read Only) 
    Bit 0 - P10 Input Right or Button A (0=Pressed) (Read Only)
    
    8bit: 0000 | 0000 

    column 1 direction ->0001 | xxxx
    column 0 select    ->0010 | xxxx
    */
    bool getJoypad();

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
    const int MODE_VRAM = 3;
    const int MODE_OAM = 2;
    const int MODE_HBLANK = 0;
    const int MODE_VBLANK = 1;
};
