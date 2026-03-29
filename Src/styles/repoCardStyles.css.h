//
// Created by halet on 3/2/2026.
//

#ifndef REPOCARDSTYLES_CSS_H
#define REPOCARDSTYLES_CSS_H
#include <string>
#include "../style.h"

inline void RepoCardStyles() {
    //Styles for card
    Style projectCard(".project"); projectCard
        .put("position", "relative")
        .put("transition", "transform 0.3s, background-color 0.3s")
        .put("border-left", "4px solid var(--colour)");

    //Styles for hover
    Style projectCardHover(".project:hover"); projectCardHover
        .put("background-color", "rgba(97, 255, 244, 0.3)")
        .put("transform", "scale(1.05)");

    //Styles for card title
    Style projectTitle(".project .title"); projectTitle
        .put("text-decoration", "underline");

    //Make the card clickable
    Style cardLink(".project .card-link"); cardLink
        .put("position", "absolute")
        .put("inset", "0");

    Style cardAnchors(".project a"); cardAnchors
        .put("position", "relative")
        .put("z-index", "1");

    //otherInfo styles
    Style otherInfo(".otherInfo"); otherInfo
        .put("display", "flex")
        .put("gap", "1em");

    Style otherInfoDivs(".otherInfo div:not(.languageCircle)"); otherInfoDivs
        .put("padding", "4px 12px")
        .put("text-align", "center")
        .put("align-items", "center")
        .put("display","flex")
        .put("border", "2px solid white")
        .put("border-radius", "999px")
        .put("background-color", "#000000");

    //Language styles
    Style language(".language"); language
        .put("gap", "0.5em");

    //LanguageCircle styles
    Style languageCircle(".languageCircle"); languageCircle
        .put("width", "20px")
        .put("height", "20px")
        .put("border", "1px solid white")
        .put("background-color", "var(--colour)")
        .put("border-radius", "50%");
    Builder::Build("repoCards");
}

#endif //REPOCARDSTYLES_CSS_H
