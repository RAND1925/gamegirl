//
// Created by dell on 2019/5/7.
// 这是一个抽象类，凡是需要在内存提供读写接口的，都要继承他

#ifndef CPPGB_ADDRESSSPACE_H
#define CPPGB_ADDRESSSPACE_H


class AddressSpace {
public:
    virtual bool accepts(Word address) = 0;
    virtual Byte getByte(Word address) = 0;
    virtual void setByte(Word address, Byte value) = 0;
};


#endif //CPPGB_ADDRESSSPACE_H
