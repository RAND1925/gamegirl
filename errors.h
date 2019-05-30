// exception handle
// Created by dell on 2019/5/8.
//

#ifndef GAMEGIRL_ERRORS_H
#define GAMEGIRL_ERRORS_H

#include <exception>
#include <iostream>
#include <string>

void warning(const std::string & s){
    std::cout << "WARNING!" << s << std::endl;
};

void error(const std::string & s){
    std::cout << "ERROR!" << s << std::endl;
};


#endif //GAMEGIRL_ERRORS_H
