//
// Created by halet on 2/20/2026.
//

#ifndef CONDITIONALS_H
#define CONDITIONALS_H

#include "typeTraits.h"

struct JSObject;
struct CallResult;
template <typename T>
class Variable;

struct Conditional;

template<typename V>
static Conditional LiteralConditionalBase(std::string operand, std::string lvalue, V rvalue);

//Handles syntax such as foo == bar || foo != baz
struct Conditional {
    std::string js;

    explicit Conditional(std::string s) : js(std::move(s)){}

    Conditional(Conditional&& other) noexcept
        : js(std::move(other.js)){}

    //Conditional operators

    //Equivalent
    template <typename V>
    Conditional operator==(V object) const {
        return LiteralConditionalBase(" === ", this->js, object);
    }
    template <typename V>
    Conditional operator==(Variable<V> object) const {
        return Conditional(this->js + " === " + object.getName());
    }

    //Not Equivalent
    template <typename V>
    Conditional operator!=(V object) const {
        return LiteralConditionalBase(" !== ", this->js, object);
    }
    template <typename V>
    Conditional operator!=(Variable<V> object) const {
        return Conditional(this->js + " !== " + object.getName());
    }

    //And
    template <typename V>
    Conditional operator&&(V object) const {
        return LiteralConditionalBase(" && ", this->js, object);
    }
    template <typename V>
    Conditional operator&&(Variable<V> object) const {
        return Conditional(this->js + " && " + object.getName());
    }

    //Or
    template <typename V>
    Conditional operator||(V object) const {
        return LiteralConditionalBase(" || ", this->js, object);
    }
    template <typename V>
    Conditional operator||(Variable<V> object) const {
        return Conditional(this->js + " || " + object.getName());
    }

    //Less than
    template <typename V>
    Conditional operator>(V object) const {
        return LiteralConditionalBase(" > ", this->js, object);
    }
    template <typename V>
    Conditional operator>(Variable<V> object) const {
        return Conditional(this->js + " > " + object.getName());
    }

    //Greater than
    template <typename V>
    Conditional operator<(V object) const {
        return LiteralConditionalBase(" < ", this->js, object);
    }
    template <typename V>
    Conditional operator<(Variable<V> object) const {
        return Conditional(this->js + " < " + object.getName());
    }

    template <typename V>
    Conditional operator>=(V object) const {
        return LiteralConditionalBase(" >= ", this->js, object);
    }
    template <typename V>
    Conditional operator>=(Variable<V> object) const {
        return Conditional(this->js + " >= " + object.getName());
    }

    template <typename V>
    Conditional operator<=(V object) const {
        return LiteralConditionalBase(" <= ", this->js, object);
    }
    template <typename V>
    Conditional operator<=(Variable<V> object) const {
        return Conditional(this->js + " <= " + object.getName());
    }
};

template<typename V>
    static Conditional LiteralConditionalBase(std::string operand, const std::string lvalue, V rvalue) {
    std::string result = lvalue;


    if constexpr (std::is_same_v<std::decay_t<V>, CallResult>) result += operand + rvalue.use();
    else if constexpr (std::is_same_v<std::decay_t<V>, Conditional>) result += operand + rvalue.js;
    else if constexpr (!std::is_convertible_v<V, std::string> && !is_vector<V>::value) result += operand + std::to_string(rvalue);
    else if constexpr (is_vector<V>::value) result += operand + ArrayToString(rvalue);
    else if constexpr (std::is_same_v<V, JSObject>) result += operand + std::string(rvalue);
    else result += operand + "\"" + std::string(rvalue) + "\"";
    return Conditional(result);
}

class Cond {
public:
    static void If(const Conditional &cond) {
        *JS::currJs += "if (" + cond.js + ") {";
    }

    static void ElseIf(const Conditional &cond) {
        *JS::currJs += "}\n else if (" + cond.js + ") {";
    }

    static void Else() {
        *JS::currJs += "}\n else {";
    }

    static void EndIf() {
        *JS::currJs += "}\n";
    }
};

#endif //CONDITIONALS_H
