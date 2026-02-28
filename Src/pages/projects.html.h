//
// Created by halet on 9/17/2025.
//

#ifndef PROJECTS_H
#define PROJECTS_H

#include "../style.h"
#include "../Tag.h"
#include "../utils/nav/nav.h"
#include "../utils/repos/repos.h"

inline void projects() {
    Tag header("header");

    header.put(nav::buildNavbar());

    Tag main("main");
    main.addAttr("class", "container");

    main.put(h1("Projects"));

    main.put(repos::getRepoData("Haletas033"));

    //Styles for card
    Style projectCard(".project"); projectCard.put("position", "relative");
    Style projectTitle(".project .title"); projectTitle.put("text-decoration", "underline");
    Style cardLink(".project .card-link"); cardLink.put("position", "absolute").put("inset", "0");
    const std::string clickableCard = Builder::BuildInline();

    WriteHTML("projects.html", repos::updateStyles(), header, main, "Projects", Tag("style").text(misc::extraStyles + clickableCard));
}

#endif //PROJECTS_H
