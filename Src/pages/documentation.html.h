//
// Created by halet on 9/17/2025.
//

#ifndef DOCUMENTATION_H
#define DOCUMENTATION_H

#include "../Tag.h"
#include "../utils/nav/nav.h"

inline void documentation() {
    Tag header("header");
    header.put(nav::buildNavbar());

    Tag hr("hr");

    Tag main("main");
    main.addAttr("class", "container");

    Tag docs("article");

    docs
        .put(h3()
            .put(Tag("a").addAttr("href", "LSIMdocs.html")
                .text("L-SIM ENGINE Documentation")))
        .put(p("Documentation for how to use the L-SIM ENGINE"));

    main
        .put(h1("Documentation"))
        .put(docs);

    WriteHTML("docs.html", header, main, "Docs", Tag("style").text(misc::extraStyles));
}

#endif //DOCUMENTATION_H
