//
// Created by halet on 9/17/2025.
//

#ifndef REPOS_H
#define REPOS_H

#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include "../../Tag.h"

//Information from repos about how to display (dsp) their card
typedef std::string file;
typedef std::string page;

struct Dsp {
    std::string repoName;
    page websiteIndex;
    page docsIndex;
    std::vector<file> build;
    std::vector<file> resources;
    std::vector<file> styles;

    void setWebsiteIndex(const std::string &value) { websiteIndex = value; }
    void setDocsIndex(const std::string &value) { docsIndex = value; }
};

class repos {
private:
    static std::vector<Dsp> dsps;
    static std::string cpp;
public:
    static size_t writeCallBack(void* contents, size_t size, size_t nmemb, void* userp);

    static std::string getUrl(const std::string& url);

    static std::string getRepos(const std::string& username) {
        return getUrl("https://api.github.com/users/" + username + "/repos");
    }

    static std::string getDsp(const std::string& repoName);

    static Dsp parseDsp(const std::string &dsp);

    static void generateRepoCard(const nlohmann::json &repo, Tag &container, const Dsp &dsp);

    static void generateProjectsCPP();

    static void loadProjectHeaders();

    static Tag getRepoData(const std::string &username);
};

#endif //REPOS_H
