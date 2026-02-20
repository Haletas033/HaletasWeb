//
// Created by halet on 2/20/2026.
//

#ifndef TYPETRAITS_H
#define TYPETRAITS_H
#include <type_traits>
#include "variable.h"

template <typename T>
class Variable;

template<typename T>
struct is_vector : std::false_type {};

template<typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>> : std::true_type {};

template <typename T>
struct is_variable : std::false_type {};

template <typename T>
struct is_variable<Variable<T>> : std::true_type {};

#endif //TYPETRAITS_H
