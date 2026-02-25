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

    for (auto [docsIndex, docsTitle, docsDescription] : HaletasWeb::docsIndices) {
        docs
            .put(h3()
                .put(Tag("a").addAttr("href", docsIndex)
                    .text(docsTitle)))
            .put(p(docsDescription));
    }

    main
        .put(h1("Documentation"))
        .put(docs);

    WriteHTML("docs.html", HaletasWeb::DefaultHTMLHead(), header, main, "Docs", Tag("style").text(misc::extraStyles));
}

#endif //DOCUMENTATION_H
