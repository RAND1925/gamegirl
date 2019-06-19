#include <utility>

#include <utility>

#include <utility>

#include <utility>

//
// Created by dell on 2019/6/16.
//
#include "Exceptions.h"
#include "SDL.h"

#ifndef NO_ADDRESS_ERROR
InterruptException::InterruptException(std::string  msg, Byte iE, Byte iF)
        :_msg(std::move(msg)), _iF(iF), _iE(iE){
#ifndef NLOG
    logger << "ERROR: " << _msg << "iE" << std::hex << (int)iE << "iF"<< std::hex <<(int)iF << std::endl;
#endif
}

#endif

SDLException::SDLException(std::string msg) : _msg(std::move(msg)){
#ifndef NLOG
    logger << _msg<< " " << SDL_GetError() << std::endl;
#endif
}

FileNotFoundException::FileNotFoundException(std::string msg, const std::string path) : _msg{std::move(msg)}, _path{path}{
#ifndef NLOG
    logger << "ERROR: " << _msg << " path: " << path << std::endl;
#endif
}

