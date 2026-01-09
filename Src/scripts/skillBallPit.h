//
// Created by halet on 1/5/2026.
//

#ifndef SKILLBALLPIT_H
#define SKILLBALLPIT_H
#include <iostream>

#endif //SKILLBALLPIT_H

#include "../JS/js.h"

#define test skillBallPit

inline void t() {
    Console::Log("Hello, World");

    Variable Engine(CONSTANT, "Engine");                   Engine = JSObject("Matter.Engine");
    Variable Render(CONSTANT, "Render");                   Render = JSObject("Matter.Render");
    Variable Runner(CONSTANT, "Runner");                   Runner = JSObject("Matter.Runner");
    Variable Bodies(CONSTANT, "Bodies");                   Bodies = JSObject("Matter.Bodies");
    Variable Common(CONSTANT, "Common");                   Common = JSObject("Matter.Common");
    Variable Mouse(CONSTANT, "Mouse");                     Mouse = JSObject("Matter.Mouse");
    Variable MouseConstraint(CONSTANT, "MouseConstraint"); MouseConstraint = JSObject("Matter.MouseConstraint");
    Variable Composite(CONSTANT, "Composite");             Composite = JSObject("Matter.Composite");
    Variable Composites(CONSTANT, "Composites");           Composites = JSObject("Matter.Composites");

    Variable engine(LET, "engine");                        engine = JSObject("Engine.create()");
    Variable world(LET, "world");                          engine = JSObject("engine.world()");

    Variable foo(LET, "foo"); foo = 10;
    Variable bar(LET, "bar"); bar = 20;

    foo+bar;

    Variable<std::vector<std::string>> skills(CONSTANT, "skills"); skills = std::vector<std::string>{"CXX", "C", "CSHARP", "PYTHON", "HTML", "CSS", "JS"};

    Console::Log(skills);

    foo("bar", 2.5, "China", bar, JSObject("JSThing"), std::vector{1,2,3,4,5,6,7,8,9,10});

    std::cout << JS::js;
}

inline void test1() { //Should succeed
    //Test of calling function of variable
    std::cout << "Test 1" << '\n';
    Variable foo(LET, "foo"); foo = JSObject("module");
    Variable bar(LET, "bar"); foo = "Hello, World";
    foo("func", 1.2, "test", 15, std::vector{1,2,3,4,5,6,7,8,9,10}, bar);

    std::cout << JS::js;
}

inline void test2() { //Should fail because it is called before const bar is defined
    //Test of calling function of variable
    std::cout << "Test 2" << '\n';
    Variable foo(LET, "foo"); foo = JSObject("module");
    Variable bar(CONSTANT, "bar");
    foo("func", 1.2, "test", 15, std::vector{1,2,3,4,5,6,7,8,9,10});

    std::cout << JS::js;
}

inline void test() { //Should fail because the function name into a string type
    //Test of calling function of variable
    Variable foo(LET, "foo"); foo = JSObject("module");
    Variable bar(LET, "bar"); foo = "Hello, World";
    foo(123, 1.2, "test", 15, std::vector{1,2,3,4,5,6,7,8,9,10});

    std::cout << JS::js;
}



