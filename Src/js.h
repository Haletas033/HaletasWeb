//
// Created by halet on 1/5/2026.
//

//A JavaScript DSL

#ifndef JS_H
#define JS_H
#include <any>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <ostream>

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

extern std::string js;
extern std::unordered_map<VarType, std::string> VarTypeToString;

template<typename T>
class Variable;
extern void *expectedNextInitialized;
extern bool nextInitializedIsRequired;

template <typename T = void>
class Variable {
    const VarType type;
    const std::type_index staticType = typeid(void);
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
        if (this == expectedNextInitialized) js+=";\n";
        const std::string variable = VarTypeToString[t] + " " + name;
        js += variable;
        expectedNextInitialized = static_cast<void*>(this);
        nextInitializedIsRequired = type == CONSTANT;
    }

    //For safer static types
    template <typename U = T>
    Variable(const VarType t, std::string n,
        std::enable_if_t<!std::is_same_v<U, void>, int> = 0)
            : type(t), staticType(typeid(T)), staticallyTyped(true), name(std::move(n))
    {
        if (!nextInitializedIsRequired) js+=";\n";
        else throw std::logic_error("Const variable wasn't initialized");

        const std::string variable = VarTypeToString[t] + " " + name;
        js += variable;
        expectedNextInitialized = static_cast<void*>(this);
        nextInitializedIsRequired = type == CONSTANT;
    }

    [[nodiscard]] bool isInitialized() const {
        return initialized;
    }

    template <typename V>
    Variable& operator=(V object) {
        if (type != CONSTANT || !isInitialized()) {
            if (staticallyTyped) {
                if constexpr (!std::is_convertible_v<V, T>)
                    throw std::logic_error(std::string("Expected type ") + staticType.name() + " got " + typeid(V).name() + " instead.");
            }
            std::string assign;
            if constexpr(!std::is_same_v<V, std::string> && !std::is_same_v<V, JSObject>) assign = " = " + std::to_string(object);
            else if (std::is_same_v<V, JSObject>) assign = " = " + std::string(object);
            else assign = " = \"" + std::string(object) + "\"";

            if (!expectedNextInitialized) js+=this->name;

            js+=assign;
            if (this == expectedNextInitialized || !nextInitializedIsRequired) js+=";\n";
            else throw std::logic_error("Variable initialized before uninitialized const variable");

            initialized = true;

            expectedNextInitialized = nullptr;
            nextInitializedIsRequired = false;

            return *this;
        }
        throw std::logic_error("Can't modify const variable \"" + this->name + "\" Consider using LET or VAR instead.");
    }

    //Arithmetic
    template <typename V>
    void IsLegal() const {
        if constexpr (!std::is_convertible_v<V, T>)
            throw std::logic_error(std::string("Can't do arithmetic on ") + typeid(T).name() + " and " + typeid(V).name());
        else if (!this->initialized)
            throw std::logic_error(std::string("Can't do arithmetic on uninitialized variable"));
        else if (this != expectedNextInitialized && nextInitializedIsRequired)
            throw std::logic_error(std::string("Tried to do arithmetic before initialization of a const variable"));
    }

    //Add
    template <typename V>
    void operator+(V object) {
        IsLegal<V>();
        const std::string add = this->name + " + " + std::to_string(object) + ";\n";
        js+=add;
    }
    template <typename V>
    void operator+(Variable<V> &object) {
        IsLegal<V>();
        const std::string add = this->name + " + " + object.name + ";\n";
        js+=add;
    }

    //Subtract
    template <typename V>
    void operator-(V object) {
        IsLegal<V>();
        const std::string minus = this->name + " - " + std::to_string(object) + ";\n";
        js+=minus;
    }
    template <typename V>
    void operator-(Variable<V> &object) {
        IsLegal<V>();
        const std::string minus = this->name + " - " + object.name + ";\n";
        js+=minus;
    }

    //Multiply
    template <typename V>
    void operator*(V object) {
        IsLegal<V>();
        const std::string multiply = this->name + " * " + std::to_string(object) + ";\n";
        js+=multiply;
    }
    template <typename V>
    void operator*(Variable<V> &object) {
        IsLegal<V>();
        const std::string multiply = this->name + " * " + object.name + ";\n";
        js+=multiply;
    }

    //Divide
    template <typename V>
    void operator/(V object) {
        IsLegal<V>();
        const std::string divide = this->name + " / " + std::to_string(object) + ";\n";
        js+=divide;
    }
    template <typename V>
    void operator/(Variable<V> &object) {
        IsLegal<V>();
        const std::string divide = this->name + " / " + object.name + ";\n";
        js+=divide;
    }
};

template <typename T>
 std::ostream& operator<<(std::ostream& os, const Variable<T>& var) {
    if (!var.isInitialized()) return os << "Tried to print uninitialized variable";

    const auto value = var.value();
    if (value.type() == typeid(int)) return os << std::any_cast<int>(value);
    if (value.type() == typeid(double)) return os << std::any_cast<double>(value);
    if (value.type() == typeid(float)) return os << std::any_cast<float>(value);
    if (value.type() == typeid(char)) return os << std::any_cast<char>(value);
    if (value.type() == typeid(const char*)) return os << std::any_cast<const char*>(value);
    if (value.type() == typeid(std::string)) return os << std::any_cast<std::string>(value);
    if (value.type() == typeid(JSObject)) {
        const JSObject &obj = std::any_cast<JSObject>(value);
        return os << obj.object << " (as raw text)";
    }
    return os << "Unprintable Variable Type: " << value.type().name();
}

#endif //JS_H
