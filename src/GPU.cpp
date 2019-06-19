
#include <algorithm>
#include <tuple>
#include "MMU.h"
#include "GPU.h"
#include "SDLManager.h"
#include "InterruptManager.h"
#include <cassert>
#include "Exceptions.h"

void GPU::addTime(int clock)
{
    //reset the flag
    //setInterruptFlag(0, false);
    //setInterruptFlag(1, false);
    // check isLCDenabled if() acoordig to the oxff40
#ifndef NLOG
    display();
#endif
    Byte statusLCD =regLcdControl;
    if (!getBit(statusLCD,7 ))
        return;
    //increase the gpu clock
    innerClock += clock;

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
        if (innerClock >= 79)
        {
            innerClock -= 79;
            setMode(MODE_VRAM);
        }

        break;
    case MODE_VRAM:
        if (innerClock >= 172)
        {
            innerClock -= 172;
            setMode(MODE_HBLANK);
        }
        break;
    case MODE_HBLANK:
        if (innerClock >= 205)
        {
            innerClock -= 205; //back to 0;
            //get the current line to draw
            draw();
            regLineY++;
            if (regLineY >= 144) //call for the interrrput
            {
                setMode(MODE_VBLANK);
                SDLManager::getSDLManager()->refreshWindow();
            }
            else
            {
                //less than 144 ,comtinue to get the line
                //doDMA(regDMA);
                setMode(MODE_OAM);
            }
        }
        break;
    case MODE_VBLANK:
        //todo about the Vblank interrrupt
        Byte yLine = (innerClock / 456) + 144;
        if (innerClock >= 456 * 10)
        {
            //back to the oam,reset the stat
           // doDMA(regDMA);
            setMode(MODE_OAM);
            innerClock -= 456 * 10;
            regLineY=0;
        }
        else
            regLineY=yLine;
        break;
    }
    if (regLineY == regLineYC){
        setBit(regLcdStatus, 2);
        if (getBit(regLcdControl,6)){
            InterruptManager::getInterruptManager()->requestInterrupt(1);
        }
    } else {
        resetBit(regLcdStatus, 2);
    }
}

void GPU::setMode(Byte mode)
{
    if (currentMode == mode)
        return;
    currentMode = mode;
    //rese the 0bit and 1bit of the status
    //and 1111|1100
    regLcdStatus &= 0xFCu;
    //set the current mode into bit0 bit1
    regLcdStatus |= currentMode & 0x03u;

    if (mode == MODE_VBLANK)
        InterruptManager::getInterruptManager()->requestInterrupt(0);
    bool interruptFlag = false ;
    switch (mode)
    {
    case MODE_HBLANK:
        if (getBit(regLcdStatus,3))
            interruptFlag = true;
        break;
    case MODE_VBLANK:
        if (getBit(regLcdStatus,4))
            interruptFlag = true;
        break;

    case MODE_OAM:
        if (getBit(regLcdStatus,5))
            interruptFlag = true;
        break;
    default:
        break;
    }
    if (interruptFlag)
        InterruptManager::getInterruptManager()->requestInterrupt(1);
}



void GPU::draw() {
#ifndef NLOG
    display();
#endif
    //check if it's in the real line
    if (regLineY >= 144)
        return;
    Byte lcdc = regLcdControl;
    bool lcdcEnabled = getBit(lcdc,7);
    bool bgWinEnabled = getBit(lcdc, 0);
    bool bgWinDataLow = getBit(lcdc,4);
    bool bgMapHigh = getBit(lcdc, 3);
    bool winEnabled = getBit(lcdc,5);
    bool winMapHigh = getBit(lcdc,6);
    bool spriteEnabled = getBit(lcdc, 1);
    bool spriteLarge = getBit(lcdc, 2);

    if (!lcdcEnabled) {
        //todo: turn off screen
    }
    uint32_t colorLine[160] = {0};
    if (bgWinEnabled) {
        drawBg(colorLine, bgWinDataLow, bgMapHigh);
        if(winEnabled){
            drawWin(colorLine, bgWinDataLow, winMapHigh);
        }
    }
    if (spriteEnabled && useSprite){
        drawSprite(colorLine, spriteLarge);
    }
    SDLManager::getSDLManager()->setLine(regLineY, colorLine);
}

Byte GPU::getByte(Word address) {
    if (address >= offsetVram && address < offsetVram + lengthVram)
        return bytesVRam[address - offsetVram];
    else if (address >= offsetChr && address < offsetChr + lengthChr)
        return bytesChr[address - offsetChr];
    else if (address >= offsetOam && address < offsetOam + lengthOam)
        return bytesOam[address - offsetOam];
    else
    {
        switch (address)
        {
            case 0xFF40:
                return regLcdControl;
            case 0xFF41:
                return regLcdStatus;
            case 0xFF42:
                return regScrollY;
            case 0xFF43:
                return regScrollX;
            case 0xFF44:
                return regLineY;
            case 0xFF45:
                return regLineYC;
            case 0xFF46:
                return regDMA;
            case 0xFF47:
                return regBGP;
            case 0xFF48:
                return regOBP0;
            case 0xFF49:
                return regOBP1;
            case 0xFF4A:
                return regWindowY;
            case 0xFF4B:
                return regWindowX;
            default:
                break;
        }
    }
#ifndef NO_ADDRESS_ERROR
    throw WrongAddressException("GPU[read]", address);
#endif
    assert(false);
    return 0xFFu;
}

bool GPU::accepts(Word address) {
    if (address >= offsetVram && address < offsetVram + lengthVram)
        return true;
    else if (address >= offsetChr && address < offsetChr + lengthChr)
        return true;
    else if (address >= offsetOam && address < offsetOam + lengthOam)
        return true;
    else if (address >= 0xFF40 && address <= 0xFF4B)
        return true;
    return false;
}

void GPU::setByte(Word address, Byte value) {

    if (address >= offsetVram && address < offsetVram + lengthVram) {
        if(currentMode == MODE_VRAM){
            return;
        }
        bytesVRam[address - offsetVram] = value;
        return;
    } else if (address >= offsetChr && address < offsetChr + lengthChr){
        if ( currentMode == MODE_VRAM){
            return;
        }
        bytesChr[address - offsetChr] = value;
        return;
    }
    else if (address >= offsetOam && address < offsetOam + lengthOam) {
        if (currentMode == MODE_OAM || currentMode == MODE_VRAM){
            return;
        }
        bytesOam[address - offsetOam] = value;
        return;
    }
    else
    {
        switch (address)
        {
            case 0xFF40:
                regLcdControl = value;
                return;
            case 0xFF41:
                regLcdStatus = value;
                return;
            case 0xFF42:
                regScrollY = value;
                return;
            case 0xFF43:
                regScrollX = value;
                return;
            case 0xFF44:
                regLineY = 0; //warn
                return;
            case 0xFF45:
                regLineYC = value;
                return;
            case 0xFF46:
                regDMA = value;
                doDMA(regDMA);

                return;
            case 0xFF47:
                regBGP = value;
                return;
            case 0xFF48:
                regOBP0 = value;
                return;
            case 0xFF49:
                regOBP1 = value;
                return;
            case 0xFF4A:
                regWindowY = value;
                return;
            case 0xFF4B:
                regWindowX = value;
                return;
            default:
                assert(false);
        }
    }
#ifndef NO_ADDRESS_ERROR
    throw WrongAddressException("GPU[write]", address);
#endif

}
#ifndef NLOG
void GPU::display() {
    logger << "LCDC:" << std::hex <<(int)regLcdControl << ' '
           << "STAT:" << std::hex <<(int)regLcdStatus << ' '
           << "SCX:" << std::hex <<(int)regScrollX << ' '
           << "SCY:" << std::hex <<(int)regScrollY << ' '
           << "LY:" << std::hex <<(int)regLineY << ' '
           << "LYC:" << std::hex <<(int)regLineYC << std::endl;
}
#endif
void GPU::doDMA(Byte dma) {
    if (dma > 0xF1){
        return;
    }
    Word dmaAddress = (Word)dma << 8u;
#ifndef NLOG
    logger << "DMA" << regDMA;
#endif
    if ((dma >= 0xA0 && dma <= 0xF1) || dma <= 0x80){
        AddressSpace* s = MMU::getMMU()->findAddressSpace(dmaAddress);
        for (Byte i = 0; i < 0xA0; ++i){
            bytesOam[i] = s->getByte(dmaAddress + i);
        }
    } else if(dma >= 0x80 && dma < 0x98) {
        Word offset = dmaAddress - 0x8000;
        std::copy(bytesVRam.begin() + offset, bytesVRam.begin() + offset + 0xA0, bytesOam.begin());
    } else if(dma >= 0x98 && dma < 0xA0) {
        Word offset = dmaAddress - 0x9800;
        std::copy(bytesChr.begin() + offset, bytesChr.begin() + offset + 0xA0, bytesOam.begin());
    }
}

void GPU::drawBg(uint32_t * colorLine, bool bgWinDataLow, bool bgMapHigh){
    Word dataTile, mapTile;
    //check the display is enable or not
    if (bgWinDataLow)
        dataTile = 0x0000;
    else
        dataTile = 0x1000;

    //get the map
    if (bgMapHigh)
        mapTile = 0x400;
    else
        mapTile = 0x000;

    Byte xScrl = regScrollX;
    Byte yScrl = regScrollY;
    int yDraw = (regLineY + yScrl) % 256;
    int yTile = yDraw / 8;
    int yPixel = yDraw % 8;
    //?????
    Byte colorLow = 0, colorHigh = 0;
    for (size_t counter = 0; counter < 160; counter++) {
        int xDraw = ((counter + xScrl) % 256);
        int xTile = xDraw / 8;
        Byte xPixel = static_cast<Byte>(8 - xDraw % 8 - 1);
        {
            int numTile;
            if (bgWinDataLow)
                numTile = bytesChr[mapTile + (yTile * 32) + xTile];
            else
                numTile = (SByte) (bytesChr[mapTile + (yTile * 32) + xTile]);
            colorLow = bytesVRam[dataTile + (numTile * 16) + (yPixel * 2)];
            colorHigh = bytesVRam[dataTile + (numTile * 16) + (yPixel * 2) + 1];
        }
        Byte colorCode = getBit(colorLow, xPixel) | (getBit(colorHigh, xPixel) << 1u);
        Byte grayCode = getGrayCode(colorCode, regBGP);
        Uint32 rgbCode = SDLManager::getSDLManager()->mapColor(grayCode);
        colorLine[counter] = rgbCode;
    }
}

void GPU::drawWin(uint32_t colorLine[], bool bgWinDataLow, bool winMapHigh) {
    if(regWindowX < 7){
        regWindowX = 7;
    }
    Byte xWin = regWindowX - 7;
    Byte yWin = regWindowY;
    int yDraw = regLineY - yWin;
    if(yDraw < 0)
        return;
    Word dataTile, mapTile;
    //check the display is enable or not
    if (bgWinDataLow)
        dataTile = 0x0000;
    else
        dataTile = 0x1000;

    //get the map
    if (winMapHigh)
        mapTile = 0x400;
    else
        mapTile = 0x000;
    int yTile = yDraw / 8;
    int yPixel = yDraw % 8;
    //?????
    Byte colorLow = 0, colorHigh = 0;
    for (size_t counter = 0; counter < 160; counter++) {
        if (counter < xWin){
            continue;
        }
        int xDraw = counter - xWin;
        int xTile = xDraw / 8;
        Byte xPixel = static_cast<Byte>(8 - xDraw % 8 - 1);
        {
            int numTile;
            if (bgWinDataLow)
                numTile = bytesChr[mapTile + (yTile * 32) + xTile];
            else
                numTile = (SByte) (bytesChr[mapTile + (yTile * 32) + xTile]);
            colorLow = bytesVRam[dataTile + (numTile * 16) + (yPixel * 2)];
            colorHigh = bytesVRam[dataTile + (numTile * 16) + (yPixel * 2) + 1];
        }
        Byte colorCode = getBit(colorLow, xPixel) | (getBit(colorHigh, xPixel) << 1u);
        Byte grayCode = getGrayCode(colorCode, regBGP);
        uint32_t rgbCode = SDLManager::getSDLManager()->mapColor(grayCode);
        colorLine[counter] = rgbCode;
    }
}
using Sprite = std::tuple<Byte, Word, Byte>;
        //0: x, 1, colorlow, 2 colorhigh
void GPU::drawSprite(uint32_t * colorLine, bool spriteLarge) {
    Byte spriteHeight = spriteLarge ? 16: 8;
    std::vector<Sprite> ready_to_gender{};
    for (int i = 0xA0 - 4; i >= 0; i-= 4) {
        int spriteY = bytesOam[i] - 16;
        int yPixel = regLineY - spriteY;
        if ((yPixel < 0) || (yPixel >= spriteHeight)) {
            continue;
        }
        int spriteX = bytesOam[i + 1] - 8;
        if (spriteX >= 160) {
            continue;
        }
        Byte chrCode = bytesOam[i + 2];
        Byte infoCode = bytesOam[i + 3];
        bool yFlip = getBit(infoCode, 6);
        Word pixelAddress = (chrCode << 4u) + (yFlip?  ((spriteHeight - 1 - yPixel) * 2)  : yPixel * 2);
        ready_to_gender.emplace_back(spriteX, pixelAddress, infoCode);
    }
    std::stable_sort(ready_to_gender.begin(), ready_to_gender.end(), [](Sprite a, Sprite b){
        return std::get<0>(a) > std::get<0>(b);});

    int size = ready_to_gender.size();
    for (int i = 0 ; i < size; ++i) {
        auto s = ready_to_gender[i];
        Word pixelAddress = std::get<1>(s);
        Byte colorLow = bytesVRam[pixelAddress];
        Byte colorHigh = bytesVRam[pixelAddress + 1];
        Byte infoCode = std::get<2>(s);
        Byte grayPalette = getBit(infoCode, 4) ? regOBP1: regOBP0;
        bool xFlip = getBit(infoCode, 5);
        for (Byte counter = 0; counter < 8; ++ counter) {
            Byte col = std::get<0>(s) + (xFlip ? counter : 7 - counter);
            if (col < 0 || col >= 160)
                continue;
            Byte colorCode = getBit(colorLow, counter) | (getBit(colorHigh, counter) << 1);
            if (colorCode == 0){
                continue;
            }
            Byte grayCode = getGrayCode(colorCode, grayPalette);
            uint32_t rgbCode = SDLManager::getSDLManager()->mapColor(grayCode);
            colorLine[col] = rgbCode;
        }
    }
}

Byte GPU::getGrayCode(Byte colorCode, Byte reg) {
    return static_cast<Byte>(reg >> (colorCode << 1u) & 0x03u);
}

void GPU::init(bool useSprite) {
    this->useSprite = useSprite;
}

