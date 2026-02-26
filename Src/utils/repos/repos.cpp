//
// Created by halet on 9/17/2025.
//

#include "repos.h"

#include "../../haletasWebSetup.h"
#include "../../setup.h"

std::vector<Dsp> repos::dsps;
std::string repos::cpp;
std::vector<std::string> repos::styles;

//Callback function for curl to handle HTTPS requests
size_t repos::writeCallBack(void* contents, size_t size, size_t nmemb, void* userp) {

    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void initCurl() {
    curl_global_init(CURL_GLOBAL_DEFAULT); //Initialize global curl state
}

void cleanupCurl() {
    curl_global_cleanup(); //Clean up the global curl state
}

//Fetches a users GitHub repos
std::pair<std::string, long> repos::getUrl(const std::string &url) {
    std::string readBuffer;
    long httpCode;

    if (CURL *curl = curl_easy_init()) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        //Error checking
        if (const CURLcode res = curl_easy_perform(curl); res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

        curl_easy_cleanup(curl); //Clean up the curl handle after the request is done
    }



    return std::make_pair(readBuffer, httpCode); //Return the response data
}

std::pair<std::string, long> repos::getDsp(const std::string &repoName) {
    return getUrl("https://raw.githubusercontent.com/Haletas033/" + repoName + "/master/website.dsp");
}

Dsp repos::parseDsp(const std::pair<std::string, long>& dsp) {
    std::function<void(const std::string&)> indexSetter = nullptr;
    std::vector<file>* field = nullptr;
    std::string buff;
    Dsp output;
    bool inComment = false;
    bool inString = false;
    if (dsp.second != 200) return Dsp{};
    for (const char c : dsp.first) {
        if (c == '#' && !inString) inComment = true;
        else if (c == '\n') inComment = false;
        else if (c == '\"') inString = !inString;
        else if ((!isspace(c) && !inComment) || inString) {
            if (c == ':' && !inString) {
                if (buff == "websiteIndex") indexSetter = [&output](const std::string& value) { output.websiteIndex = value; };
                else if (buff == "docsIndex") indexSetter = [&output](const std::string& value) { output.docs.docsIndex = value; };
                else if (buff == "docsTitle") indexSetter = [&output](const std::string& value) { output.docs.docsTitle = value; };
                else if (buff == "docsDescription") indexSetter = [&output](const std::string& value) { output.docs.docsDescription = value; };
                else if (buff == "build") field = &output.build;
                else if (buff == "resources") field = &output.resources;
                else if (buff == "styles") field = &output.styles;
                else throw std::invalid_argument("Unknown argument: " + buff);

                buff = ""; //Clear the buffer
            }
            else if (c == ';' && indexSetter) {
                indexSetter(buff);
                indexSetter = nullptr;
                buff = "";
            }
            else if (c == ';' && field) {
                field->push_back(buff);
                field = nullptr;
                buff = "";
            }
            else if (c == ',' && field) {
                field->push_back(buff);
                buff = "";
            }
            else buff+=c;
        }
    }

    return output;
}

void repos::generateRepoCard(const nlohmann::json& repo, Tag& container, const Dsp& dsp) {
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
    Tag website = p("No website available");
    Tag docs = p("No docs available");
    if (dsp.websiteIndex != "")
         website = Tag("a").addAttr("href", dsp.websiteIndex).put(p("Website"));
    if (dsp.docs.docsIndex != "")
         docs = Tag("a").addAttr("href", dsp.docs.docsIndex).put(p("Docs"));
    Tag otherInfo = p(starsStr + " Stars | " + forksStr + " Forks | " + language);

    Tag article("article");
    article.addAttr("class", "project").addAttr("id", repoName)
            .put(link.put(h2(repoName)))
            .put(description)
            .put(website)
            .put(docs)
            .put(otherInfo);

    container.put(article);
}

void repos::updateDocs(const Dsp& dsp) {
    if (dsp.docs.docsIndex != "")
        HaletasWeb::docsIndices.push_back(dsp.docs);
}

Tag repos::updateStyles() {
    Tag updatedHead = HaletasWeb::DefaultHTMLHead();
    for (auto style : styles) {
        updatedHead
        .put(Tag("link").addAttr("rel", "stylesheet").addAttr("href", style));
    }
    return updatedHead;
}

void repos::generateProjectsCPP() {
    Setup::CreateDir("../projectBuild/Src");
    std::ofstream outFile("projectBuild/Src/main.cpp");
    std::string output = "#include \"haletasWebSetup.h\"\n";
    //Generate includes
    for (auto dsp : dsps) {
        for (auto f : dsp.build) {
            //Remove Website dir
            if (f.find("Website/Src/") == 0)
                f = f.substr(12);
            if (f != "")
                output += "#include \"" + f + "\"\n";
        }
        for (auto f : dsp.styles) {
            //Remove Website dir
            if (f.find("Website/Src/") == 0)
                f = f.substr(12);
            if (f != "")
                output += "#include \"" + f + "\"\n";
        }
    }

    output += "int main() {\n";
    output += "HaletasWeb::SetupHaletasWeb();\n";

    //Generate function calls
    for (const auto& dsp : dsps) {
        for (auto f : dsp.build) {
            if (!f.empty()) {
                //Remove Website dir
                if (f.find("Website/") == 0)
                    f = f.substr(8);
                std::string name = f.substr(f.find_last_of("/") + 1);
                name = name.substr(0, name.find_first_of('.'));
                output += name + "();\n";
            }
        }
        for (auto f : dsp.styles) {
            if (!f.empty()) {
                //Remove Website dir
                if (f.find("Website/") == 0)
                    f = f.substr(8);
                std::string name = f.substr(f.find_last_of("/") + 1);
                name = name.substr(0, name.find_first_of('.'));
                output += name + "();\n";
            }
        }
    }

    outFile << output + "}";
}

void repos::loadProjectHeaders() {
    for (const auto& dsp : dsps) {
        for (const auto& f : dsp.build) {
            //Remove Website dir
            std::string cleanedFile = f;
            if (f.find("Website/") == 0)
                cleanedFile = f.substr(8);
            std::filesystem::create_directories("projectBuild/" + cleanedFile.substr(0, cleanedFile.find_last_of("/")));
            std::ofstream outFile("projectBuild/" + cleanedFile);
            outFile << getUrl("https://raw.githubusercontent.com/Haletas033/" + dsp.repoName + "/master/" + f).first;
        }

        for (const auto& f : dsp.styles) {
            //Remove Website dir
            std::string cleanedFile = f;
            if (f.find("Website/") == 0)
                cleanedFile = f.substr(8);
            std::filesystem::create_directories("projectBuild/" + cleanedFile.substr(0, cleanedFile.find_last_of("/")));
            std::ofstream outFile("projectBuild/" + cleanedFile);
            outFile << getUrl("https://raw.githubusercontent.com/Haletas033/" + dsp.repoName + "/master/" + f).first;

            //Get rid of Src/ for styles
            if (cleanedFile.find("Src/") == 0)
                cleanedFile = cleanedFile.substr(4);

            //Remove .h for styles
            if (size_t lastExtensionPos = cleanedFile.rfind('.'); lastExtensionPos != std::string::npos)
                cleanedFile = cleanedFile.substr(0, lastExtensionPos);
            styles.push_back(cleanedFile);
        }
    }
}

void repos::getFileStructure(std::string path, const std::string& repoName) {
    const std::string response = getUrl("https://api.github.com/repos/Haletas033/" + repoName + "/contents/" + path).first;
    if (response.empty()) return;
    nlohmann::json json = nlohmann::json::parse(response);

    std::string cleanedFile = path;
    if (path.find("Website/") == 0)
        cleanedFile = path.substr(8);

    for (const auto &entry : json) {
        if (entry["type"] == "dir") {
            std::filesystem::create_directories("projectBuild/" + cleanedFile + '/' + std::string(entry["name"]));
            getFileStructure(path + '/' + std::string(entry["name"] ) + "/", repoName);
        } else if (entry["type"] == "file") {
            std::cout << "projectBuild/" + path + std::string(entry["name"]) << '\n';
            std::ofstream("projectBuild/" + cleanedFile + "/" + std::string(entry["name"])) << getUrl("https://raw.githubusercontent.com/Haletas033/" + repoName + "/master/" + path + std::string(entry["name"])).first;
        }
    }
}

void repos::loadResources() {
    for (const auto& dsp : dsps) {
        for (const auto& f : dsp.resources) {
            //Remove Website dir
            std::string cleanedFile = f;
            if (f.find("Website/") == 0)
                cleanedFile = f.substr(8);
            std::filesystem::create_directories("projectBuild/" + cleanedFile.substr(0, cleanedFile.find_last_of("/")));
            getFileStructure(f, dsp.repoName);
        }
    }
}

//Function to parse the GitHub JSON response and generate HTML links
Tag repos::getRepoData(const std::string& username) {

    initCurl();

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
        std::string repoName = repo["name"];

        Dsp dsp;
        dsp = parseDsp(getDsp(repoName));
        dsp.repoName = repoName;
        generateRepoCard(repo, container, dsp);
        updateDocs(dsp);
        dsps.push_back(dsp);

    }

    generateProjectsCPP();

    loadProjectHeaders();

    loadResources();

    cleanupCurl();

    return container;
}
