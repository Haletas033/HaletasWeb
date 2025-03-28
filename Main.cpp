#include <iostream>
#include <sstream>
#include <fstream>

#include"Tag.h"

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
    std::ofstream htmlFile("index.html");

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

    Tag div("div");
    div.addAttr("class", "navbar");

    Tag ul("ul");

    h1 << "Hello I'm Haletas";
    h2 << "Award losing C++ dev from New Zealand";

    // Add the content to the body
    htmlFile << h1.str() << "\n";
    htmlFile << h2.str() << "\n";

    ul << createNavLink("index.html", "Home");
    ul << createNavLink("links.html", "Links");
    ul << createNavLink("contact.html", "Contact");

    div << ul;
    htmlFile << div.str() << "\n";

    htmlFile << "</body>\n";
    htmlFile << "</html>\n";

    // Close the file
    htmlFile.close();

    return 0;
}

int links() {
    std::ofstream htmlFile("links.html");

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

    Tag div("div");
    div.addAttr("class", "links");

    Tag ul("ul");

    h1 << "Links";

    htmlFile << h1.str() << "\n";

    ul << createNavLink("https://github.com/Haletas033", "GitHub");
    ul << createNavLink("https://www.youtube.com/@Haletas3D", "YouTube");

    div << ul;
    htmlFile << div.str() << "\n";


    htmlFile << "</body>\n";
    htmlFile << "</html>\n";

    htmlFile.close();

    return 0;
}

int main(){

    index();
    links();

}