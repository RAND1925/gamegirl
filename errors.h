//
// Created by dell on 2019/5/8.
//

#ifndef CPPGB_ERRORS_H
#define CPPGB_ERRORS_H

#include <exception>

#include <iostream>
void warning(char *s){
    cout << "WARNING!" << s;
}

void error(char *s){
    cout << "ERROR!" << s;
}

#endif //CPPGB_ERRORS_H
