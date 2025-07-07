#include<iostream>
#include<sstream>
#include<fstream>
#include<filesystem>

#include<nlohmann/json.hpp>
#include<curl/curl.h>

//Custom-made class to create HTML tag programmatically
#include"Tag.h"

//Callback fucntion for curl to handle HTTPS requests
size_t writeCallBack(void* contents, size_t size, size_t nmemb, void* userp) {

    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//Fetchs a users github repos
std::string getRepos(const std::string& username) {

    CURL* curl;
    CURLcode res;
    std::string readbuffer;
    std::string url = "https://api.github.com/users/" + username + "/repos"; //Github API endpoint

    curl_global_init(CURL_GLOBAL_DEFAULT); //Initialize global curl state
    curl = curl_easy_init(); //Initialize curl handle

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readbuffer);

        res = curl_easy_perform(curl); //Perform the request and store the result in res

        //Error checking
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl); //Clean up the curl handle after the request is done
    }


    curl_global_cleanup(); //Clean up the global curl state
    return readbuffer; //Return the response data

}

//Function to parse the GitHub JSON response and generate HTML links
Tag generateRepoLinks(const std::string& username) {
    std::string response = getRepos(username);

    Tag container("div");

    if (response.empty()) {
        Tag errorMsg("p");
        errorMsg << "Failed to load repositories.";
        container << errorMsg;
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

        Tag article("article");
        article.addAttr("class", "project");

        Tag a("a");
        a.addAttr("href", repoUrl);
        a << repoName;

        Tag p("p");
        p << repoDescription;

        article << a << p;

        container << article;
    }

    return container;
}

//Function to make a navbar
Tag createNavLink(const std::string& href, const std::string& text)
{
    Tag a("a");
    a.addAttr("href", href);
    a << text;

    Tag li("li");
    li << a;

    return li;
}

int index() {


    std::filesystem::create_directories("out");

    std::ofstream htmlFile("out/index.html");


    if (!htmlFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    htmlFile << Tag("").Head();

    Tag header("header");

    Tag main("main");
    main.addAttr("class", "container");

    //Header tags
    Tag h1("h1");
    Tag h2("h2");
    Tag hr("hr");

    h1 << "Hello I'm Haletas";
    h2 << "Award losing C++ dev from New Zealand";

    main << header << h1 << h2 << hr;

    //Paragraph tags
    Tag p("p");
    Tag p1("p");

    Tag nav("nav");
    Tag ulNav("ul");
    ulNav << createNavLink("index.html", "Home");
    ulNav << createNavLink("links.html", "Links");
    ulNav << createNavLink("projects.html", "Projects");
    nav << ulNav;



    Tag h3About("h3");
    Tag pAbout("p");

    Tag h3Skills("h3");
    Tag pSkills("p");

    Tag AboutMe("article");
    Tag Skills("article");

    h3About << "About me:";
    pAbout << "Hello my name is Alex Curran, and I'm a high school student (currently attending Lakes High, Rotorua) and I am a enjoyer of programming, 3D modeling and electronics.";

    h3Skills << "Skills:";
    pSkills << "- C++<br>"
            << "- C#<br>"
            << "- Python<br>"
            << "- HTML<br>"
            << "- CSS<br>"
            << "- JS<br>";

    AboutMe << h3About << pAbout;
    Skills << h3Skills << pSkills;

    main << AboutMe<< Skills;

    htmlFile << nav.str() << "\n";
    htmlFile << header.str() << "\n";
    htmlFile << main.str() << "\n";

    htmlFile << "</body>\n";
    htmlFile << "</html>\n";

    htmlFile.close();

    return 0;
}

int links() {
    std::ofstream htmlFile("out/links.html");

    if (!htmlFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    htmlFile << Tag("").Head();

    Tag header("header");

    Tag nav("nav");
    Tag ulNav("ul");
    ulNav << createNavLink("index.html", "Home");
    ulNav << createNavLink("links.html", "Links");
    ulNav << createNavLink("projects.html", "Projects");
    nav << ulNav;

    Tag main("main");

    main.addAttr("class", "container");

    Tag h1("h1");
    h1 << "Links";
    main << header << h1;

    Tag links("article");

    Tag ulLinks("ul");
    links << ulLinks << createNavLink("https://github.com/Haletas033", "GitHub");
    links << ulLinks << createNavLink("https://www.youtube.com/@Haletas3D", "YouTube");
    main << links;

    htmlFile << nav.str() << "\n";
    htmlFile << header.str() << "\n";
    htmlFile << main.str() << "\n";

    htmlFile << "</body>\n</html>\n";
    htmlFile.close();

    return 0;
}

int projects() {
    std::ofstream htmlFile("out/projects.html");

    if (!htmlFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    htmlFile << Tag("").Head();

    Tag header("header");

    Tag main("main");
    main.addAttr("class", "container");

    Tag h1("h1");
    h1 << "Projects";
    main << header << h1;

    Tag nav("nav");
    Tag ulNav("ul");
    ulNav << createNavLink("index.html", "Home");
    ulNav << createNavLink("links.html", "Links");
    ulNav << createNavLink("projects.html", "Projects");
    nav << ulNav;

    main << generateRepoLinks("Haletas033");

    htmlFile << nav.str() << "\n";
    htmlFile << header.str() << "\n";
    htmlFile << main.str() << "\n";

    htmlFile << "</body>\n</html>\n";
    htmlFile.close();

    return 0;
}


int main() {
    index();
    links();
    projects();
}
