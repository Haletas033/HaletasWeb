//
// Created by halet on 2/22/2026.
//

#ifndef HALETAS_WEB_SETUP_H
#define HALETAS_WEB_SETUP_H
#include "setup.h"


class HaletasWeb final{
public:
    static void SetupFileStructure() {
        Setup::CreateDir("out");
        Setup::CreateDir("scripts");
        Setup::CreateDir("styles");

        Setup::CopyDir("LSIMdocs/imgs", "imgs");
        Setup::CopyDir("skills", "imgs/skills");
    }

    //Default head
    static Tag DefaultHTMLHead() {
        return Tag("head")
            .put(Tag("meta").addAttr("charset", "UTF-8"))
            .put(Tag("meta").addAttr("name", "viewport").addAttr("content", "width=device-width, initial-scale=1.0"))
            .put(Tag("link").addAttr("href", "https://cdn.jsdelivr.net/npm/@picocss/pico@2/css/pico.min.css").addAttr("rel", "stylesheet"))
            /*test stylesheet*/
            .put(Tag("link").addAttr("rel", "stylesheet").addAttr("href", "styles/style.css"))

            //Prism for syntax highlighting
            .put(Tag("link").addAttr("href", "https://cdn.jsdelivr.net/npm/prismjs@1.30.0/themes/prism-tomorrow.css").addAttr("rel", "stylesheet"))
            .put(Tag("script").addAttr("src", "https://cdn.jsdelivr.net/npm/prismjs@1.30.0/prism.min.js"))

            //json
            .put(Tag("script").addAttr("src", "https://cdn.jsdelivr.net/npm/prismjs@1.30.0/components/prism-json.min.js"))
            //C-like
            .put(Tag("script").addAttr("src", "https://unpkg.com/prismjs@1.30.0/components/prism-c.min.js"))

            //Mathjax for latex support
            .put(Tag("script")
                .addAttr("src", "https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-chtml.js"))

            //Matter.js for a bit of physics
            .put(Tag("script")
                    .addAttr("src", "https://cdnjs.cloudflare.com/ajax/libs/matter-js/0.18.0/matter.min.js"));
    }

    static void SetupHaletasWeb() {
        SetupFileStructure();
    }
};

#endif //HALETAS_WEB_SETUP_H
