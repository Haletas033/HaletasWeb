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

struct Docs {
    page docsIndex;
    page docsTitle;
    page docsDescription;
};

struct Dsp {
    std::string repoName;
    page websiteIndex;

    Docs docs;


    std::vector<file> build;
    std::vector<file> resources;
    std::vector<file> styles;

    void setWebsiteIndex(const std::string &value) { websiteIndex = value; }
    void setDocsIndex(const std::string &value) { docs.docsIndex = value; }
    void setDocsTitle(const std::string &value) { docs.docsTitle = value; }
    void setDocsDescription(const std::string &value) { docs.docsDescription = value; }
};

class repos {
private:
    static std::vector<Dsp> dsps;
    static std::string cpp;
    static std::vector<file> styles;
    static std::string repoStyles;
public:
    static size_t writeCallBack(void* contents, size_t size, size_t nmemb, void* userp);

    static std::pair<std::string, long> getUrl(const std::string &url);

    static std::string getRepos(const std::string& username) {
        return getUrl("https://api.github.com/users/" + username + "/repos").first;
    }

    static std::pair<std::string, long> getDsp(const std::string &repoName);

    static Dsp parseDsp(const std::pair<std::string, long> &dsp);

    static void generateRepoCard(const nlohmann::json &repo, Tag &container, const Dsp &dsp);

    static void updateDocs(const Dsp &dsp);

    static void generateProjectsCPP();

    static void loadProjectHeaders();

    static void getFileStructure(std::string path, const std::string &repoName);

    static void loadResources();

    static Tag getRepoData(const std::string &username);
};

#endif //REPOS_H
