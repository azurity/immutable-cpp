#pragma once
#ifndef _IMMUTABLE_ARRAY_HPP_
#define _IMMUTABLE_ARRAY_HPP_

#include <array>
#include "utils.hpp"

namespace immutable
{
    struct array_t
    {
    };

    template <typename T, size_t N>
    struct array : public std::array<T, N>, public array_t
    {
        using Base = std::array<T,N>;

        constexpr std::array<T, N> to_array() const
        {
            return *this;
        }

        template <typename It>
        constexpr auto push_back(const It it) const
        {
            array<T, N + 1> result{};
            for (int i = 0; i < N; i++)
            {
                result.std::array<T, N + 1>::at(i) = std::array<T, N>::at(i);
            }
            result.std::array<T, N + 1>::at(N) = T(it);
            return result;
        }

        template <AnyConst I>
        constexpr auto at(I) const
        {
            return std::array<T, N>::at(I::value);
        }

        template <typename F, typename Left>
        constexpr auto reduce(F f, const Left left) const
        {
            return reduce_impl(f, left, std::make_index_sequence<N>());
        }
        template <typename F>
        constexpr auto reduce(F f) const
        {
            return reduce_impl(f, T(), std::make_index_sequence<N>());
        }

    private:
        template <typename F, typename Left, typename S>
        constexpr auto reduce_impl(const F &f, Left &&left, std::integer_sequence<S> &&) const
        {
            return left;
        }
        template <typename F, typename Left, typename S, S I0, S... I>
        constexpr auto reduce_impl(const F &f, Left &&left, std::integer_sequence<S, I0, I...> &&) const
        {
            auto item = std::array<T, N>::at(I0);
            auto current = f(left, item);
            return reduce_impl(f, current, std::integer_sequence<S, I...>());
        }

    public:
        template <AnyConst Begin, AnyConst End = std::integral_constant<size_t, N>>
        requires(Begin::value >= 0 && End::value <= N && Begin::value <= End::value) constexpr auto slice(Begin, End = End()) const
        {
            constexpr auto Len = End::value - Begin::value;
            array<T, Len> result{};
            for (size_t i = Begin::value; i < End::value; i++)
            {
                result.std::array<T, Len>::at(i - Begin::value) = std::array<T, N>::at(i);
            }
            return result;
        }

        template <AnyConst Begin, AnyConst Size, BaseOf<array_t> Array>
        requires(Begin::value >= 0 && Begin::value + Size::value <= N) constexpr auto splice(Begin, Size, const Array a) const
        {
            constexpr auto Len = N - Size::value + a.size();
            array<T, Len> result{};
            for (size_t i = 0; i < Begin::value; i++)
            {
                result.std::array<T, Len>::at(i) = std::array<T, N>::at(i);
            }
            for (size_t i = 0; i < a.size(); i++)
            {
                result.std::array<T, Len>::at(i + Begin::value) = a.Array::Base::at(i);
            }
            for (size_t i = Begin::value + Size::value; i < N; i++)
            {
                result.std::array<T, Len>::at(i + Len - N) = std::array<T, N>::at(i);
            }
            return result;
        }

        template <AnyConst I, typename It>
        constexpr auto set(I, const It it) const
        {
            array<T, N> result{};
            for (int i = 0; i < N; i++)
            {
                result.std::array<T, N>::at(i) = std::array<T, N>::at(i);
            }
            result.std::array<T, N>::at(I::value) = T(it);
            return result;
        }
    };

    template <typename T, typename... U>
    array(T, U...) -> array<T, 1 + sizeof...(U)>;
} // namespace immutable

#endif