//
// Created by halet on 1/5/2026.
//

#ifndef SKILLBALLPIT_H
#define SKILLBALLPIT_H
#include <iostream>

#endif //SKILLBALLPIT_H

#include "../js.h"

inline void skillBallPit() {
    Variable<int> foo(LET, "foo");
    Variable<int> bar(CONSTANT, "bar");
    foo = 314;
    bar = 314;

    foo+124;
    foo+bar;

    std::cout << js;
}

