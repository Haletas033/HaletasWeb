//
// Created by halet on 9/17/2025.
//

#include "misc.h"

#include "../../style.h"
const std::string navStyles = "nav {top: 0;width: 100%; z-index: 1000; padding-left: 1em;}main.container {padding-top: 1em;}";
const std::string skillStyles = "#skills {display: flex;flex-direction: column;justify-content: center;align-items: center;}";

const std::string misc::extraStyles = navStyles + skillStyles;

std::string misc::convertToLatex(const std::string& md) {
    std::string result = md;

    // Regex to match inline $...$
    const std::regex inlineMath(R"(\$(.+?)\$)");
    result = std::regex_replace(result, inlineMath, R"(\\($1\\))");

    return result;
}
