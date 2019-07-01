//
// Created by dell on 2019/6/6.
//

#include "SDLManager.h"
#include "Logger.h"
struct Color{
    Byte r, g, b, a;
};
Color realColorMap[5][4]{{
         {0xFF, 0xFF, 0xFF, 0x00},
         {0xAA, 0xAA, 0xAA, 0x55},
         {0x55, 0x55, 0x55, 0xAA},
         {0x00, 0x00, 0x00, 0xFF}
     },
     {
         {0xFF, 0xFA, 0xCD, 0x00},
         {0xFF, 0xCA, 0x28, 0x55},
         {0xFF, 0x00, 0x00, 0xAA},
         {0x00, 0x00, 0x00, 0xFF}
     },
     {
         {0xE0, 0xFF, 0xFF, 0x00},
         {0x6B, 0xC2, 0x35, 0x55},
         {0x06, 0x80, 0x43, 0xAA},
         {0x00, 0x00, 0x00, 0xFF}
     },
     {
         {0xFF, 0xE1, 0xFF, 0x00},
         {0xFF, 0xBB, 0xFF, 0x55},
         {0xBB, 0xFF, 0xFF, 0xAA},
         {0x00, 0x00, 0x00, 0xFF}
     },
     {
         {0x00, 0x00, 0x00, 0x00},
         {0xFF, 0xCA, 0x28, 0x55},
         {0xEE, 0xEE, 0x00, 0xAA},
         {0xFF, 0xFF, 0xFF, 0xFF}
     }
};
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

    if (SDL_GetTicks() - fpsTimer < 1000 / fps)
    {
#ifndef NLOG
        logger << "frameTime:" << SDL_GetTicks() - fpsTimer << std::endl;
#endif
        SDL_Delay(1000 / fps - SDL_GetTicks() + fpsTimer);

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
    Color color = realColorMap[colorMapChoice][grayCode];
    return SDL_MapRGBA(fmt, color.r, color.g, color.b, color.a);
}

void SDLManager::setLine(Byte lineNum, Uint32 *line) {
    std::copy(line, line + 160, tmp + lineNum * 160);
}


bool SDLManager::handleInput() {
    while (SDL_PollEvent(&e)) {
#ifndef NLOG
        logger << "input:" << e.type<< std::endl;
#endif
        if (e.type == SDL_QUIT)
            return true;
        if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    return true;
                    //for column 1
                case kR:
                    setBit(joypadC1, 0);
                    break;
                case kL:
                    setBit(joypadC1, 1);
                    break;
                case kU:
                    setBit(joypadC1, 2);
                    break;
                case kD:
                    setBit(joypadC1, 3);
                    break;

                    //for column 0
                case kA:
                    setBit(joypadC0, 0);
                    break;
                case kB:
                    setBit(joypadC0, 1);
                    break;
                case kSel:
                    setBit(joypadC0, 2);
                    break;
                case kSta:
                    setBit(joypadC0, 3);
                    break;
            }
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    return true;
                //for column 1
                case kR:
                    resetBit(joypadC1, 0);
                    break;
                case kL:
                    resetBit(joypadC1, 1);
                    break;
                case kU:
                    resetBit(joypadC1, 2);
                    break;
                case kD:
                    resetBit(joypadC1, 3);
                    break;

                    //for column 0
                case kA:
                    resetBit(joypadC0, 0);
                    break;
                case kB:
                    resetBit(joypadC0, 1);
                    break;
                case kSel:
                    resetBit(joypadC0, 2);
                    break;
                case kSta:
                    resetBit(joypadC0, 3);
                    break;
                default:
                    break;
            }
        } else {
            continue;
        }
    }
    return isQuit;
    //the return value is used to determine if it's end
}

Byte SDLManager::getJoypad(Byte in) {
    in &= 0xF0u;
    if (!getBit(in, 4)){
        in |= (joypadC1 & 0x0Fu);
    }
    if (!getBit(in, 5)){
        in |= (joypadC0 & 0x0Fu);
    }
    return in;
}

void SDLManager::changeColor(int choice = 0) {
    colorMapChoice = choice;
}
void SDLManager::init(const std::string &title_window, int zoomTime, int xPos, int yPos, int fps) {

    this->zoomTime = zoomTime;
    this->xPos = xPos;
    this->yPos = yPos;
    this->fps = fps;
    this->zoomTime = zoomTime;
    int initRes = SDL_Init(SDL_INIT_EVERYTHING);
    if (initRes < 0){
        throw SDLException("init");
    }
    win = SDL_CreateWindow(title_window.c_str(), xPos, yPos, WINDOW_WIDTH * zoomTime,WINDOW_HEIGHT * zoomTime, SDL_WINDOW_SHOWN);
    if (win == nullptr){
        throw SDLException("window create");
    }
    surface = SDL_GetWindowSurface(win);
    //todo: set a flag pf Uint32 in surface
    //???
    fmt = surface->format;
    auto pixel_format = SDL_MapRGBA(surface->format, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_FillRect(surface, nullptr, pixel_format);
    SDL_UpdateWindowSurface(win);
}

SDLManager *SDLManager::getSDLManager() {
    static SDLManager sdlManager;
    return &sdlManager;
};

