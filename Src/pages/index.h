//
// Created by halet on 9/17/2025.
//

#ifndef INDEX_H
#define INDEX_H

#include <array>

#include "../Tag.h"
#include "../utils/misc/misc.h"
#include "../utils/nav/nav.h"

inline void index() {
    Tag header("header");
    header.put(nav::buildNavbar());

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

    Skills.put(h3("Skills:"));

    Tag ul("ul");

    for (const std::string& lang : std::array<std::string, 6>{"C++", "C#", "Python", "HTML", "CSS", "JS"}) {
        ul.put(Tag("li").text(lang));
    }

    Skills.put(ul);
    main.put(AboutMe).put(Skills);

    WriteHTML("index.html", header, main, "Home", Tag("style").text(misc::extraStyles));
}

#endif //INDEX_H
