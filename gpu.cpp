#include <string>
#include <iostream>
#include <algorithm>
#include "gpu.h"
#include "SDLManager.h"
#include "InterruptManager.h"
#include "MMU.h"
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
            Byte yLine = regLineY;
            draw(yLine);
            yLine++;
            regLineY=yLine;
            if (yLine >= 144) //call for the interrrput
            {
                setMode(MODE_VBLANK);
                sdlManager.refreshWindow();
            }
            else
            {
                //less than 144 ,comtinue to get the line
                doDMA(regDMA);
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
            doDMA(regDMA);
            setMode(MODE_OAM);
            innerClock -= 456 * 10;
            regLineY=0;
            
        }
        else
            regLineY=yLine;
        break;
    }
    setLCYInterrupt();
}
void GPU::setMode(Byte mode)
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
    regLcdStatus = statusLCDC;
    if (mode == MODE_VBLANK)
        interruptManager.requestInterrupt(0);
    bool interruptFlag = false ;
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

}



void GPU::draw(int yLine) {


#ifndef NLOG
    display();
#endif
    //lcdc :
    //OBJ 1-switch 2-size
    //WINDOW 5-switch 6=selection
    //BG 0-switch 3-bgcode 4-bgCharacter
    //LCDC switch 7
    //check if it's in the real line
    if (yLine >= 144)
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

    if (!getBit(lcdc, 7)) {
        //todo: turn off screen
    }
    uint32_t colorLine[160] = {0};
    if (bgWinEnabled) {
        drawBg(colorLine,bgWinDataLow, bgMapHigh);
    }
    if (spriteEnabled){
        drawSprite(colorLine, spriteLarge);
    }
    sdlManager.setLine(yLine, colorLine);
}

void GPU::setLCYInterrupt()
{
    Byte yLine = regLineY;
    Byte yLine_cp = regLineYC;
    Byte statusLCDC = regLcdStatus;
    //write to the stat bit 2
    if (yLine == yLine_cp){
        setBit(statusLCDC, 2);
    } else {
        resetBit(statusLCDC, 2);
    }
    regLcdStatus=statusLCDC;
}

Byte GPU::getByte(Word address) {
    if (address >= offsetVram && address < offsetVram + lengthVram)
        return bytesVram[address - offsetVram];
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
                return regWindowX;
            case 0xFF4B:
                return regWindowY;
            default:
                break;
        }
    }
    throw WrongAddressException("GPU[read]", address);
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
        bytesVram[address - offsetVram] = value;
        return;
    } else if (address >= offsetChr && address < offsetChr + lengthChr){
        bytesChr[address - offsetChr] = value;
        return;
    }
    else if (address >= offsetOam && address < offsetOam + lengthOam) {
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
                regWindowX = value;
                return;
            case 0xFF4B:
                regWindowY = value;
                return;
            default:
                break;
        }
    }
    throw WrongAddressException("GPU[write]", address);
}

void GPU::display() {
#ifndef NLOG
    logger << "LCDC:" << std::hex <<(int)regLcdControl << ' '
           << "STAT:" << std::hex <<(int)regLcdStatus << ' '
           << "SCX:" << std::hex <<(int)regScrollX << ' '
           << "SCY:" << std::hex <<(int)regScrollY << ' '
           << "LY:" << std::hex <<(int)regLineY << ' '
           << "LYC:" << std::hex <<(int)regLineYC << std::endl;
#endif
}

void GPU::doDMA(Byte dma) {
    if (dma > 0xF1){
        return;
    }
    Word dmaAddress = (Word)dma << 8;
#ifndef NLOG
    logger << "DMA" << regDMA;
#endif
    AddressSpace * s = mmu.findAddressSpace(dmaAddress);
    for (Byte i = 0; i < 0xA0; ++i){
        bytesOam[i] = s->getByte(dmaAddress + i);
    }

}

void GPU::drawBg(uint32_t colorLine[], Byte bgWinDataLow, Byte bgMapHigh) {
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

    Byte xScroll = regScrollX;
    Byte yScroll = regScrollY;
    int yDraw = (regLineY + yScroll) % 256;
    int yTile = yDraw / 8;
    int yPixel = yDraw % 8;
    //?????
    Byte colorLow = 0, colorHigh = 0;

    for (size_t counter = 0; counter < 160; counter++) {
        int xDraw = ((counter + xScroll) % 256);
        int xTile = xDraw / 8;
        Byte xPixel = static_cast<Byte>(8 - xDraw % 8 - 1);
        {
            int numTile;
            if (bgWinDataLow)
                numTile = bytesChr[mapTile + (yTile * 32) + xTile];
            else
                numTile = (SByte) (bytesChr[mapTile + (yTile * 32) + xTile]);
            colorLow = bytesVram[dataTile + (numTile * 16) + (yPixel * 2)];
            colorHigh = bytesVram[dataTile + (numTile * 16) + (yPixel * 2) + 1];
            //    lastPixel = xTile;
        }
        Byte colorCode = getBit(colorLow, xPixel) | (getBit(colorHigh, xPixel) << 1);
        Byte grayCode = getGrayCode(colorCode, regBGP);
        Uint32 rgbCode = sdlManager.mapColor(grayCode);
        colorLine[counter] = rgbCode;
    }
}

void GPU::drawSprite(uint32_t * colorLine, Byte spriteLarge) {

    Byte spriteHeight = spriteLarge ? 16: 8;
    for (int i = 0xA0 - 4; i >= 0; i-= 4){
        int spriteY = bytesOam[i] - 16;
        int yPixel = regLineY - spriteY;
        if ((yPixel < 0) || (yPixel >= spriteHeight)){
            continue;
        }
        int spriteX = bytesOam[i + 1] - 8;
        if (spriteX < 0){
            continue;
        }
        Byte chrCode = bytesOam[i + 2];
        Byte colorLow = bytesVram[chrCode * 0x10 + yPixel * 2];
        Byte colorHigh = bytesVram[chrCode * 0x10 + yPixel * 2 + 1];
        for (Byte counter = 0; counter < 8; ++counter) {
            Byte colorCode = getBit(colorLow, counter) | (getBit(colorHigh, counter) << 1);
            Byte grayPalette = getBit(bytesOam[i + 3], 4) ? regOBP1: regOBP0;
            Byte grayCode = getGrayCode(colorCode, grayPalette);
           // if (grayCode != 3)
            {
                Uint32 rgbCode = sdlManager.mapColor(colorCode);
                colorLine[counter + spriteX] = rgbCode;
            }
        }
    }
}

