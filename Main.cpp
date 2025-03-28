
#include <inja/inja.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

//Header files containing html files as a string
#include"Home.h"
#include"Links.h"

int main() {
    inja::Environment env;

    //Data (contains links to socials atm)
    nlohmann::json data;
    data["github"] = "https://github.com/Haletas033";
    data["youtube"] = "https://www.youtube.com/@Haletas3D";


    //Loops through data and adds it as a html line to links
   for (auto& [key, value] : data.items())
   {
       links_html += "<li><a href=\"" + value.get<std::string>() + "\">" + key + "</a></li>\n";
   }

   //Closes the html file after adding data
   links_html += R"(
                        </ul>
                    </div>
            </body>
        </html>
    )";

    

   //Creates a home.html file and writes the home html string to it
    std::string result_home = env.render(home_html, data);

    std::ofstream file_home("home.html");
    if (file_home.is_open()) {
        file_home << result_home;
        file_home.close();
        std::cout << "HTML saved to home.html" << std::endl;
    }
    else {
        std::cerr << "Error: Could not open file for writing." << std::endl;
    }



    //Creates a links.html file and writes the links html string to it
    std::string result_links = env.render(links_html, data);

    std::ofstream file_links("links.html");
    if (file_links.is_open()) {
        file_links << result_links;
        file_links.close();
        std::cout << "HTML saved to links.html" << std::endl;
    }
    else {
        std::cerr << "Error: Could not open file for writing." << std::endl;
    }
    return 0;
}
