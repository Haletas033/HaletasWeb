//
// Created by halet on 2/21/2026.
//

#ifndef BUILDJS_H
#define BUILDJS_H

#include <iostream>

#include "function.h"
#include "jsCore.h"

static void Build(const std::string& fileName) {
    std::ofstream jsFile("out/scripts/" + fileName + ".js");

    jsFile << Function::GetFunctions();
    jsFile << JS::js;
}

#endif //BUILDJS_H
