//
// Created by halet on 9/17/2025.
//

#ifndef LSIMDOCS_H
#define LSIMDOCS_H

#include <filesystem>
#include <fstream>
#include <string.h>
#include <cmark.h>

#include "../../Tag.h"
#include "../../utils/nav/nav.h"

namespace docs {
    inline void LSIMDocs() {
        Tag header("header");
        header.put(nav::buildNavbar());

        Tag main("main");
        main.addAttr("class", "container");

        std::string html;

        std::vector<std::filesystem::path> mds;

        for (const auto &currMd : std::filesystem::directory_iterator("LSIMdocs")) {
            mds.push_back(currMd);
        }

        std::sort(mds.begin(), mds.end());

        for (const auto &md : mds) {
            std::ifstream file(md);

            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string mdStr = buffer.str();

            mdStr = misc::convertToLatex(mdStr);

            html += cmark_markdown_to_html(mdStr.c_str(), strlen(mdStr.c_str()), CMARK_OPT_DEFAULT | CMARK_OPT_UNSAFE);
            html += "<hr/>";


            file.close();
        }

        main
            .text(html);

        WriteHTML("LSIMdocs.html", site.GetHead(), header, main, "Docs", Tag("style").text(misc::extraStyles));
    }
}

#endif //LSIMDOCS_H
