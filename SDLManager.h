// split sdl part from others
// Created by dell on 2019/6/6.
//

#ifndef GAMEGIRL_SDLMANAGER_H
#define GAMEGIRL_SDLMANAGER_H

#include <string>
#include <fstream>
#include "Exceptions.h"
extern "C" {
    #include "SDL.h"
}

#include "common.h"

struct Color{
    Byte r, g, b, a;
};
extern const Color realColorMap[4];

class SDLManager {
    SDL_Window *win{nullptr};
    SDL_Surface *surface{nullptr};
    SDL_PixelFormat* fmt{nullptr};
    SDL_Event e{};

    const static int WINDOW_HEIGHT = 144;
    const static int WINDOW_WIDTH = 160;
    const static int BUFFER_HEIGHT = 256;
    const static int BUFFER_WIDTH = 256;
    int xPos = SDL_WINDOWPOS_UNDEFINED;
    int yPos = SDL_WINDOWPOS_UNDEFINED;
    int zoomTime = 1;
    uint8_t fps = 60;
    Byte joypadC1 = 0x0F;
    Byte joypadC0 = 0x0F;
    bool isQuit = false;
    uint32_t tmp[WINDOW_HEIGHT * WINDOW_WIDTH]{};
    uint32_t fpsTimer = 0;

    std::ofstream p{"a.txt"};
public:

    void refreshWindow();
    void setLine(Byte lineNum, Uint32 * line);
    Uint32 mapColor(Byte);;
    Byte getJoypad(Byte in);
    bool handleInput ();
    ~SDLManager();
    void init(const std::string& title_window, int xPos, int zoomTime, int yPos, int fps);

};
extern SDLManager sdlManager;

#endif //GAMEGIRL_SDLMANAGER_H

