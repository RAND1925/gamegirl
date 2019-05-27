#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "const.h"
#include"../ZRam.h"
#include "../common.h"

class windows
{
public:
    //create the window and init the private var
    void initWindow(int WINDOW_WIDTH, int WINDOW_HEIGHT, int pos_x, int pos_Y, std::string title_window);
    //destory the surface
    void end();
    //set the certain pixel's color
    void setPixelColor(int pos_x, int pos_y, int color);
    //it's the main cycle of the gpu
    void addTime(int clock);
    //choose the mode and the status matched with the mode
    void setMode(int mode);
    //fersh the windows 
    void fresh(){SDL_UpdateWindowSurface(win)};
    //to compare the 0xff44 0xff45 to judge if it's a interrupt
    void setLCYStatus();


    /*
    Bit 7 - Not used        Bit 6 - Not used
    Bit 5 - P15 Select Button Keys (0=Select)
    Bit 4 - P14 Select Direction Keys (0=Select) 
    Bit 3 - P13 Input Down  or Start(0=Pressed) (Read Only)
    Bit 2 - P12 Input Up    or Select (0=Pressed) (Read Only) 
    Bit 1 - P11 Input Left  or Button B (0=Pressed) (Read Only) 
    Bit 0 - P10 Input Right or Button A (0=Pressed) (Read Only)
    
    8bit: 0000 | 0000 

    column 1 direction ->0001 | xxxx
    column 0 select    ->0010 | xxxx
    */
    bool getJoypad();

    int CurrentLine = 0;
    int CurrentMode = 0;
    int counter = 0;

private:
    SDL_Window *win;
    SDL_Surface *surface;
    int window_width;
    int window_height;
    int InerClock = 0;
};