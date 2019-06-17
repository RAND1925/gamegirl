//
// Created by dell on 2019/6/6.
//

#include "SDLManager.h"
#include "Logger.h"
Color realColorMap[4]{
        {0xFF, 0xFF, 0xFF, 0x00},
        {0xAA, 0xAA, 0xAA, 0x55},
        {0x55, 0x55, 0x55, 0xAA},
        {0x00, 0x00, 0x00, 0xFF}
};
Color coleredMap1[4] = {
        {0xFF, 0xFA, 0xCD, 0x00},
        {0xFF, 0xCA, 0x28, 0x55},
        {0xFF, 0x00, 0x00, 0xAA},
        {0x00, 0x00, 0x00, 0xFF}
};
Color coleredMap2[4] = {
        {0xE0, 0xFF, 0xFF, 0x00},
        {107, 194, 53, 0x55},
        {6, 128, 67, 0xAA},
        {0x00, 0x00, 0x00, 0xFF}
};
Color coleredMap3[4] = {
        {0xFF, 0xE1, 0xFF, 0x00},
        {0xFF, 0xBB, 0xFF, 0x55},
        {0xBB, 0xFF, 0xFF, 0xAA},
        {0x00, 0x00, 0x00, 0xFF}
};
Color coleredMap4[4] = {
        {0x00, 0x00, 0x00, 0x00},
        {0xFF, 0xCA, 0x28, 0x55},
        {0xEE, 0xEE, 0x00, 0xAA},
        {0xFF, 0xFF, 0xFF, 0xFF}
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
    //    SDL_Delay(1000 / fps - SDL_GetTicks() + fpsTimer);

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
}

void SDLManager::changeColor(int choice) {
    switch (choice) {
        case 1: {
            for (int i = 0; i < 4; i++) {
                realColorMap[i] = coleredMap1[i];
            }
            break;
        }
        case 2:{
            for (int i = 0; i < 4; i++) {
                realColorMap[i] = coleredMap2[i];
            }
            break;
        }
        case 3:{
            for (int i = 0; i < 4; i++) {
                realColorMap[i] = coleredMap3[i];
            }
            break;
        }
        case 4:{
            for (int i = 0; i < 4; i++) {
                realColorMap[i] = coleredMap4[i];
            }
            break;
        }
        default:
            break;
    }
}
void SDLManager::init(const std::string &title_window, int zoomTime, int xPos, int yPos, int fps) {

    this->zoomTime = zoomTime;
    this->xPos = xPos;
    this->yPos = yPos;
    this->fps = fps;this->zoomTime = zoomTime;
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

