//
// Created by dell on 2019/6/5.
//

#ifndef GAMEGIRL_INTERRUPTMANAGER_H
#define GAMEGIRL_INTERRUPTMANAGER_H

#include "common.h"
#include "Exceptions.h"
#include "AddressSpace.h"

class InterruptManager : public AddressSpace
{
    bool iME = false;
    Byte iE = 0; //FFFF
    Byte iF = 0; //FF0F

    bool stop = false;
    bool halt = false;

public:
    void requestInterrupt(Byte bit)
    {
        setBit(iF, bit);
    }
    void setIF(Byte newIF)
    {
        iF = newIF;
    };
    void setIME(bool newIME)
    {
        iME = newIME;
    }
    bool getIME()
    {
        return iME;
    }
    void setStop(bool newStop)
    {
        stop = newStop;
    }
    void setHalt(bool newHalt)
    {
        halt = newHalt;
    }
    bool hasInterrupt()
    {
        Byte maskCode = iE & iF;
        return iME && maskCode;
    }

    Byte handleInterrupt()
    {
        iME = false;
        halt = false;
        Byte maskCode = iE & iF;
        for (Byte i = 0; i < 5; ++i)
        {
            if (getBit(maskCode, i))
            {
                std::cout << "interrupt: " << i << std::endl;
                iF = 0x0;
                return i;
            }
        }
        throw InterruptException("", iE, iF);
    }

    bool handleHalt()
    {
        if (halt)
        {
            if (!iME)
            {
                std::cout << "halt" << std::endl;
                halt = false;
            }
        }
        else
        {
            return true;
        }
        return false;
    }
    bool accepts(Word address) override
    {
        return address == 0xFF0F || address == 0xFFFF;
    }
    Byte getByte(Word address) override
    {
        if (address == 0xFF0F)
        {
            return iF;
        }
        else if (address == 0xFFFF)
        {
            return iE;
        }
        else
        {
            throw AddressWrongException("Interrupt[read]", address);
        }
    }

    void setByte(Word address, Byte value) override
    {
        if (address == 0xFF0F)
        {
            iF = value;
        }
        else if (address == 0xFFFF)
        {
            iE = value;
        }
        else
        {
            throw AddressWrongException("Interrupt[write]", address);
        }
    }
};

extern InterruptManager interruptManager;

#endif //GAMEGIRL_INTERRUPTMANAGER_H
