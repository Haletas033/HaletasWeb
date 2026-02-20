//
// Created by halet on 1/7/2026.
//

#ifndef CONSOLE_H
#define CONSOLE_H

#include "jsCore.h"
#include "typeTraits.h"
#include "variable.h"

class Console {
private:

    //For literals
    template <typename T, typename = std::enable_if_t<!is_variable<std::decay_t<T>>::value>>
    static void ConsoleBase(T&& object, const std::string& funcName) {
        const std::string consoleFuncName = "console." + funcName;

        if (expectedNextInitialized != nullptr && nextInitializedIsRequired)
            throw std::logic_error("Tried to call " + consoleFuncName + " before initialization of a const variable.");

        if constexpr (is_vector<std::decay_t<T>>::value) *JS::currJs+=consoleFuncName + "(" + Variable<>::ArrayToString(object) + ");\n";
        else if constexpr (std::is_same_v<std::decay_t<T>, CallResult>) *JS::currJs+=std::string(consoleFuncName + "(") + object.use() + ");\n";
        else if constexpr (!std::is_convertible_v<std::decay_t<T>, std::string>) *JS::currJs+=std::string(consoleFuncName + "(") + std::to_string(object) + ");\n";
        else if constexpr (std::is_same_v<std::decay_t<T>, JSObject>) *JS::currJs+=std::string(consoleFuncName + "(") + std::string(object) + ");\n";
        else *JS::currJs+=std::string(consoleFuncName + "(\"") + object + "\");\n";
    }

    //For variables
    template <typename T>
    static void ConsoleBase(Variable<T> &object, const std::string& funcName) {
        const std::string consoleFuncName = "console." + funcName;

        if (expectedNextInitialized != nullptr && nextInitializedIsRequired)
            throw std::logic_error("Tried to call " + consoleFuncName + " before initialization of a const variable.");

        *JS::currJs+=consoleFuncName + "(" + object.getName() + ");\n";
    }

public:
    //Non variable
    template <typename T>
    static void Log(T&& object) { ConsoleBase(std::forward<T>(object), "log"); }
    template <typename T>
    static void Info(T&& object) { ConsoleBase(std::forward<T>(object), "info"); }
    template <typename T>
    static void Warn(T&& object) { ConsoleBase(std::forward<T>(object), "warn"); }
    template <typename T>
    static void Error(T&& object) { ConsoleBase(std::forward<T>(object), "error"); }
    template <typename T>
    static void Debug(T&& object) { ConsoleBase(std::forward<T>(object), "debug"); }
    template <typename T>
    static void Trace(T&& object) { ConsoleBase(std::forward<T>(object), "trace"); }
};

#endif //CONSOLE_H
