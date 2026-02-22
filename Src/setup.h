//
// Created by halet on 2/22/2026.
//

#ifndef SETUP_H
#define SETUP_H
#include <filesystem>
#include <iostream>
#include <string>

#include "Tag.h"

class Setup {
public:
    //Creates a dir in out/
    static void CreateDir(const std::string &name) {
        try {
            std::filesystem::create_directories("out/" + name);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Warning: could not create " + name + ": " << e.what() << '\n';
        }
    }

    //Copies a dir into out/
    static void CopyDir(const std::string &src, const std::string &dst) {
        try {
            std::filesystem::copy(src, "out/" + dst,
            std::filesystem::copy_options::recursive |
            std::filesystem::copy_options::overwrite_existing);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Warning: could not copy " + src + ": " << e.what() << '\n';
        }
    }
};


#endif //SETUP_H
