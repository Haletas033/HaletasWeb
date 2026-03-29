[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

# HaletasWeb

HaletasWeb is my personal website built entirely in C++ which is the main focus on the project
With full support for HTML, CSS, and JS.

---

## Project Structure:

The main files in the project are tag.h, style.h and the JS dir.
Each of these is either a header or contains headers so you can simply copy them into your own project with no linking required.
Besides these are a few helper classes for getting data from other repos and more. However, these, at the moment, are specific to my account
so you may have to modify them to work for your needs though I plan to make them non-specific headers soon.
Other than that the project contains html.h, css.h, and js.h files which are built into a full website

---

## Features:
* Builder pattern for generated html and css
* Template metaprogramming for generated JS that reads like real code
* Pulls data from every repo
* reads a website.dsp file from supported repos to download there C++ websites and build them

---

## Examples:
### HTML Example:
```c++
inline void index() {
    Tag header("header");
    header.put(nav::buildNavbar());
    
    Tag main("main");
    main.addAttr("class", "container");

    main
        .put(h1("Hello, World!"))
        .put(h2("Goodbye, World!"))
        .put(hr);

    Tag coolStuff = Tag("article")
        .put(h3("WOW!"))
        .put(p("HTML am I right..."));
        
    main.put(coolStuff);
    WriteHTML("index.html", HaletasWeb::DefaultHTMLHead(), header, main, "Home", Tag("style").text(misc::extraStyles));
}
```
### Style Example:
```c++
inline void Stylesheet() {
    Style h1("h1"); h1
        .put("color", "red");

    Builder::Build("style");
}
```

### JS Example (snippet from skillBallPit.js.h won't compile on it's own):
```c++
inline void OnStackCreateObject() {
    Function onStackCreateObject{};

    Variable<> x(CONSTANT, "x"); x.asArg(onStackCreateObject);
    Variable<> y(CONSTANT, "y"); y.asArg(onStackCreateObject);
    Variable<> skills(CONSTANT, "skills"); skills.asArg(onStackCreateObject);
    Variable<> Common(CONSTANT, "Common"); Common.asArg(onStackCreateObject);
    Variable<> Bodies(CONSTANT, "Bodies"); Bodies.asArg(onStackCreateObject);

    onStackCreateObject.Func("onStackCreateObject", x, y, skills, Common, Bodies);
        Variable<> radius(CONSTANT, "radius"); radius = Common("random", 15, 30);
        Variable<> skill(CONSTANT, "skill"); skill = JSObject("skills[Math.floor(Common.random(0, skills.length))]");
        Variable<> texture(CONSTANT, "texture"); texture = JSObject(R"('./imgs/skills/' + skill + '.png')");

        Variable<> body(LET, "body"); body = 0;

        Cond::If(skill == "JS");
            body = Bodies("rectangle", x, y, radius*2, radius*2,
                JSObject("{restitution: 0.6, friction: 0.1, render:{sprite:{texture: texture}}}")); //Technically could be written without using JSObject
        Cond::Else();
            body = Bodies("circle", x, y, radius,
                JSObject("{restitution: 0.6, friction: 0.1, render:{sprite:{texture: texture}}}"));

        Cond::EndIf();

        Variable<> img(LET, "img"); img = JSObject("new Image()");

        img>>"src" = texture;
        img>>"onload" = Function::ArrowCall("resizeImage", body, radius, img);

        Function::Return(body);
    onStackCreateObject.EndFunc();
}

inline void skillBallPit() {
    //Function declarations
    ResizeImage();
    OnStackCreateObject();

    Console::Log("Hello, World");
    Variable<> Engine(CONSTANT, "Engine"); Engine = JSObject("Matter.Engine");
    Variable<> Render(CONSTANT, "Render"); Render = JSObject("Matter.Render");
    Variable<> Runner(CONSTANT, "Runner"); Runner = JSObject("Matter.Runner");
    Variable<> Bodies(CONSTANT, "Bodies"); Bodies = JSObject("Matter.Bodies");
    Variable<> Common(CONSTANT, "Common"); Common = JSObject("Matter.Common");
    Variable<> Mouse(CONSTANT, "Mouse"); Mouse = JSObject("Matter.Mouse");
    Variable<> MouseConstraint(CONSTANT, "MouseConstraint"); MouseConstraint = JSObject("Matter.MouseConstraint");
    Variable<> Composite(CONSTANT, "Composite"); Composite = JSObject("Matter.Composite");
    Variable<> Composites(CONSTANT, "Composites"); Composites = JSObject("Matter.Composites");
    Build("skillBallPit");
}
```
#### Note: FUll docs coming soon

## Contributions / Feedback:
All bug reports, contribution, and feedback are supported. Just open an issue and I should get back to you.

## License

This project is licensed under the [MIT License](LICENSE).
