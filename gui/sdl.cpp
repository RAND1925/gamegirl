#include <string>
#include <iostream>
#include "common.h"
#include "sdl.h"
void windows::initWindow(int WINDOW_WIDTH, int WINDOW_HEIGHT, int pos_x, int pos_y, std::string title_window)
{
    //init the sdl system
    SDL_Init(SDL_INIT_VIDEO);
    //init the window  to use
    win = SDL_CreateWindow(title_window.c_str(), pos_x, pos_y, WINDOW_WIDTH, WINDOW_WIDTH, SDL_WINDOW_SHOWN);
    //init the var of width and height
    window_width = WINDOW_WIDTH;
    ;
    window_height = WINDOW_HEIGHT;

    //about the clock to fresh

    //let *surface get the image from the *win
    surface = SDL_GetWindowSurface(win);
    //todo: set a flag pf Uint32 in surface
    //???
    //todo : fill the surface with some color
    auto pixel_format = SDL_MapRGB(surface->format, 193, 0, 120);
    SDL_FillRect(surface, NULL, pixel_format);
}
void windows::end()
{
    SDL_FreeSurface(surface);
    SDL_Quit();
}
void windows::setPixelColor(int pos_x, int pos_y, int color)
{
    SDL_UnlockSurface(surface);
    //the pixel matrix 
    //note: nut for 1 level array
    Uint32 *pixels = (Uint32 *)surface->pixels;
    //note: why same color data
    auto pixel_format = surface->format;
    pixels[pos_y*160+pos_x]=SDL_MapRGB(pixel_format,color,color,color);
}