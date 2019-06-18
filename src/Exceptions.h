// exception handle
// Created by dell on 2019/5/8.
//

#ifndef GAMEGIRL_ERRORS_H
#define GAMEGIRL_ERRORS_H

#include <exception>
#include <string>
#include <cassert>
#include "common.h"
#include "Logger.h"


#define NO_WRONG_ADDRESS_ERROR

/* wrong address is an assertion now
 */
class WrongAddressException: public std::exception{

    std::string _msg{};
    Word _address = 0;
public:
    WrongAddressException(std::string  msg, Word address);;
};

class FileNotFoundException: public std::exception{
    std::string _msg{};
    std::string _path{};
public:
    FileNotFoundException(std::string msg, std::string path);
};

class SDLException: public std::exception{
    std::string _msg{};
public:
    explicit SDLException(std::string  msg);
};

class InterruptException: public std::exception{
    std::string _msg{};
    Byte _iF;
    Byte _iE;
public:
    InterruptException(std::string msg, Byte iE, Byte iF);
};

#endif //GAMEGIRL_ERRORS_H
