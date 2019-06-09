#include <string>
#include <iostream>
#include <algorithm>
#include "gpu.h"
GPU gpu;
void GPU::addTime(int clock)
{
    //reset the flag
    //setInterruptFlag(0, false);
    //setInterruptFlag(1, false);
    // check isLCDenabled if() acoordig to the oxff40
    Byte statusLCD =regLcdControl;
    if (!getBit(statusLCD,7 ))
        return;
    //increase the gpu clock
    inerClock += clock;

    /*
        OAM -> VRAM -> HBlank -> VBlank
        Period	                GPU mode number	    Time spent (clocks)
        Scanline (accessing OAM)	2	                 80
        Scanline (accessing VRAM)	3	                172
        Horizontal blank	        0	                204

        One line (scan and blank)		                456
        Vertical blank	            1	                4560 (10 lines)
        Full frame (scans and vblank)		            70224
    */

    switch (currentMode)
    {
    case MODE_OAM:
        if (inerClock >= 79)
        {
            inerClock -= 79;
            setMode(MODE_VRAM);
        }

        break;
    case MODE_VRAM:
        if (inerClock >= 172)
        {
            inerClock -= 172;
            setMode(MODE_HBLANK);
        }
        break;
    case MODE_HBLANK:
        if (inerClock >= 205)
        {
            inerClock -= 205; //back to 0;
            //get the current line to draw
            Byte yLine = regLineY;
            draw(yLine);
            yLine++;
            regLineY=yLine;
            if (yLine >= 144) //call for the interrrput
            {
                setMode(MODE_VBLANK);
                fresh();
            }
            else
            {
                //less than 144 ,comtinue to get the line
                setMode(MODE_OAM);
            }
        }
        break;
    case MODE_VBLANK:
        //todo about the Vblank interrrupt
        Byte yLine = (inerClock / 456) + 144;
        if (inerClock >= 456 * 10)
        {
            //back to the oam,reset the stat
            setMode(MODE_OAM);
            inerClock -= 456 * 10;
            regLineY=0;
            
        }
        else
            regLineY=yLine;
        break;
    }
    setLCYInterrupt();
}
void GPU::setMode(int mode)
{
    if (currentMode == mode)
        return;
    currentMode = mode;
    Byte statusLCDC = regLcdStatus;

    //rese the 0bit and 1bit of the status
    //and 1111|1100
    statusLCDC &= 0xFC;
    //set the current mode into bit0 bit1
    statusLCDC |= currentMode & 0x03;
    bool interruptFlag;
    switch (mode)
    {
    case MODE_VBLANK:
        if (getBit(statusLCDC,4))
            interruptFlag = true;
        break;
    case MODE_HBLANK:
        if (getBit(statusLCDC,3))
            interruptFlag = true;
        break;
    case MODE_OAM:
        if (getBit(statusLCDC,5))
            interruptFlag = true;
        break;
    default:
        break;
    }
    if (interruptFlag)
        interruptManager.requestInterrupt(1);
    if (mode == MODE_VBLANK)
        interruptManager.requestInterrupt(1);
}
void GPU::draw(int yLine)
{
    //check if it's in the real line
    if (yLine >= 144)
        return;
    Byte lcdc =regLcdControl;
    Byte xScroll =regScrollX;
    Byte yScroll = regScrollY;

    //to decide which mao tile to use
    bool flagTile = getBit(lcdc,4);
    //In the first case, patterns have signed numbers from 0 to 255
    //(i.e. pattern #0 lies at address $8000).
    // In the second case, patterns have signed numbers from -128 to 127 //(i.e. pattern #0 lies at address $9000).
    Byte dataTile, mapTile;
    if (flagTile)
        dataTile =(Byte) 0x9000;
    else
        dataTile =(Byte) 0x8000;

    //get the map
    if (getBit(lcdc,3))
        mapTile =(Byte) 0x9800;
    else
        mapTile =(Byte) 0x9C00;
    //check the display is enable or not
    if (getBit(lcdc,0))
    {
        int yDraw = (yLine + yScroll) % 256;

        //?????
        Byte colorA, colorB;

        int lastPixel = -1;
        for (size_t counter = 0; counter < 160; counter++)
        {
            int xDraw = (counter + xScroll) % 256;
            int xTile = xDraw / 8;
            int yTile = yDraw / 8;
            int xPixel = 8 - xDraw % 8 - 1;
            int yPixel = yDraw % 8;

            if (xTile != lastPixel) //a new tile
            {
                int numTile;
                if (flagTile)
                    numTile = bytesVram[mapTile + (yTile * 32) + xTile-offsetVram];
                else if (!flagTile)
                    numTile = (SByte)(bytesVram[mapTile + (yTile * 32) + xTile-offsetVram]);
                colorA = bytesVram[dataTile + (numTile * 16) + (yPixel * 2)-offsetVram];
                colorB = bytesVram[dataTile + (numTile * 16) + (yPixel * 2) + 1-offsetVram];
                lastPixel = xTile;
            }
            int color = (((colorA >> xPixel) & 1) << 1) | ((colorB >> xPixel) & 1);
            setPixelColor(counter, yLine, colorMap[color]);
        }
    }

    //put sprite ito draw
    std::vector<SpriteInfo> spriteData = getSprites(yLine);
    int size = spriteData.size();
    if (size >= 10)
        size = 10;
    for (int i = size - 1; i >= 0; i++)
    {
        SpriteInfo tempSprite = spriteData[i];
        int yPixel = yLine - tempSprite.y + 16;
        bool xReserve = getBit(tempSprite.flags,5);
        bool yReserve = getBit(tempSprite.flags,6);
        bool propirty = getBit(tempSprite.flags,7);
        Byte tileSprite = tempSprite.tile | 0x01;
        yPixel -= 8;
        if (yReserve)
            yPixel = 7 - yPixel;
        Byte colorA = bytesOam[0x8000 + tileSprite * 16 + yPixel * 2-offsetOam];
        Byte colorB = bytesOam[0x8000 + tileSprite * 16 + yPixel * 2 + 1-offsetOam];
        for (int x = 0; x < 8; ++x)
        {
            if (tempSprite.x + x - 8 < 0)
                continue;
            int xPixel = 8 - x % 8 - 1;
            if (xReserve)
                xPixel = 8 - xPixel - 1 ;
            int color = (((colorA >> xPixel) & 1) << 1) | ((colorB >> xPixel) & 1);
            setPixelColor(tempSprite.x + x - 8, yLine, colorMap[color]);
        }
    }
}
void GPU::initWindow(int height, int width, int pos_x, int pos_y, std::string title_window)
{
    //init the sdl system
    SDL_Init(SDL_INIT_VIDEO);
    //init the window  to use
    win = SDL_CreateWindow(title_window.c_str(), pos_x, pos_y, width, height, SDL_WINDOW_SHOWN);
    //init the var of width and height
    windowWidth = width;

    windowHeight = height;

    //about the clock to fresh

    //let *surface get the image from the *win
    surface = SDL_GetWindowSurface(win);
    //todo: set a flag pf Uint32 in surface
    //???
    //todo : fill the surface with some color
    auto pixel_format = SDL_MapRGB(surface->format, 193, 0, 120);
    SDL_FillRect(surface, NULL, pixel_format);
}
void GPU::end()
{
    SDL_FreeSurface(surface);
    SDL_Quit();
}
void GPU::interruptJoypad()
{
    bool isRequestIF;
    // player pressed button and programmer intersted in button
    if (keyColumn == 0x10 && joypadC0 != 0x0f)
        isRequestIF == true;
    // player pressed directional and programmer interested
    else if (keyColumn == 0x20 && joypadC1 != 0x0f)
        isRequestIF == true;
    //set the IF
    if (isRequestIF)
        interruptManager.requestInterrupt(4);
}

void GPU::setLCYInterrupt()
{
    Byte yLine = regLineY;
    Byte yLine_cp = regLineYC;
    Byte statusLCDC = regLcdStatus;
    //write to the stat bit 2
    setBiT(statusLCDC, 2, yLine == yLine_cp);
    regLcdStatus=statusLCDC;
    
}
/*void GPU::setInterruptFlag(int pos, bool _bool)
{
    Byte requestFlag = MMU::readByte(IF_ADDRESS);
    setBiT(requestFlag, pos, _bool);
    MMU::writeByte(IF_ADDRESS, requestFlag);
}*/
void GPU::setPixelColor(int pos_x, int pos_y, int color)
{
    SDL_UnlockSurface(surface);
    //the pixel matrix
    //note: nut for 1 level array
    Uint32 *pixels = (Uint32 *)surface->pixels;
    //note: why same color data
    auto pixelFormat = surface->format;
    pixels[pos_y * 160 + pos_x] = SDL_MapRGB(pixelFormat, color, color, color);
}

std::vector<GPU::SpriteInfo> GPU::getSprites(int yLine)
{
    std::vector<SpriteInfo> sprites;
    for (int i = 0; i < 40; ++i)
    {
        SpriteInfo it = SpriteInfo(i);
        //check if it's in the line
        if (!(it.y == 0 || it.y >= 160 || yLine <= it.y - 16 || yLine >= it.y))
        {
            sprites.push_back(it);
        }
    }

    //sort by x pos
    sort(sprites.begin(), sprites.end(), [](const SpriteInfo &left, const SpriteInfo &right) { return left.x < right.x; });
    return sprites;
}

GPU::SpriteInfo::SpriteInfo(int id) 
    : y(mmu.readByte(0xFE00 + id * 4 + 0-offsetOam)),
      x(mmu.readByte(0xFE00 + id * 4 + 1-offsetOam)),
      tile(mmu.readByte(0xFE00 + id * 4 + 2-offsetOam)),
      flags(mmu.readByte(0xFE00 + id * 4 + 3-offsetOam)) {}

bool GPU::getJoypad()
{
    bool isQuit = false;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            isQuit == true;
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
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
            interruptJoypad();
        }
        else if (e.type == SDL_KEYUP)
        {
            switch (e.key.keysym.sym)
            {
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
    if (isQuit)
        return false;
    else
        return true;
    //the return value is used to determine if it's end
}
