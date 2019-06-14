// exception handle
// Created by dell on 2019/5/8.
//

#ifndef GAMEGIRL_ERRORS_H
#define GAMEGIRL_ERRORS_H

#include <exception>
#include <iostream>
#include <string>
#include "common.h"
#include "SDL.h"
#include "Logger.h"
#include "AddressSpace.h"

class AddressSpaceRemoveException: public std::exception{

    std::string _msg{""};
    AddressSpace* _addressSpacePointer;
public:
    AddressSpaceRemoveException(const std::string msg, AddressSpace* addressSpacePointer): _msg(msg), _addressSpacePointer(addressSpacePointer){
#ifndef LOG
        logger << "ERROR: " << _msg <<  std::endl;
#endif
    }
};
class WrongAddressException: public std::exception{

    std::string _msg{""};
    Word _address = 0;
public:

    WrongAddressException(const std::string msg, Word address): _msg(msg), _address(address){
#ifndef LOG
        logger << "ERROR: " << _msg << "in" << address << std::endl;
#endif
    }
};

class SDLException: public std::exception{
    std::string _msg{""};
public:
    SDLException(const std::string & msg): _msg(msg){
#ifndef LOG
       logger << _msg<< " " << SDL_GetError() << std::endl;
#endif
    }
};


class InterruptException: public std::exception{
    std::string _msg{""};
    Byte _iF = 0;
    Byte _iE = 0;
public:
    InterruptException(const std::string & msg, Byte iE, Byte iF):_msg(msg), _iE(iE), _iF(iF){
#ifndef LOG
        logger << "ERROR: " << _msg << "iE" << std::hex << (int)iE << "iF"<< std::hex <<(int)iF << std::endl;
#endif
    }
};

#endif //GAMEGIRL_ERRORS_H
