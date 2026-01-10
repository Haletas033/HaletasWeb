//
// Created by halet on 1/7/2026.
//

#ifndef VARIABLE_H
#define VARIABLE_H

#include <any>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <ostream>
#include <vector>
#include "jsCore.h"
struct JSObject;
template <typename T>
class Variable;

template <typename V>
std::string arithmeticBase(const std::string& lvalue, V rvalue, const char op) {
    std::string arithmetic;
    const std::string operand = std::string(" ") + op + " ";

    if constexpr (std::is_same_v<std::decay_t<V>, JSObject>) arithmetic = lvalue + operand + std::string(rvalue);
    else if constexpr (std::is_convertible_v<std::decay_t<V>, std::string>) arithmetic = lvalue + operand + "\"" + std::string(rvalue) + "\"";
    else arithmetic = lvalue + operand + std::to_string(rvalue);
    return arithmetic;
}

//Used when you need to reference a variable or something from a js file
struct JSObject {
    std::string object;
    explicit JSObject(std::string obj) : object(std::move(obj)) {}
    operator std::string() const { return object;}

    //Add
    template <typename V>
    JSObject operator+(V object) {
        return JSObject(arithmeticBase(this->object, object, '+'));
    }
    template <typename V>
    JSObject operator+(Variable<V> &object) {
        const std::string add = this->object + " + " + object.getName();
        return JSObject(add);
    }

    //Subtract
    template <typename V>
    JSObject operator-(V object) {
        return JSObject(arithmeticBase(this->object, object, '-'));
    }
    template <typename V>
    JSObject operator-(Variable<V> &object) {
        const std::string minus = this->object + " - " + object.getName();
        return JSObject(minus);
    }

    //Multiply
    template <typename V>
    JSObject operator*(V object) {
        return JSObject(arithmeticBase(this->object, object, '*'));
    }
    template <typename V>
    JSObject operator*(Variable<V> &object) {
        const std::string multiply = this->object + " * " + object.getName();
        return JSObject(multiply);
    }

    //Divide
    template <typename V>
    JSObject operator/(V object) {
        return JSObject(arithmeticBase(this->object, object, '/'));
    }
    template <typename V>
    JSObject operator/(Variable<V> &object) {
        const std::string divide = this->object + " / " + object.getName();
        return JSObject(divide);
    }
};

enum VarType {
    CONSTANT,
    LET,
    VAR
};

inline std::unordered_map<VarType, std::string> VarTypeToString = {
    {CONSTANT, "const"},
    {LET, "let"},
    {VAR, "var"}
};

inline void *expectedNextInitialized = nullptr;

inline bool nextInitializedIsRequired = false;

template<typename T>
struct is_vector : std::false_type {};

template<typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>> : std::true_type {};

template <typename T = void>
class Variable {
    const VarType type;
    std::type_index staticType = typeid(void);
    const bool staticallyTyped = false;
    const std::string name;
    bool initialized = false;
public:
    //For dynamic types
    template <typename U = T>
    Variable(const VarType t, std::string n,
        std::enable_if_t<std::is_same_v<U, void>, int> = 0)
            : type(t), name(std::move(n))
    {
        if (this == expectedNextInitialized) JS::js+=";\n";
        const std::string variable = VarTypeToString[t] + " " + name;
        JS::js += variable;
        expectedNextInitialized = static_cast<void*>(this);
        nextInitializedIsRequired = type == CONSTANT;
    }

    //For safer static types
    template <typename U = T>
    Variable(const VarType t, std::string n,
        std::enable_if_t<!std::is_same_v<U, void>, int> = 0)
            : type(t), staticType(typeid(T)), staticallyTyped(true), name(std::move(n))
    {
        if (!nextInitializedIsRequired) JS::js+=";\n";
        else throw std::logic_error("Const variable wasn't initialized");

        const std::string variable = VarTypeToString[t] + " " + name;
        JS::js += variable;
        expectedNextInitialized = static_cast<void*>(this);
        nextInitializedIsRequired = type == CONSTANT;
    }

    [[nodiscard]] bool isInitialized() const {
        return initialized;
    }

    [[nodiscard]] std::string getName() const {
        return name;
    }

    template <typename V>
    static std::string ArrayToString(const std::vector<V> &array) {
        std::string output = "[";
        for (auto i : array) {
            if constexpr(std::is_same_v<V, std::string>) output += + "\"" + i + "\",";
            else output += std::to_string(i) + ",";
        }
        //Remove extra comma
        output.pop_back();

        return  output + "]";
    }

    template <typename V>
    Variable& operator=(V object) {
        if (type != CONSTANT || !isInitialized()) {
            if (staticallyTyped) {
                if constexpr (!std::is_convertible_v<V, T>)
                    throw std::logic_error(std::string("Expected type ") + staticType.name() + " got " + typeid(V).name() + " instead.");
            }
            std::string assign;
            if constexpr (!std::is_convertible_v<V, std::string> && !is_vector<V>::value) assign = " = " + std::to_string(object);
            else if constexpr (is_vector<V>::value) assign = " = " + ArrayToString(object);
            else if constexpr (std::is_same_v<V, JSObject>) assign = " = " + std::string(object);
            else assign = " = \"" + std::string(object) + "\"";

            if (!expectedNextInitialized) JS::js+=this->name;

            JS::js+=assign;
            if (this == expectedNextInitialized || !nextInitializedIsRequired) JS::js+=";\n";
            else throw std::logic_error("Variable initialized before uninitialized const variable");

            initialized = true;

            expectedNextInitialized = nullptr;
            nextInitializedIsRequired = false;

            staticType = typeid(V);

            return *this;
        }
        throw std::logic_error("Can't modify const variable \"" + this->name + "\" Consider using LET or VAR instead.");
    }

    template <typename V>
    Variable& operator=(Variable<V> &object) {
        if (type != CONSTANT || !isInitialized()) {
            if (staticallyTyped) {
                if constexpr (!std::is_convertible_v<V, T>)
                    throw std::logic_error(std::string("Expected type ") + staticType.name() + " got " + object.staticType + " instead.");
            }
            const std::string assign = " = " + object.getName();

            if (!expectedNextInitialized) JS::js+=this->name;

            JS::js+=assign;
            if (this == expectedNextInitialized || !nextInitializedIsRequired) JS::js+=";\n";
            else throw std::logic_error("Variable initialized before uninitialized const variable");

            initialized = true;

            expectedNextInitialized = nullptr;
            nextInitializedIsRequired = false;

            staticType = typeid(V);

            return *this;
        }
        throw std::logic_error("Can't modify const variable \"" + this->name + "\" Consider using LET or VAR instead.");
    }

private:

    //Arithmetic
    template <typename V>
    void IsLegalLiteral() const {
        if constexpr (!std::is_convertible_v<V, T> && !std::is_same_v<T, void> && !std::is_same_v<V, void>)
            throw std::logic_error(std::string("Can't do arithmetic on ") + typeid(T).name() + " and " + typeid(V).name());
        else if (!this->initialized)
            throw std::logic_error(std::string("Can't do arithmetic on uninitialized variable"));
        else if (this != expectedNextInitialized && nextInitializedIsRequired)
            throw std::logic_error(std::string("Tried to do arithmetic before initialization of a const variable"));

    }

    void IsLegalVariable(Variable& other) const {
        if (!this->initialized)
            throw std::logic_error(std::string("Can't do arithmetic on uninitialized variable"));
        if (this != expectedNextInitialized && nextInitializedIsRequired)
            throw std::logic_error(std::string("Tried to do arithmetic before initialization of a const variable"));
        if (this->staticType != other.staticType) {
            throw std::logic_error(std::string("Tried to do arithmetic on ") + staticType.name() + " and " + other.staticType.name());
        }
    }

public:

    //Add
    template <typename V>
    JSObject operator+(V object) {
        IsLegalLiteral<V>();
        return JSObject(arithmeticBase(this->name, object, '+'));
    }
    template <typename V>
    JSObject operator+(Variable<V> &object) {
        IsLegalVariable(object);
        const std::string add = this->name + " + " + object.getName();
        return JSObject(add);
    }

    //Subtract
    template <typename V>
    JSObject operator-(V object) {
        IsLegalLiteral<V>();
        return JSObject(arithmeticBase(this->name, object, '-'));
    }
    template <typename V>
    JSObject operator-(Variable<V> &object) {
        IsLegalVariable(object);
        const std::string minus = this->name + " - " + object.getName();
        return JSObject(minus);
    }

    //Multiply
    template <typename V>
    JSObject operator*(V object) {
        IsLegalLiteral<V>();
        return JSObject(arithmeticBase(this->name, object, '*'));
    }
    template <typename V>
    JSObject operator*(Variable<V> &object) {
        IsLegalVariable(object);
        const std::string multiply = this->name + " * " + object.getName();
        return JSObject(multiply);
    }

    //Divide
    template <typename V>
    JSObject operator/(V object) {
        IsLegalLiteral<V>();
        return JSObject(arithmeticBase(this->name, object, '/'));
    }
    template <typename V>
    JSObject operator/(Variable<V> &object) {
        IsLegalVariable(object);
        const std::string divide = this->name + " / " + object.getName();
        return JSObject(divide);
    }

    template <typename V>
    void AddArg(V&& arg) {
        if constexpr (is_vector<std::decay_t<V>>::value) JS::js+=ArrayToString(arg) + ",";
        else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, JSObject>) JS::js+=std::string(arg) + ",";
        else if constexpr (std::is_convertible_v<decltype(arg), std::string>) JS::js+=std::string("\"") + arg + "\"" + ",";
        else JS::js+=std::to_string(arg) + ",";
    }

    template <typename V>
    static void AddArg(Variable<V>& arg) {
        JS::js+=arg.getName() + std::string(",");
    }

    //Use () to call functions of variables because . is not overloadable
    template <typename V, typename... Args>
    void operator()(V object, Args&&... args) {
        if (expectedNextInitialized != nullptr && nextInitializedIsRequired) {
            if constexpr (std::is_convertible_v<V, std::string>)
                throw std::logic_error(std::string("Tried to call function \"") + object + "\" on \"" + this->name + "\" before initialization of a const variable");
            throw std::logic_error(std::string("Expected function name/string type got ") + typeid(V).name() + " instead.");
        }

        if constexpr (std::is_convertible_v<V, std::string>) JS::js+=this->name + "." + object + "(";
        else throw std::logic_error(std::string("Expected function name/string type got ") + typeid(V).name() + " instead.");

        //Add arguments
        auto functionArgs = std::forward_as_tuple(std::forward<Args>(args)...);
        std::apply([&](auto&&... arg){
            (([&]{
                AddArg(arg);
            }()), ...);
        }, std::forward_as_tuple(std::forward<Args>(args)...));

        //Close function
        if (sizeof...(Args)) JS::js.pop_back(); //Only pop_back if there are arguments
        JS::js+=");\n";
    }};

#endif //VARIABLE_H
