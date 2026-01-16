//
// Created by halet on 1/14/2026.
//

#ifndef FUNCTION_H
#define FUNCTION_H

#include "variable.h"

class Function {
public:
    inline static std::string funcJs;
    template <typename ...Args>
    static void Func(const std::string& name, Args&&... args) {
        JS::currJs = &funcJs;
        std::string result = "function " + name + '(';

        if (expectedNextInitialized != nullptr && nextInitializedIsRequired)
            throw std::logic_error(std::string("Tried to declare function "+ name + "before the initialization of a const variable"));

        //Add arguments
        auto functionArgs = std::forward_as_tuple(std::forward<Args>(args)...);
        std::apply([&](auto&&... arg){
            (([&]{
                result+=Variable<>::AddArg(arg);
            }()), ...);
        }, std::forward_as_tuple(std::forward<Args>(args)...));

        //Close function
        if (sizeof...(Args)) result.pop_back(); //Only pop_back if there are arguments
        funcJs+=result + "){\n";
    }

    void Return(); //not implemented
    static void EndFunc() {
        funcJs+="}\n";
        JS::currJs = &JS::js;
    }
};

#endif //FUNCTION_H
