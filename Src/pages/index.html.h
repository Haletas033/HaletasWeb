//
// Created by halet on 9/17/2025.
//

#ifndef INDEX_H
#define INDEX_H

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
        .put(p("I'm a high school student from Rotorua, New Zealand who enjoys working at a low-level, "
                    "whether that's programming, electronics, or 3D modelling."));

    Tag AboutTheWebsite = Tag("article")
        .put(h3("About The Website:"))
        .put(p("This website was designed to host everything I need. From a portfolio, to docs, to landing pages for other projects. "
            "Everything on this website is generated from C++ including this, using a custom-built DSL for HTML, CSS, and JS. "
                "For more information on how this works and how to use it for your own websites click ")
                .put(Tag("a").addAttr("href", "404").text("HERE"))
                .put(Tag("span").text(".")));

    Tag Skills("article");

    Skills.addAttr("id", "skills").put(Tag("h1").text("Ball pit of skills"));

    main.put(AboutMe).put(AboutTheWebsite).put(Skills);

    main.put(Tag("script").addAttr("src", "scripts/skillBallPit.js"));

    WriteHTML("index.html", HaletasWeb::DefaultHTMLHead(), header, main, "Home", Tag("style").text(misc::extraStyles));
}

#endif //INDEX_H
