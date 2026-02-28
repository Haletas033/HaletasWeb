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
    Style projectCard(".project"); projectCard
        .put("position", "relative")
        .put("border-left", "4px solid var(--colour)");

    //Styles for hover
    Style projectCardHover(".project:hover"); projectCardHover
        .put("transition", "transform 0.3s, background-color 0.3s")
        .put("background-color", "rgba(97, 255, 244, 0.3)")
        .put("transform", "scale(1.05)");

    //Styles for card title
    Style projectTitle(".project .title"); projectTitle
        .put("text-decoration", "underline");

    //Make the card clickable
    Style cardLink(".project .card-link"); cardLink
        .put("position", "absolute")
        .put("inset", "0");

    //otherInfo styles
    Style otherInfo(".otherInfo"); otherInfo
        .put("display", "flex")
        .put("gap", "1em");

    Style otherInfoP(".otherInfo div p"); otherInfoP
        .put("margin", "0");

    Style otherInfoDivs(".otherInfo div"); otherInfoDivs
        .put("padding", "4px 12px")
        .put("align-items", "center")
        .put("display","flex")
        .put("border", "1px solid white")
        .put("border-radius", "999px")
        .put("background-color", "#000000");
    const std::string repoCardStyles = Builder::BuildInline();

    WriteHTML("projects.html", repos::updateStyles(), header, main, "Projects", Tag("style").text(misc::extraStyles + repoCardStyles));
}

#endif //PROJECTS_H
