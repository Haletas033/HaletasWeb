//
// Created by halet on 9/17/2025.
//

#ifndef PROJECTS_H
#define PROJECTS_H

#include "../Tag.h"
#include "../utils/nav/nav.h"
#include "../utils/repos/repos.h"

inline void projects() {
    Tag header("header");

    header.put(nav::buildNavbar());

    Tag main("main");
    main.addAttr("class", "container");

    main.put(h1("Projects"));

    main.put(repos::generateRepoLinks("Haletas033"));

    WriteHTML("projects.html", header, main, "Projects", Tag("style").text(misc::extraStyles));
}

#endif //PROJECTS_H
