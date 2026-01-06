//
// Created by halet on 1/5/2026.
//

#ifndef SKILLBALLPIT_H
#define SKILLBALLPIT_H
#include <iostream>

#endif //SKILLBALLPIT_H

#include "../js.h"

inline void skillBallPit() {
    Variable<int> foo(LET, "foo"); foo = 50;
    Variable<int> bar(CONSTANT, "bar"); bar = 40;

    foo = 10;
    bar = 10;
    std::cout << js;
}

