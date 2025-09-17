//
// Created by halet on 9/17/2025.
//

#ifndef REPOS_H
#define REPOS_H

#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include "../../Tag.h"

class repos {
public:
    static size_t writeCallBack(void* contents, size_t size, size_t nmemb, void* userp);

    static std::string getRepos(const std::string& username);

    static Tag generateRepoLinks(const std::string &username);
};

#endif //REPOS_H
