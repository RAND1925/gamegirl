#include <string>
#include <iostream>
#include "gpu.h"

bool GPU::getBit(int pos, Byte &byte)
{
    return (byte >> pos) & 1;
}
void GPU::addTime(int clock)
{
    setInterruptFlag();
    // check isLCDenabled if() acoordig to the oxff40
    Byte statusLCD=GRam::getByte(LCDC_ADDRESS);
    if (!getBit(7,statusLCD))
        return;
    //increase the gpu clock
    inerClock += clock;

    /*
        OAM -> VRAM -> HBlank -> VBlank
        Period	                GPU mode number	    Time spent (clocks)
        Scanline (accessing OAM)	2	                 80
        Scanline (accessing VRAM)	3	                172
        Horizontal blank	        0	                204

        One line (scan and blank)		                456
        Vertical blank	            1	                4560 (10 lines)
        Full frame (scans and vblank)		            70224
    */

    switch (currentMode)
    {
    case MODE_OAM:
        if (inerClock >= 79)
        {
            inerClock -= 79;
            setMode(MODE_VRAM);
        }

        break;
    case MODE_VRAM:

        if (inerClock >= 172)
        {
            inerClock -= 172;
            setMode(MODE_HBLANK);
        }
        break;
    case MODE_HBLANK:
        if (inerClock >= 205)
        {
            inerClock -= 205; //back to 0;
            //get the current line to draw
            Byte line_y = GRam::getByte(LY_ADDRESS);
            draw(line_y);

            ly++;
            GRam::setByte(LY_ADDRESS, ly);
            if (line_y >= 144) //call for the interrrput
            {
                setMode(MODE_VBLANK);

                fresh();
            }
            else
            {
                //less than 144 ,comtinue to get the line
                setMode(MODE_OAM);
            }
        }
        break;
    case MODE_VBLANK:
        //todo about the Vblank interrrupt
        Byte line_y = (inerClock / 456) + 144;
        if (inerClock >= 456 * 10)
        {
            //back to the oam,reset the stat
            setMode(MODE_OAM);
            inerClock -= 456 * 10;
            GRam::setByte(LY_ADDRESS, 0);
        }
        else
            GRam::setByte(LY_ADDRESS, line_y);
        break;
    default:
        break;
    }
    setLCYStatus();
}

void GPU::setMode(int mode)
{
    if (currentMode == mode)
        return;
    currentMode = mode;
    Byte statusLCDC = GRam::ReadByte(STAT_ADDRESS);

    //rese the 0bit and 1bit of the status
    //and 1111|1100
    statusLCDC &= 0xFC;
    //set the current mode
    statusLCDC |= currentMode;

    //todo call for interrupt
    Byte interruptflag =GRam::getByte(IF_ADDRESS);
}

void GPU::initWindow(int height, int width , int pos_x, int pos_y, std::string title_window)
{
    //init the sdl system
    SDL_Init(SDL_INIT_VIDEO);
    //init the window  to use
    win = SDL_CreateWindow(title_window.c_str(), pos_x, pos_y, width, height, SDL_WINDOW_SHOWN);
    //init the var of width and height
    windowWidth = width;
    
    windowheight = height;

    //about the clock to fresh

    //let *surface get the image from the *win
    surface = SDL_GetWindowSurface(win);
    //todo: set a flag pf Uint32 in surface
    //???
    //todo : fill the surface with some color
    auto pixel_format = SDL_MapRGB(surface->format, 193, 0, 120);
    SDL_FillRect(surface, NULL, pixel_format);
}

bool GPU::getJoypad()
{

    bool isQuit = false;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_Quit)
            isQuit == true;
        if (e.type == SDL_keyDOWN)
        {
            switch (e.key.keysym.sym)
            {
            //for column 1
            case SDLK_ESCAPE:
                isQuit = true;
                break;
            case SDLK_RIGHT:
                mmu::joypad_C1 &= 0xE;
                break;
            case SDLK_LEFT:
                GRam::jopad_C1 &= 0xD;
                break;
            case SDLK_UP:
                GRam::joypad_C1 &= 0xB;
                break;
            case SDLK_DOWN:
                GRam::joypad_C1 &= 0x7;
                break;

            //for column 0
            case SDLK_z:
                GRam::joypad_C0 &= 0xE;
                break;
            case SDLK_x:
                GRam::joypad_C0 &= 0xD;
                break;
            case SDLK_SPACE:
                GRam::joypad_C0 &= 0xB;
                break;
            case SDLK_RETURN;
                GRam::joypad_C0 &=0x7;
                break;
            }
        }
        else if (e.type == SDL_keyUP)
        {
            switch (e.key.keysym.sym)
            {
            //for column 1
            case SDLK_RIGHT:
                GRam::joypad_C1 |= 0x1;
                break;
            case SDLK_LEFT:
                GRam::joypad_C1 |= 0x2;
                break;
            case SDLK_UP:
                GRam::joypad_C1 |= 0x4;
                break;
            case SDLK_DOWN:
                GRam::joypad_C1 |= 0x8;
                break;

            //for column 0
            case SDLK_z:
                GRam::joypad_C0 |= 0x1;
                break;
            case SDLK_x:
                GRam::joypad_C0 |= 0x2;
                break;
            case SDLK_SPACE:
                GRam::joypad_C0 |= 0x4;
                break;
            case SDLK_RETURN;
                GRam::joypad_C0 |= 0x8;
                break;
            }
        }
        // for the interrupt
    }
    if(isQuit)
        return false;
    return true;
    //the return value is used to determine if it's end;
}

void GPU::setPixelColor(int pos_x, int pos_y, int color)
{
    SDL_UnlockSurface(surface);
    //the pixel matrix
    //note: nut for 1 level array
    Uint32 *pixels = (Uint32 *)surface->pixels;
    //note: why same color data
    auto pixelFormat = surface->format;
    pixels[pos_y * 160 + pos_x] = SDL_MapRGB(pixelFormat, color, color, color);
}
void GPU::end()
{
    SDL_FreeSurface(surface);
    SDL_Quit();
}