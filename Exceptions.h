// exception handle
// Created by dell on 2019/5/8.
//

#ifndef GAMEGIRL_ERRORS_H
#define GAMEGIRL_ERRORS_H

#include <exception>
#include <iostream>
#include <string>
#include "common.h"
#include "mmu.h"

class AddressWrongException: public std::invalid_argument{

    std::string _msg{""};
    Word _address = 0;
public:
    AddressWrongException(const std::string msg): invalid_argument(msg), _msg(msg){
        std::cerr << _msg<< std::endl;
    }
    AddressWrongException(const std::string msg, Word address): invalid_argument(msg),_msg(msg), _address(address){
        std::cerr << "ERROR: " << _msg << "in" << address << std::endl;
    }
};

class InterruptException: public std::invalid_argument{

    std::string _msg{""};
    Byte _iF = 0;
    Byte _iE = 0;
public:
    InterruptException(const std::string & msg): invalid_argument(msg), _msg(msg){
        std::cerr << _msg<< std::endl;
    }
    InterruptException(const std::string & msg, Byte iE, Byte iF): invalid_argument(msg),_msg(msg), _iE(iE), _iF(iF){
        std::cerr << "ERROR: " << _msg << "iE" << std::hex << (int)iE << "iF"<< std::hex <<(int)iF << std::endl;
    }
};

#endif //GAMEGIRL_ERRORS_H
