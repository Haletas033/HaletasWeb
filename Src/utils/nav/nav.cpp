//
// Created by halet on 9/17/2025.
//

#include "nav.h"

//Function to make a navbar
Tag nav::createNavLink(const std::string& href, const std::string& text)
{
    return Tag("li").put(
        Tag("a").addAttr("href", href).text(text)
    );
}

Tag nav::buildNavbar() {
    Tag nav("nav");
    Tag ul("ul");
    ul
        .put(createNavLink("index.html", "Home"))
        .put(createNavLink("links.html", "Links"))
        .put(createNavLink("projects.html", "Projects"))
        .put(createNavLink("docs.html", "Docs"));
    nav.put(ul);
    return nav;
}
