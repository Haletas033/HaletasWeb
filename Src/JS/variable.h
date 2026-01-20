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
struct CallResult;
struct JSObject;
template <typename T>
class Variable;
class Function;

inline thread_local int callDepth = 0;

template <typename V>
std::string arithmeticBase(const std::string& lvalue, V&& rvalue, const char op) {
    std::string arithmetic;
    const std::string operand = std::string(" ") + op + " ";

    if constexpr (std::is_same_v<std::decay_t<V>, CallResult>) arithmetic = lvalue + operand + rvalue.use();
    else if constexpr (std::is_same_v<std::decay_t<V>, JSObject>) arithmetic = lvalue + operand + std::string(rvalue);
    else if constexpr (std::is_convertible_v<std::decay_t<V>, std::string>) arithmetic = lvalue + operand + "\"" + std::string(rvalue) + "\"";
    else arithmetic = lvalue + operand + std::to_string(rvalue);
    return arithmetic;
}

//Struct made to decide whether to add something to *JS::currJs or not.
struct CallResult {
    std::string js;
    bool used = false;
    bool isRoot = false;

    explicit CallResult(std::string s) : js(std::move(s)) {
        if (callDepth == 0) isRoot = true;
        ++callDepth;
    }

    CallResult(CallResult&& other) noexcept
        : js(std::move(other.js)), used(other.used), isRoot(other.isRoot)
    {
        other.used = true;
        ++callDepth;
    }

    ~CallResult() {
        --callDepth;
        if (isRoot && !used) *JS::currJs += js + ";\n";
    }

    const std::string& use() {
        used = true;
        return js;
    }

    //Add
    template <typename V>
    CallResult operator+(V&& object) {
        return CallResult(arithmeticBase(use(), object, '+'));
    }
    template <typename V>
    CallResult operator+(Variable<V> &object) {
        const std::string add = use() + " + " + object.getName();
        return CallResult(add);
    }

    //Subtract
    template <typename V>
    CallResult operator-(V&& object) {
        return CallResult(arithmeticBase(use(), object, '-'));
    }
    template <typename V>
    CallResult operator-(Variable<V> &object) {
        const std::string minus = use() + " - " + object.getName();
        return CallResult(minus);
    }

    //Multiply
    template <typename V>
    CallResult operator*(V&& object) {
        return CallResult(arithmeticBase(use(), object, '*'));
    }
    template <typename V>
    CallResult operator*(Variable<V> &object) {
        const std::string multiply = use() + " * " + object.getName();
        return CallResult(multiply);
    }

    //Divide
    template <typename V>
    CallResult operator/(V&& object) {
        return CallResult(arithmeticBase(use(), object, '/'));
    }
    template <typename V>
    CallResult operator/(Variable<V> &object) {
        const std::string divide = use() + " / " + object.getName();
        return CallResult(divide);
    }
};

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

template <typename T>
struct is_variable : std::false_type {};

template <typename U>
struct is_variable<Variable<U>> : std::true_type {};


template <typename T = void>
class Variable {
    const VarType type;
    std::type_index staticType = typeid(void);
    const bool staticallyTyped = false;
    const std::string name;
    bool initialized = false;
    bool isArg = false;
public:
    //For dynamic types
    template <typename U = T>
    Variable(const VarType t, std::string n,
        std::enable_if_t<std::is_same_v<U, void>, int> = 0)
            : type(t), name(std::move(n))
    {
        if (this == expectedNextInitialized) *JS::currJs+=";\n";
        const std::string variable = VarTypeToString[t] + " " + name;
        *JS::currJs += variable;
        expectedNextInitialized = static_cast<void*>(this);
        nextInitializedIsRequired = type == CONSTANT;
    }

    //For safer static types
    template <typename U = T>
    Variable(const VarType t, std::string n,
        std::enable_if_t<!std::is_same_v<U, void>, int> = 0)
            : type(t), staticType(typeid(T)), staticallyTyped(true), name(std::move(n))
    {
        if (!nextInitializedIsRequired) *JS::currJs+=";\n";
        else throw std::logic_error("Const variable wasn't initialized");

        const std::string variable = VarTypeToString[t] + " " + name;
        *JS::currJs += variable;
        expectedNextInitialized = static_cast<void*>(this);
        nextInitializedIsRequired = type == CONSTANT;
    }

    [[nodiscard]] bool isInitialized() const {
        return initialized;
    }

    [[nodiscard]] std::string getName() const {
        return name;
    }

    Variable asArg(Function& func) {
        func.argCount++;
        this->isArg = true;
        return *this;
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
    Variable& operator=(V&& object) {
        if (type != CONSTANT || !isInitialized()) {
            if (staticallyTyped) {
                if constexpr (!std::is_convertible_v<V, T>)
                    throw std::logic_error(std::string("Expected type ") + staticType.name() + " got " + typeid(V).name() + " instead.");
            }
            std::string assign;
            if constexpr (std::is_same_v<std::decay_t<V>, CallResult>) assign = " = " + object.use();
            else if constexpr (!std::is_convertible_v<V, std::string> && !is_vector<V>::value) assign = " = " + std::to_string(object);
            else if constexpr (is_vector<V>::value) assign = " = " + ArrayToString(object);
            else if constexpr (std::is_same_v<V, JSObject>) assign = " = " + std::string(object);
            else assign = " = \"" + std::string(object) + "\"";

            if (!expectedNextInitialized) *JS::currJs+=this->name;

            *JS::currJs+=assign;
            if (this == expectedNextInitialized || !nextInitializedIsRequired) *JS::currJs+=";\n";
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

            if (!expectedNextInitialized) *JS::currJs+=this->name;

            *JS::currJs+=assign;
            if (this == expectedNextInitialized || !nextInitializedIsRequired) *JS::currJs+=";\n";
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
        if (!this->isArg) {
            if (!this->initialized)
                throw std::logic_error(std::string("Can't do arithmetic on uninitialized variable"));
            if (this != expectedNextInitialized && nextInitializedIsRequired)
                throw std::logic_error(std::string("Tried to do arithmetic before initialization of a const variable"));
        }

    }

    void IsLegalVariable(Variable& other) const {
        if (!other.isArg || !this->isArg) {
            if (!this->initialized)
                throw std::logic_error(std::string("Can't do arithmetic on uninitialized variable"));
            if (this != expectedNextInitialized && nextInitializedIsRequired)
                throw std::logic_error(std::string("Tried to do arithmetic before initialization of a const variable"));
        }
        if (this->staticType != other.staticType && this->staticType != typeid(CallResult)) {
            throw std::logic_error(std::string("Tried to do arithmetic on ") + staticType.name() + " and " + other.staticType.name());
        }
    }

public:

    //Add
    template <typename V>
    CallResult operator+(V&& object) {
        IsLegalLiteral<V>();
        return CallResult(arithmeticBase(this->name, object, '+'));
    }
    template <typename V>
    CallResult operator+(Variable<V> &object) {
        IsLegalVariable(object);
        const std::string add = this->name + " + " + object.getName();
        return CallResult(add);
    }

    //Subtract
    template <typename V>
    CallResult operator-(V&& object) {
        IsLegalLiteral<V>();
        return CallResult(arithmeticBase(this->name, object, '-'));
    }
    template <typename V>
    CallResult operator-(Variable<V> &object) {
        IsLegalVariable(object);
        const std::string minus = this->name + " - " + object.getName();
        return CallResult(minus);
    }

    //Multiply
    template <typename V>
    CallResult operator*(V&& object) {
        IsLegalLiteral<V>();
        return CallResult(arithmeticBase(this->name, object, '*'));
    }
    template <typename V>
    CallResult operator*(Variable<V> &object) {
        IsLegalVariable(object);
        const std::string multiply = this->name + " * " + object.getName();
        return CallResult(multiply);
    }

    //Divide
    template <typename V>
    CallResult operator/(V&& object) {
        IsLegalLiteral<V>();
        return CallResult(arithmeticBase(this->name, object, '/'));
    }
    template <typename V>
    CallResult operator/(Variable<V> &object) {
        IsLegalVariable(object);
        const std::string divide = this->name + " / " + object.getName();
        return CallResult(divide);
    }

    template <typename V>
    static std::string AddArg(V&& arg) {
        if constexpr (is_vector<std::decay_t<V>>::value) return ArrayToString(arg) + ",";
        else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, JSObject>) return std::string(arg) + ",";
        else if constexpr (std::is_convertible_v<decltype(arg), std::string>) return std::string("\"") + arg + "\"" + ",";
        else return std::to_string(arg) + ",";
    }

    template <typename V>
    static std::string AddArg(Variable<V>& arg) {
        return arg.getName() + std::string(",");
    }

    //Use () to call functions of variables because . is not overloadable
    template <typename V, typename... Args>
    CallResult operator()(V object, Args&&... args) {
        std::string result;

        if (expectedNextInitialized != nullptr && nextInitializedIsRequired) {
            if constexpr (std::is_convertible_v<V, std::string>)
                throw std::logic_error(std::string("Tried to call function \"") + object + "\" on \"" + this->name + "\" before initialization of a const variable");
            throw std::logic_error(std::string("Expected function name/string type got ") + typeid(V).name() + " instead.");
        }

        if constexpr (std::is_convertible_v<V, std::string>) result+=this->name + "." + object + "(";
        else throw std::logic_error(std::string("Expected function name/string type got ") + typeid(V).name() + " instead.");

        //Add arguments
        auto functionArgs = std::forward_as_tuple(std::forward<Args>(args)...);
        std::apply([&](auto&&... arg){
            (([&]{
                result+=AddArg(arg);
            }()), ...);
        }, std::forward_as_tuple(std::forward<Args>(args)...));

        //Close function
        if (sizeof...(Args)) result.pop_back(); //Only pop_back if there are arguments
        return CallResult(result + ')');
    }};

#endif //VARIABLE_H
