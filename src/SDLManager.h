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



class SDLManager {
public:

    void refreshWindow();
    void setLine(Byte lineNum, Uint32 * line);
    Uint32 mapColor(Byte);
    Byte getJoypad(Byte in);
    bool handleInput ();
    ~SDLManager();
    void init(const std::string& title_window, int zoomTime, int xPos, int yPos, int fps);
    void changeColor(int i);
    static SDLManager* getSDLManager();
protected:
    SDLManager()= default;
private:
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
    using Key = SDL_Keymod;
#define kA SDLK_j
#define kB SDLK_k
#define kSta SDLK_l
#define kSel SDLK_SEMICOLON
#define kU SDLK_w
#define kR SDLK_d
#define kD SDLK_s
#define kL SDLK_a

    int colorMapChoice = 0;
    int zoomTime = 1;
    uint8_t fps = 60;
    Byte joypadC1 = 0x0F;
    Byte joypadC0 = 0x0F;
    bool isQuit = false;
    uint32_t tmp[WINDOW_HEIGHT * WINDOW_WIDTH]{};
    uint32_t fpsTimer = 0;

};


#endif //GAMEGIRL_SDLMANAGER_H

