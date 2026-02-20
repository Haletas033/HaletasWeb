//
// Created by halet on 1/5/2026.
//

#ifndef SKILLBALLPIT_H
#define SKILLBALLPIT_H
#include <iostream>

#include "../JS/js.h"

inline void OnStackCreateObject(Variable<>& Common, Variable<>& Bodies) {
    Function onStackCreateObject{};

    std::string temp;
    JS::currJs = &temp;

    Variable<> x(CONSTANT, "x"); x.asArg(onStackCreateObject);
    Variable<> y(CONSTANT, "y"); y.asArg(onStackCreateObject);

    Function::Func("onStackCreateObject", x, y);
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
        img>>"onload" = JSObject("function() {body.render.sprite.xScale = (radius * 2) / img.width;body.render.sprite.yScale = (radius * 2) / img.width;}");

        Function::Return(body);
    Function::EndFunc();
}

inline void skillBallPit() {
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

    Variable<> engine(LET, "engine"); engine = Engine("create");
    Variable<> world(LET, "world"); world = engine>>"world";

    Variable<> renderOptions(LET, "renderOptions"); renderOptions = JSObject("new Object()");
    renderOptions>>"width" = 800;
    renderOptions>>"height" = 600;
    renderOptions>>"showAngleIndicator" = false;
    renderOptions>>"wireframes" = false;

    Variable<> renderer (LET, "renderer"); renderer = JSObject("new Object()");
    renderer>>"element" = JSObject("document.getElementById(\"skills\")");
    renderer>>"engine" = engine;
    renderer>>"options" = renderOptions;

    Variable<> render(LET, "render"); render = Render("create", renderer);

    Variable<> mouse(LET, "mouse"); mouse = Mouse("create", render>>"canvas");

    Variable constraintOptions(LET, "constraintOptions"); constraintOptions = JSObject("new Object");
    constraintOptions>>"stiffness" = 0.2f;
    constraintOptions>>"render" = JSObject("{ visible: false }"); //Technically this could be written without JSObject

    Variable<> mouseConstraintOptions(LET, "mouseConstraintOptions"); mouseConstraintOptions = JSObject("new Object");
    mouseConstraintOptions>>"mouse" = mouse;
    mouseConstraintOptions>>"constraint" = constraintOptions;

    Variable<> mouseConstraint(LET, "mouseConstraint"); mouseConstraint = MouseConstraint("create", engine, mouseConstraintOptions);

    //Sync mouse with renderer
    render>>"mouse" = mouse;

    Variable<> skills(CONSTANT, "skills"); skills = std::vector<std::string>{"CXX", "C", "CSHARP", "PYTHON", "HTML", "CSS", "JS"};

    Variable<> ground(LET, "ground"); ground = Bodies("rectangle", 400, 610, 800, 60, JSObject("{ isStatic: true }"));
    Variable<> wallLeft(LET, "wallLeft"); wallLeft = Bodies("rectangle", 0, 610, 60, 800, JSObject("{ isStatic: true }"));
    Variable<> wallRight(LET, "wallRight"); wallRight = Bodies("rectangle", 800, 610, 60, 800, JSObject("{ isStatic: true }"));

    OnStackCreateObject(Common, Bodies);

    Variable<> stack(LET, "stack"); stack = Composites("stack", 100, 0, 10, 8, 10, 10,
        Function::CallBack("onStackCreateObject"));

    Composite("add", world, std::vector{ground, wallLeft, wallRight, stack});
    Composite("add", world, mouseConstraint);

    Render("run", render);

    Variable<> runner(LET, "runner"); runner = Runner("create");

    Runner("run", runner, engine);


    std::cout << Function::funcJs;
    std::cout << JS::js;
}

#endif //SKILLBALLPIT_H
