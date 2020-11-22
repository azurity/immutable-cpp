#pragma once
#ifndef _IMMUTABLE_UTILS_HPP_
#define _IMMUTABLE_UTILS_HPP_

#include <type_traits>
#include <utility>

#define make_const(x) std::integral_constant<decltype(x), x>()

namespace immutable
{
    template <typename T>
    concept AnyConst = requires
    {
        typename T::value_type;
        T::value;
    };

    template <typename T, typename X>
    concept Const = AnyConst<T> &&std::is_same_v<X, typename T::value_type>;

    template <typename T, typename X>
    concept BaseOf = std::is_base_of_v<X, T>;

    template <typename T, typename X>
    concept BaseConst = AnyConst<T> &&BaseOf<typename T::value_type, X>;
} // namespace immutable

#endif
