//
// Created by dell on 2019/6/6.
//

#include "SDLManager.h"
#include "Logger.h"
const Color realColorMap[4]{
        {0xFF, 0xFF, 0xFF, 0x00},
        {0xAA, 0xAA, 0xAA, 0x55},
        {0x55, 0x55, 0x55, 0xAA},
        {0x00, 0x00, 0x00, 0xFF}
};

SDLManager sdlManager;
void SDLManager::refreshWindow(){
#ifndef NLOG
    logger << "REFRESH!" << std::endl;
#endif
    SDL_LockSurface(surface);
    uint32_t * pixels = (uint32_t*)surface->pixels;
    if (zoomTime == 1){
        std::copy(tmp, tmp + 160 * 144, pixels);
    } else{
        for(Byte i = 0; i < 144; ++i){

            uint32_t * lineBegin = pixels + i * 160 * zoomTime * zoomTime;
            for(Byte j = 0; j < 160; ++j){
                uint32_t *blockBegin = lineBegin + j * zoomTime;
                for(Byte k = 0; k < zoomTime; ++k){
                    blockBegin[k] = tmp[i * 160 + j];
                }
            }
            for (Byte m = 1; m < zoomTime; ++m){
                std::copy(lineBegin, lineBegin + zoomTime * 160, lineBegin + m * zoomTime * 160);
            }
        }
    }
    SDL_UnlockSurface(surface);
    SDL_UpdateWindowSurface(win);
    if (SDL_GetTicks() - fpsTimer < 1000 / FPS)
    {
        SDL_Delay(1000 / FPS - SDL_GetTicks() + fpsTimer);
    }
    fpsTimer = SDL_GetTicks();
}

SDLManager::~SDLManager(){
    SDL_FreeFormat(fmt);
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

Uint32 SDLManager::mapColor(Byte grayCode) {
    Color color = realColorMap[grayCode];
    return SDL_MapRGBA(fmt, color.r, color.g, color.b, color.a);
}

void SDLManager::setLine(Byte lineNum, Uint32 *line) {
    std::copy(line, line + 160, tmp + lineNum * 160);
}

void SDLManager::init(std::string title_window,int zoomTime) {
    this->zoomTime = zoomTime;
    int initRes = SDL_Init(SDL_INIT_EVERYTHING);
    if (initRes < 0){
        throw SDLException("init");
    }
    win = SDL_CreateWindow(title_window.c_str(), WINDOW_POSITION_X , WINDOW_POSITION_Y, WINDOW_WIDTH * zoomTime,WINDOW_HEIGHT * zoomTime, SDL_WINDOW_SHOWN);

    if (win == nullptr){
        throw SDLException("window");
    }
    //about the clock to fresh

    //let *surface get the image from the *win
    surface = SDL_GetWindowSurface(win);
    //todo: set a flag pf Uint32 in surface
    //???
    fmt = surface->format;
    auto pixel_format = SDL_MapRGB(surface->format, 193, 0, 120);
    SDL_FillRect(surface, nullptr, pixel_format);
    refreshWindow();
}

bool SDLManager::handleInput() {
    while (SDL_PollEvent(&e)) {
#ifndef NLOG
        logger << "input:" << e.type<< std::endl;
#endif
        if (e.type == SDL_QUIT)
            isQuit = true;
        if (e.type == SDL_KEYDOWN) {
#ifndef NLOG
            logger << "KEY:" << e.key.keysym.sym << std::endl;
#endif
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    isQuit = true;
                    break;
                    //for column 1
                case SDLK_RIGHT:
                    joypadC1 &= 0xE;
                    break;
                case SDLK_LEFT:
                    joypadC1 &= 0xD;
                    break;
                case SDLK_UP:
                    joypadC1 &= 0xB;
                    break;
                case SDLK_DOWN:
                    joypadC1 &= 0x7;
                    break;

                    //for column 0
                case SDLK_z:
                    joypadC0 &= 0xE;
                    break;
                case SDLK_x:
                    joypadC0 &= 0xD;
                    break;
                case SDLK_SPACE:
                    joypadC0 &= 0xB;
                    break;
                case SDLK_RETURN:
                    joypadC0 &= 0x7;
                    break;
            }
        } else if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
                //for column 1
                case SDLK_RIGHT:
                    joypadC1 |= 0x1;
                    break;
                case SDLK_LEFT:
                    joypadC1 |= 0x2;
                    break;
                case SDLK_UP:
                    joypadC1 |= 0x4;
                    break;
                case SDLK_DOWN:
                    joypadC1 |= 0x8;
                    break;

                    //for column 0
                case SDLK_z:
                    joypadC0 |= 0x1;
                    break;
                case SDLK_x:
                    joypadC0 |= 0x2;
                    break;
                case SDLK_SPACE:
                    joypadC0 |= 0x4;
                    break;
                case SDLK_RETURN:
                    joypadC0 |= 0x8;
                    break;
                default:
                    break;
            }

        } else {
            break;
        }
    }
    return isQuit;
    //the return value is used to determine if it's end
}

Byte SDLManager::getJoypad(Byte in) {
    in &= 0xF0;
    if (!getBit(in, 4)){
        in |= joypadC1;
    }
    if (!getBit(in, 5)){
        in |= joypadC0;
    }
    return in;
};

