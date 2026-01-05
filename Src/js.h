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

//Used when you need to reference a variable or something from a js file
struct JSObject {
    std::string object;
    explicit JSObject(const std::string &obj) : object(obj) {}
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

inline std::string js = "";

template <typename T = void>
class Variable {
    const VarType type;
    const std::type_index staticType = typeid(void);
    const bool staticallyTyped = false;
    const std::string name;
    bool initialized = false;
    std::any storedValue;
public:
    //For dynamic types
    template <typename U = T>
    Variable(const VarType t, std::string n,
        std::enable_if_t<std::is_same_v<U, void>, int> = 0)
            : type(t), name(std::move(n))
    {
        const std::string variable = VarTypeToString[t] + " " + name + " ";
        js += variable;
    }

    //For safer static types
    template <typename U = T>
    Variable(const VarType t, std::string n,
        std::enable_if_t<!std::is_same_v<U, void>, int> = 0)
            : type(t), staticType(typeid(T)), staticallyTyped(true), name(std::move(n))
    {
        const std::string variable = VarTypeToString[t] + " " + name + " ";
        js += variable;
    }

    [[nodiscard]] const std::any& value() const {
        return storedValue;
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

            storedValue = std::move(object);
            initialized = true;
            return *this;
        }
        throw std::logic_error("Can't modify const variable \"" + this->name + "\" Consider using LET or VAR instead.");
    }

    //Arithmetic
    template <typename V, typename Operand>
    T Arithmetic(V object, Operand op) {
        if constexpr (std::is_convertible_v<V, T>) {
            return op(std::any_cast<T>(storedValue), object);
        }
        throw std::logic_error(std::string("Can't add ") + typeid(T).name() + " to " + typeid(V).name());
    }

    template <typename V, typename Operand>
    T Arithmetic(Variable<V> &object, Operand op) {
        if constexpr (std::is_convertible_v<V, T>) {
            return op(std::any_cast<T>(storedValue), std::any_cast<T>(object.storedValue));
        }
        throw std::logic_error(std::string("Can't add ") + typeid(T).name() + " to " + typeid(V).name());
    }

    //Add
    template <typename V>
    T operator+(V object) {
        const std::string add = this->name + " + " + std::to_string(object) + ";\n";
        js+=add;
        return Arithmetic(object, std::plus<>());
    }
    template <typename V>
    T operator+(Variable<V> &object) {
        const std::string add = this->name + " + " + object.name + ";\n";
        js+=add;
        return Arithmetic(object, std::plus<>());
    }

    //Subtract
    template <typename V>
    T operator-(V object) {
        const std::string minus = this->name + " - " + std::to_string(object) + ";\n";
        js+=minus;
        return Arithmetic(object, std::minus<>());
    }
    template <typename V>
    T operator-(Variable<V> &object) {
        const std::string minus = this->name + " - " + object.name + ";\n";
        js+=minus;
        return Arithmetic(object, std::minus<>());
    }

    //Multiply
    template <typename V>
    T operator*(V object) {
        const std::string multiply = this->name + " * " + std::to_string(object) + ";\n";
        js+=multiply;
        return Arithmetic(object, std::multiplies<>());
    }
    template <typename V>
    T operator*(Variable<V> &object) {
        const std::string multiply = this->name + " * " + object.name + ";\n";
        js+=multiply;
        return Arithmetic(object, std::multiplies<>());
    }

    //Divide
    template <typename V>
    T operator/(V object) {
        const std::string divide = this->name + " / " + std::to_string(object) + ";\n";
        js+=divide;
        return Arithmetic(object, std::divides<>());
    }
    template <typename V>
    T operator/(Variable<V> &object) {
        const std::string divide = this->name + " / " + object.name + ";\n";
        js+=divide;
        return Arithmetic(object, std::divides<>());
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
