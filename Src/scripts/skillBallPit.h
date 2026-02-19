//
// Created by halet on 1/5/2026.
//

#ifndef SKILLBALLPIT_H
#define SKILLBALLPIT_H
#include <iostream>

#endif //SKILLBALLPIT_H

#include "../JS/js.h"


//Functions

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
    Variable<> world(LET, "world"); world = engine["world"];

    std::cout << JS::js;
}