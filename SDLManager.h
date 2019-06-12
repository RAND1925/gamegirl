//
// Created by dell on 2019/6/6.
//

#ifndef GAMEGIRL_SDLMANAGER_H
#define GAMEGIRL_SDLMANAGER_H

#include <string>
#include "Exceptions.h"
#include "SDL.h"

struct Color{
    Byte r,g,b;
};
extern const Color realColorMap[4];

class SDLManager {
    SDL_Window *win;
    SDL_Surface *surface;
    SDL_PixelFormat* fmt;
    SDL_Event e;
    const static int WINDOW_HEIGHT = 144;
    const static int WINDOW_WIDTH = 160;
    const static int BUFFER_HEIGHT = 256;
    const static int BUFFER_WIDTH = 256;
    const static int WINDOW_POSITION_X = SDL_WINDOWPOS_UNDEFINED;
    const static int WINDOW_POSITION_Y = SDL_WINDOWPOS_UNDEFINED;

public:
    ~SDLManager();
    void init();
    void loadGame( std::string title_window)
    {


    }

    void refreshWindow();
    Uint32 mapColor(const Color& color){
        return SDL_MapRGB(fmt, color.r, color.g, color.b);
    };
    void setLine(Byte lineNum, Uint32 * line)
    {
        SDL_LockSurface(surface);
        //the pixel matrix
        //note: nut for 1 level array
        auto pixels = static_cast<Uint32 *>(surface->pixels);
        std::copy(line, line+160, pixels + lineNum * 160);
        SDL_UnlockSurface(surface);
    }
    /*
    Byte joypadC1;
    Byte joypadC0;
    bool getJoypad() {
        bool isQuit = false;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                isQuit = true;
            if (e.type == SDL_KEYDOWN) {
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
                //  interruptJoypad();
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
                }
            }
        }
        return !isQuit;
        //the return value is used to determine if it's end
    }
    */

};
extern SDLManager sdlManager;

#endif //GAMEGIRL_SDLMANAGER_H
