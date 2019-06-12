//
// Created by dell on 2019/6/6.
//

#include "SDLManager.h"
#include "Logger.h"

const Color realColorMap[4]{
        {0xFF, 0xFF, 0xFF},
        {0xAA, 0xAA, 0xAA},
        {0x55, 0x55, 0x55},
        {0x00, 0x00, 0x00}
};


SDLManager sdlManager;
void SDLManager::refreshWindow(){
#ifndef NDEBUG
    logger << "REFRESH!" << std::endl;
#else
    SDL_Delay(1);
#endif
    SDL_UpdateWindowSurface(win);

}

SDLManager::~SDLManager(){
    SDL_FreeFormat(fmt);
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

Uint32 SDLManager::mapColor(const Color &color) {
    return SDL_MapRGB(fmt, color.r, color.g, color.b);
}

void SDLManager::setLine(Byte lineNum, Uint32 *line) {
    SDL_LockSurface(surface);
    //the pixel matrix
    //note: nut for 1 level array
    auto pixels = static_cast<Uint32 *>(surface->pixels);
    std::copy(line, line+160, pixels + lineNum * 160);
    SDL_UnlockSurface(surface);
}

void SDLManager::init(std::string title_window) {
    int initRes = SDL_Init(SDL_INIT_EVERYTHING);
    if (initRes < 0){
        throw SDLException("init");
    }
    win = SDL_CreateWindow(title_window.c_str(), WINDOW_POSITION_X, WINDOW_POSITION_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

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

