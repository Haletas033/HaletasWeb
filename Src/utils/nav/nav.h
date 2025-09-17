//
// Created by halet on 9/17/2025.
//

#ifndef NAV_H
#define NAV_H

#include "../../Tag.h"

class nav {
public:
    static Tag createNavLink(const std::string &href, const std::string &text);

    static Tag buildNavbar();
};

#endif //NAV_H
