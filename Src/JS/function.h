//
// Created by halet on 1/14/2026.
//

#ifndef FUNCTION_H
#define FUNCTION_H

#include "typeTraits.h"
#include "variable.h"

class Function {
public:
    inline static std::string funcJs;
    unsigned int argCount;

    //Helper function
    template <typename ...Args>
    static std::string AddArgs(Args&&... args) {
        std::string result;
        auto functionArgs = std::forward_as_tuple(std::forward<Args>(args)...);
        std::apply([&](auto&&... arg){
            (([&]{
                result+=Variable<>::AddArg(arg);
            }()), ...);
        }, std::forward_as_tuple(std::forward<Args>(args)...));

        //Remove trailing comma
        if (sizeof...(Args)) result.pop_back(); //Only pop_back if there are arguments

        return result;
    }

    template <typename ...Args>
    static void Func(const std::string& name, Args&&... args) {
        JS::currJs = &funcJs;
        std::string result = "function " + name + '(';

        //Add arguments
        result+=AddArgs(args...);

        //Close function
        funcJs+=result + "){\n";
    }

    //For void
    static void Return() {
        *JS::currJs += "return;\n";
    }

    //For literals
    template <typename T>
    static void Return(T&& object) {
        std::string returnStr = "return ";

        if (expectedNextInitialized != nullptr && nextInitializedIsRequired)
            throw std::logic_error("Tried to return before initialization of a const variable.");

        if constexpr (is_vector<std::decay_t<T>>::value) returnStr+= Variable<>::ArrayToString(object) + ";\n";
        else if constexpr (std::is_same_v<std::decay_t<T>, CallResult>) returnStr+= object.use() + ";\n";
        else if constexpr (!std::is_convertible_v<std::decay_t<T>, std::string>) returnStr+= std::to_string(object) + ";\n";
        else if constexpr (std::is_same_v<std::decay_t<T>, JSObject>) returnStr+= std::string(object) + ";\n";
        else returnStr+= object + ";\n";

        *JS::currJs += returnStr;
    }

    //For variables
    template <typename T>
    static void Return(Variable<T> &object) {
        const std::string returnStr = "return " + object.getName() + ";\n";

        if (expectedNextInitialized != nullptr && nextInitializedIsRequired)
            throw std::logic_error("Tried to call return before initialization of a const variable.");

        *JS::currJs+=returnStr;
    }

    static void EndFunc() {
        funcJs+="}\n";
        JS::currJs = &JS::js;
    }

    template <typename ...Args>
    static CallResult Call(const std::string& name, Args&&...args) {
        return CallResult(name + '(' + AddArgs(args...) + ")");
    }

    static CallResult CallBack(const std::string& name) { return CallResult(name); }
};

#endif //FUNCTION_H
