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

    Skills.addAttr("id", "skills").put(Tag("h1").text("Ball pit of skills"));

    main.put(AboutMe).put(Skills);

    main.put(Tag("script").addAttr("src", "scripts/skillBallPit.js"));

    WriteHTML("index.html", header, main, "Home", Tag("style").text(misc::extraStyles));
}

#endif //INDEX_H
