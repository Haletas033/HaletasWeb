#include "haletasWebSetup.h"
#include "pages/404.html.h"
#include "pages/index.html.h"
#include "pages/links.html.h"
#include "pages/projects.html.h"
#include "pages/documentation.html.h"
#include "pages/docs/LSIMdocs.h"
#include "scripts/skillBallPit.js.h"
#include "styles/style.css.h"

int main() {
    //Setup
    HaletasWeb::SetupHaletasWeb();

    index();
    //_404();
    // links();
    // projects();
    // documentation();
    // docs::LSIMDocs();
    // Stylesheet();
    //skillBallPit();
}
