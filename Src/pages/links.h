//
// Created by halet on 9/17/2025.
//

#ifndef LINKS_H
#define LINKS_H

#include "../Tag.h"
#include "../utils/misc/misc.h"
#include "../utils/nav/nav.h"

inline void links() {
    Tag header("header");

    header.put(nav::buildNavbar());

    Tag main("main");
    main.addAttr("class", "container")
        .put(h1("Links"));

    Tag links("article");
    links.put(
        Tag("ul")
            .put(nav::createNavLink("https://github.com/Haletas033", "GitHub"))
            .put(nav::createNavLink("https://www.youtube.com/@Haletas3D", "YouTube"))
    );

    main.put(links);

    WriteHTML("links.html", header, main, "Links", Tag("style").text(misc::extraStyles));
}

#endif //LINKS_H
