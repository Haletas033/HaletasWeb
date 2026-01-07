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
    Variable foo(LET, "foo"); foo = 50;
    Variable bar(LET, "bar"); bar = 40;

    foo = 1;
    bar = 10;
    foo + bar;
    foo + 50;
    bar + 40;
    std::cout << js;
}

