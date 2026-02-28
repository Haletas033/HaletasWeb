//
// Created by halet on 9/17/2025.
//

#ifndef MISC_H
#define MISC_H

#include <regex>
#include <unordered_map>

class misc {
public:
    static const std::string extraStyles;

    static const std::unordered_map<std::string, std::string> languageColours;

    static std::string convertToLatex(const std::string &md);
};

#endif //MISC_H
