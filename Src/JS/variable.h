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

//Used when you need to reference a variable or something from a js file
struct JSObject {
    std::string object;
    explicit JSObject(std::string obj) : object(std::move(obj)) {}
    operator std::string() const { return object;}

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

    //Add
    template <typename V>
    void operator+(V object) {
        IsLegalLiteral<V>();
        const std::string add = this->name + " + " + std::to_string(object) + ";\n";
        JS::js+=add;
    }
    template <typename V>
    void operator+(Variable<V> &object) {
        IsLegalVariable(object);
        const std::string add = this->name + " + " + object.getName() + ";\n";
        JS::js+=add;
    }

    //Subtract
    template <typename V>
    void operator-(V object) {
        IsLegalLiteral<V>();
        const std::string minus = this->name + " - " + std::to_string(object) + ";\n";
        JS::js+=minus;
    }
    template <typename V>
    void operator-(Variable<V> &object) {
        IsLegalVariable(object);
        const std::string minus = this->name + " - " + object.getName() + ";\n";
        JS::js+=minus;
    }

    //Multiply
    template <typename V>
    void operator*(V object) {
        IsLegalLiteral<V>();
        const std::string multiply = this->name + " * " + std::to_string(object) + ";\n";
        JS::js+=multiply;
    }
    template <typename V>
    void operator*(Variable<V> &object) {
        IsLegalVariable(object);
        const std::string multiply = this->name + " * " + object.getName() + ";\n";
        JS::js+=multiply;
    }

    //Divide
    template <typename V>
    void operator/(V object) {
        IsLegalLiteral<V>();
        const std::string divide = this->name + " / " + std::to_string(object) + ";\n";
        JS::js+=divide;
    }
    template <typename V>
    void operator/(Variable<V> &object) {
        IsLegalVariable(object);
        const std::string divide = this->name + " / " + object.getName() + ";\n";
        JS::js+=divide;
    }
};

#endif //VARIABLE_H
