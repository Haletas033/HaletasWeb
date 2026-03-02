//
// Created by halet on 2/21/2026.
//

#ifndef STYLE_CSS_H
#define STYLE_CSS_H

#include "../style.h"

//Currently a test
inline void Stylesheet() {
    Style h1("h1"); h1
        .put("color", "red");

    Style skillCanvas("#skills canvas"); skillCanvas
        .put("border-radius", "50px");

    Builder::Build("style");
}

#endif //STYLE_CSS_H
