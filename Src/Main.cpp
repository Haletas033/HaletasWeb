#include "pages/index.html.h"
#include "pages/links.html.h"
#include "pages/projects.html.h"
#include "pages/documentation.html.h"
#include "pages/docs/LSIMdocs.h"
#include "scripts/skillBallPit.js.h"
#include "styles/style.css.h"

int main() {

    index();
    links();
    projects();
    documentation();
    docs::LSIMDocs();
    Stylesheet();
    skillBallPit();
}
