//
// Created by halet on 2/22/2026.
//

#ifndef INC_404_HTML_H
#define INC_404_HTML_H

#include "../Tag.h"
#include "../utils/misc/misc.h"
#include "../utils/nav/nav.h"

extern HaletasWeb site;

inline void _404() {
    Tag header("header");
    header.put(nav::buildNavbar());

    Tag hr("hr");

    Tag main("main");
    main.addAttr("class", "container");

    main.put(h1("404"));

    WriteHTML("404.html", site.GetHead(), header, main, "404", Tag("style").text(misc::extraStyles));
}

#endif //INC_404_HTML_H
