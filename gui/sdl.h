#include <SDL2/SDL.h>
#include <string>
#include "Mem.h"
#include "common.h"

class windows
{
public:
    void initWindow(int WINDOW_WIDTH, int WINDOW_HEIGHT, int pos_x, int pos_Y, std::string title_window);

private:
    SDL_Window *win;
    SDL_Surface *surface;
    const int window_width;
    const int window_height;
};