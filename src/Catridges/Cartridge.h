//
// Created by dell on 2019/6/15.
//

#ifndef GAMEGIRL_CARTRIDGE_H
#define GAMEGIRL_CARTRIDGE_H

#include "../AddressSpace.h"

class Cartridge: public AddressSpace{
public:
  virtual ~Cartridge(){};
};


#endif //GAMEGIRL_CARTRIDGE_H
