#include<iostream>
#include<sstream>
#include<fstream>
#include<filesystem>

#include<nlohmann/json.hpp>
#include<curl/curl.h>

//Custom-made class to create HTML tag programmatically
#include"Tag.h"

//Callback function for curl to handle HTTPS requests
size_t writeCallBack(void* contents, size_t size, size_t nmemb, void* userp) {

    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//Fetches a users github repos
std::string getRepos(const std::string& username) {

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
Tag generateRepoLinks(const std::string& username) {
    std::string response = getRepos(username);

    Tag container = div();

    if (response.empty()) {
        Tag errorMsg = p("Failed to load repositories");
        div().put(errorMsg);
        return container;
    }

    auto repos = nlohmann::json::parse(response);

    for (const auto& repo : repos) {
        std::string repoName = repo["name"];
        std::string repoUrl = repo["html_url"];
        std::string repoDescription;

        if (repo.contains("description") && !repo["description"].is_null()) {
            repoDescription = repo["description"];
        } else {
            repoDescription = "No description provided.";
        }

        Tag link = Tag("a").addAttr("href", repoUrl).text(repoName);
        Tag description = p(repoDescription);

        Tag article("article");
        article.addAttr("class", "project")
               .put(link)
               .put(description);

        container.put(article);
    }

    return container;
}

//Function to make a navbar
Tag createNavLink(const std::string& href, const std::string& text)
{
    return Tag("li").put(
        Tag("a").addAttr("href", href).text(text)
    );
}

Tag buildNavbar() {
    Tag nav("nav");
    Tag ul("ul");
    ul
        .put(createNavLink("index.html", "Home"))
        .put(createNavLink("links.html", "Links"))
        .put(createNavLink("projects.html", "Projects"));
    nav.put(ul);
    return nav;
}

int index() {
    Tag header("header");
    header.put(buildNavbar());

    Tag hr("hr");

    Tag main("main");
    main.addAttr("class", "container");

    main
        .put(h1("Hello, I'm Haletas"))
        .put(h2("Award losing C++ dev from New Zealand"))
        .put(hr);

    Tag AboutMe = Tag("article")
        .put(h3("About Me:"))
        .put(p("Hello my name is Alex Curran. "
                   "I'm a high school student (currently attending Lakes High, Rotorua) and I enjoy programming, "
                   "3D modeling and electronics."));

    Tag Skills("article");

    Skills.put(h3("Skills:"))

    .put(p(
         "- C++<br>"
        "- C#<br>"
        "- Python<br>"
        "- HTML<br>"
        "- CSS<br>"
        "- JS<br>"
    ));

    main.put(AboutMe).put(Skills);

    WriteHTML("index.html", header, main);

    return 0;
}

int links() {
    Tag header("header");

    header.put(buildNavbar());

    Tag main("main");
    main.addAttr("class", "container")
        .put(h1("Links"));

    Tag links("article");
    links.put(
        Tag("ul")
            .put(createNavLink("https://github.com/Haletas033", "GitHub"))
            .put(createNavLink("https://www.youtube.com/@Haletas3D", "YouTube"))
    );

    main.put(links);

    WriteHTML("links.html", header, main);

    return 0;
}

int projects() {
    Tag header("header");

    header.put(buildNavbar());

    Tag main("main");
    main.addAttr("class", "container");

    main.put(h1("Projects"));

    main.put(generateRepoLinks("Haletas033"));

    WriteHTML("projects.html", header, main);

    return 0;
}

int main() {
    index();
    links();
    projects();
}