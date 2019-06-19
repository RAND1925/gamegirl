//
// Created by dell on 2019/6/19.
//

#ifndef GAMEGIRL_BATTERY_H
#define GAMEGIRL_BATTERY_H

#include <string>
class Battery {
protected:
    virtual void loadData(const std::string&) = 0;
    virtual void saveData(const std::string&) = 0;
};



#endif //GAMEGIRL_BATTERY_H
