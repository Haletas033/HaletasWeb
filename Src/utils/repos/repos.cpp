//
// Created by halet on 9/17/2025.
//

#include "repos.h"

//Callback function for curl to handle HTTPS requests
size_t repos::writeCallBack(void* contents, size_t size, size_t nmemb, void* userp) {

    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//Fetches a users GitHub repos
std::string repos::getRepos(const std::string& username) {

    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    std::string url = "https://api.github.com/users/" + username + "/repos"; //Github API endpoint

    curl_global_init(CURL_GLOBAL_DEFAULT); //Initialize global curl state
    curl = curl_easy_init(); //Initialize curl handle

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl); //Perform the request and store the result in res

        //Error checking
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl); //Clean up the curl handle after the request is done
    }


    curl_global_cleanup(); //Clean up the global curl state
    return readBuffer; //Return the response data

}

//Function to parse the GitHub JSON response and generate HTML links
Tag repos::generateRepoLinks(const std::string& username) {
    std::string response = getRepos(username);

    Tag container = div();

    if (response.empty()) {
        Tag errorMsg = p("Failed to load repositories");
        div().put(errorMsg);
        return container;
    }

    auto repos = nlohmann::json::parse(response);

    if (!repos.is_array()) {
        std::cerr << "Error: GitHub API did not return a list of repos\n";
        return container;
    }

    for (const auto& repo : repos) {
        if (!repo.is_object()) continue;

        int stars = repo["stargazers_count"];
        int forks = repo["forks_count"];
        std::string repoName = repo["name"];
        std::string repoUrl = repo["html_url"];
        std::string repoDescription;
        std::string language;

        if (repo.contains("description") && !repo["description"].is_null()) {
            repoDescription = repo["description"];
        } else {
            repoDescription = "No description provided.";
        }

        if (repo.contains("language") && !repo["language"].is_null()) {
            language = repo["language"];
        } else {
            language = "Unknown";
        }

        std::string starsStr = std::to_string(stars);
        std::string forksStr = std::to_string(forks);

        Tag link = Tag("a").addAttr("href", repoUrl);
        Tag description = p(repoDescription);
        Tag otherInfo = p(starsStr + " Stars | " + forksStr + " Forks | " + language);

        Tag article("article");
        article.addAttr("class", "project")
               .put(link.put(h2(repoName)))
               .put(description)
               .put(otherInfo);

        container.put(article);
    }

    return container;
}
