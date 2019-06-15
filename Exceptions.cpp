//
// Created by dell on 2019/6/16.
//
#include "Exceptions.h"

InterruptException::InterruptException(std::string msg, Byte iE, Byte iF)
        :_msg(std::move(msg)), _iE(iE), _iF(iF){
#ifndef NLOG
    logger << "ERROR: " << _msg << "iE" << std::hex << (int)iE << "iF"<< std::hex <<(int)iF << std::endl;
#endif
}

SDLException::SDLException(std::string msg) : _msg(std::move(msg)){
#ifndef NLOG
    logger << _msg<< " " << SDL_GetError() << std::endl;
#endif
}

FileNotFoundException::FileNotFoundException(const std::string msg, const std::string path) : _msg{msg}, _path{path}{
#ifndef NLOG
    logger << "ERROR: " << _msg << "in" << address << std::endl;
#endif
}

WrongAddressException::WrongAddressException(const std::string msg, Word address) : _msg(msg), _address(address){
#ifndef NLOG
    logger << "ERROR: " << _msg << "in" << address << std::endl;
#endif
}
