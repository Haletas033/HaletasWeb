//
// Created by halet on 1/7/2026.
//

#ifndef CONSOLE_H
#define CONSOLE_H

#include "jsCore.h"
#include "variable.h"

class Console {
private:

    //For literals
    template <typename  T>
    static void ConsoleBase(const T &object, const std::string& funcName) {
        const std::string consoleFuncName = "console." + funcName;

        if (expectedNextInitialized != nullptr && nextInitializedIsRequired)
            throw std::logic_error("Tried to call " + consoleFuncName + " before initialization of a const variable.");

        if constexpr (is_vector<T>::value) *JS::currJs+=consoleFuncName + "(" + Variable<>::ArrayToString(object) + ");\n";
        else if constexpr (!std::is_convertible_v<T, std::string>) *JS::currJs+=std::string(consoleFuncName + "(") + std::to_string(object) + ");\n";
        else if constexpr (std::is_same_v<T, JSObject>) *JS::currJs+=std::string(consoleFuncName + "(") + std::string(object) + ");\n";
        else *JS::currJs+=std::string(consoleFuncName + "(\"") + object + "\");\n";
    }

    //For variables
    template <typename T>
    static void ConsoleBase(const Variable<T> &object, const std::string& funcName) {
        const std::string consoleFuncName = "console." + funcName;

        if (expectedNextInitialized != nullptr && nextInitializedIsRequired)
            throw std::logic_error("Tried to call " + consoleFuncName + " before initialization of a const variable.");

        *JS::currJs+=consoleFuncName + "(" + object.getName() + ");\n";
    }

public:
    //Non variable
    template <typename T>
    static void Log(const T &object) { ConsoleBase(object, "log"); }
    template <typename T>
    static void Info(const T &object) { ConsoleBase(object, "info"); }
    template <typename T>
    static void Warn(const T &object) { ConsoleBase(object, "warn"); }
    template <typename T>
    static void Error(const T &object) { ConsoleBase(object, "error"); }
    template <typename T>
    static void Debug(const T &object) { ConsoleBase(object, "debug"); }
    template <typename T>
    static void Trace(const T &object) { ConsoleBase(object, "trace"); }
};

#endif //CONSOLE_H
