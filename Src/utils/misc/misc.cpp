//
// Created by halet on 9/17/2025.
//

#include "misc.h"

#include "../../style.h"
const std::string navStyles = "nav {top: 0;width: 100%; z-index: 1000; padding-left: 1em;}main.container {padding-top: 1em;}";
const std::string skillStyles = "#skills {display: flex;flex-direction: column;justify-content: center;align-items: center;}";

const std::string misc::extraStyles = navStyles + skillStyles;

const std::unordered_map<std::string, std::string> misc::languageColours = {
    {"C++", "#f34b7d"},
    {"C", "#555555"},
    {"Zig", "#ec915c"},
    {"C#", "#178600"},
    {"Python", "#3572a5"},
    {"Java", "#b07219"},
    {"Kotlin", "#a97bff"}
};

std::string misc::convertToLatex(const std::string& md) {
    std::string result = md;

    // Regex to match inline $...$
    const std::regex inlineMath(R"(\$(.+?)\$)");
    result = std::regex_replace(result, inlineMath, R"(\\($1\\))");

    return result;
}
