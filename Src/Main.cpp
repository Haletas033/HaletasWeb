#include <iostream>
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>/

//Custom made class to create HTML tag programatically
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
    std::string response = getRepos(username); //Fetch JSON data from GitHub API

    Tag ul("ul"); //Create an unordered list tag

    //Error checking
    if (response.empty()) {
        ul << Tag("li") << "Failed to load repositories.";
        return ul; //Return empty list if no data
    }

    auto repos = nlohmann::json::parse(response); //Parse JSON response

    for (const auto& repo : repos) {
        std::string repoName = repo["name"];
        std::string repoUrl = repo["html_url"];

        Tag li("li");
        Tag a("a");
        a.addAttr("href", repoUrl);
        a << repoName;

        li << a;

        ul << li; //Append each repo to the unordered list
    }

    return ul; //Return the list of repositories
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
    // Open a file for writing the HTML content
    std::ofstream htmlFile("Build/index.html");

    if (!htmlFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    // Start writing the HTML structure
    htmlFile << "<!DOCTYPE html>\n";
    htmlFile << "<html lang=\"en\">\n";
    htmlFile << "<head>\n";
    htmlFile << "    <meta charset=\"UTF-8\">\n";
    htmlFile << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    htmlFile << "    <title>Haletas - Index</title>\n";
    htmlFile << "    <link rel=\"stylesheet\" href=\"styles.css\">\n";
    htmlFile << "</head>\n";
    htmlFile << "<body>\n";

    Tag h1("h1");
    Tag h2("h2");
    Tag h3("h3");
    Tag h3a("h3");
    Tag hr("hr");
    
    Tag p("p");
    Tag p1("p");

    Tag div("div");
    div.addAttr("class", "navbar");

    Tag ul("ul");

    h1 << "Hello I'm Haletas";
    h2 << "Award losing C++ dev from New Zealand";
    h3a << "About me:";
    h3 << "Skills:";

    p1 << "Hello my name is Alex Curran and I'm a high school stduent (currently attending Lakes High, Rotorua) and I am a enjoyer of programming, 3D modeling and electronics.";

    p << "- C++<br>";
    p << "- C#<br>";
    p << "- Python<br>";
    p << "- HTML<br>";
    p << "- CSS<br>";
    p << "- JS<br>";


    // Add the content to the body
    htmlFile << h1.str() << "\n";
    htmlFile << h2.str() << "\n";
    htmlFile << hr.str() << "\n";
    htmlFile << h3a.str() << "\n";
    htmlFile << p1.str() << "\n";
    htmlFile << hr.str() << "\n";
    htmlFile << h3.str() << "\n";
    htmlFile << p.str() << "\n";
    htmlFile << hr.str() << "\n";

    ul << createNavLink("index.html", "Home");
    ul << createNavLink("links.html", "Links");
    ul << createNavLink("projects.html", "Projects");

    div << ul;
    htmlFile << div.str() << "\n";

    htmlFile << "</body>\n";
    htmlFile << "</html>\n";

    // Close the file
    htmlFile.close();

    return 0;
}

int links() {
    std::ofstream htmlFile("Build/links.html");

    if (!htmlFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    // Start writing the HTML structure
    htmlFile << "<!DOCTYPE html>\n";
    htmlFile << "<html lang=\"en\">\n";
    htmlFile << "<head>\n";
    htmlFile << "    <meta charset=\"UTF-8\">\n";
    htmlFile << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    htmlFile << "    <title>Haletas - Index</title>\n";
    htmlFile << "    <link rel=\"stylesheet\" href=\"styles.css\">\n";
    htmlFile << "</head>\n";
    htmlFile << "<body>\n";


    Tag div1("div");
    div1.addAttr("class", "navbar");

    Tag ul1("ul");

    Tag h1("h1");

    Tag div("div");
    div.addAttr("class", "links");

    Tag ul("ul");

    h1 << "Links";

    htmlFile << h1.str() << "\n";

    ul1 << createNavLink("index.html", "Home");
    ul1 << createNavLink("links.html", "Links");
    ul1 << createNavLink("projects.html", "Projects");

    ul << createNavLink("https://github.com/Haletas033", "GitHub");
    ul << createNavLink("https://www.youtube.com/@Haletas3D", "YouTube");

    div1 << ul1;
    div << ul;

    htmlFile << div1.str() << "\n";
    htmlFile << div.str() << "\n";


    htmlFile << "</body>\n";
    htmlFile << "</html>\n";

    htmlFile.close();

    return 0;
}

int projects() {
    std::ofstream htmlFile("Build/projects.html");

    if (!htmlFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    // Start writing the HTML structure
    htmlFile << "<!DOCTYPE html>\n";
    htmlFile << "<html lang=\"en\">\n";
    htmlFile << "<head>\n";
    htmlFile << "    <meta charset=\"UTF-8\">\n";
    htmlFile << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    htmlFile << "    <title>Haletas - Index</title>\n";
    htmlFile << "    <link rel=\"stylesheet\" href=\"styles.css\">\n";
    htmlFile << "</head>\n";
    htmlFile << "<body>\n";

    Tag div1("div");
    div1.addAttr("class", "navbar");

    Tag ul("ul");

    Tag h1("h1");

    h1 << "Projects";

    ul << createNavLink("index.html", "Home");
    ul << createNavLink("links.html", "Links");
    ul << createNavLink("projects.html", "Projects");

    div1 << ul;

    htmlFile << h1.str() << "\n";

    Tag div("div");
    div.addAttr("class", "projects");

    

    div << generateRepoLinks("Haletas033");

    htmlFile << div1.str() << "\n";
    htmlFile << div.str() << "\n";


    htmlFile << "</body>\n";
    htmlFile << "</html>\n";

    htmlFile.close();

    return 0;
}

int main(){

    index();
    links();
    projects();
}