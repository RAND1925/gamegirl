//
// Created by dell on 2019/6/19.
//

#ifndef GAMEGIRL_BATTERY_H
#define GAMEGIRL_BATTERY_H

#include <string>

class Battery {
    virtual void saveFile(std::string filePath) = 0;
    virtual void loadFile(std::string filePath) = 0;
};


#endif //GAMEGIRL_BATTERY_H
