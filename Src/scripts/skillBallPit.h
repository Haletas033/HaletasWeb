//
// Created by halet on 1/5/2026.
//

#ifndef SKILLBALLPIT_H
#define SKILLBALLPIT_H
#include <iostream>

#endif //SKILLBALLPIT_H

#include "../js.h"

#define test skillBallPit

inline void test() {
    Variable<int> foo(LET, "foo"); foo = 50;
    Variable<int> bar(LET, "bar"); bar = 40;

    foo = 1;
    bar = 10;
    foo + bar;
    std::cout << js;
}

