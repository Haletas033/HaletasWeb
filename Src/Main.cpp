#include "pages/index.h"
#include "pages/links.h"
#include "pages/projects.h"
#include "pages/documentation.h"
#include "pages/docs/LSIMdocs.h"

int main() {
    index();
    links();
    projects();
    documentation();
    docs::LSIMDocs();
}