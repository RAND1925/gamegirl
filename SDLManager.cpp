//
// Created by dell on 2019/6/6.
//

#include "SDLManager.h"

const Color realColorMap[4]{
        {0xFF, 0xFF, 0xFF},
        {0xAA, 0xAA, 0xAA},
        {0x55, 0x55, 0x55},
        {0x00, 0x00, 0x00}
};

SDLManager sdlManager;


void SDLManager::init(){

    int initRes = SDL_Init(SDL_INIT_EVERYTHING);
    if (initRes < 0){
        throw SDLException("init");
    }
    win = SDL_CreateWindow("test", WINDOW_POSITION_X, WINDOW_POSITION_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (win == nullptr){
        throw SDLException("window");
    }
    //about the clock to fresh

    //let *surface get the image from the *win
    surface = SDL_GetWindowSurface(win);
    //todo: set a flag pf Uint32 in surface
    //???
    //todo : fill the surface with some color
    fmt = surface->format;
    auto pixel_format = SDL_MapRGB(surface->format, 193, 0, 120);
    SDL_FillRect(surface, nullptr, pixel_format);
    refreshWindow();
};

void SDLManager::refreshWindow(){
    SDL_UpdateWindowSurface(win);
}

SDLManager::~SDLManager(){
    SDL_FreeFormat(fmt);
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(win);
    SDL_Quit();
}