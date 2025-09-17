//
// Created by halet on 9/17/2025.
//

#include "misc.h"

const std::string misc::extraStyles = "nav {top: 0;width: 100%; z-index: 1000; padding-left: 1em;}main.container {padding-top: 1em;}";

std::string misc::convertToLatex(const std::string& md) {
    std::string result = md;

    // Regex to match inline $...$
    std::regex inlineMath(R"(\$(.+?)\$)");
    result = std::regex_replace(result, inlineMath, R"(\\($1\\))");

    return result;
}
