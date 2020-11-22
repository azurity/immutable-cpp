#pragma once
#ifndef _IMMUTABLE_TUPLE_HPP_
#define _IMMUTABLE_TUPLE_HPP_

#include <tuple>
#include "utils.hpp"

namespace immutable
{
    struct tuple_t
    {
    };

    template <typename... T>
    struct tuple : public tuple_t
    {
        const std::tuple<T...> content;

        constexpr tuple(T... args) : content{args...} {}
        constexpr tuple(std::tuple<T...> arg) : content(arg) {}

        constexpr std::tuple<T...> to_tuple() const
        {
            return content;
        }

        constexpr auto size() const
        {
            return sizeof...(T);
        }

        template <typename It>
        constexpr auto push_back(const It it) const
        {
            return immutable::tuple(std::tuple_cat(to_tuple(), std::make_tuple(it)));
        }

        template <AnyConst I>
        constexpr auto at(I) const
        {
            return std::get<I::value>(content);
        }

        template <typename F, typename Left>
        constexpr auto reduce(F f, Left left) const
        {
            return reduce_impl(f, left, std::make_index_sequence<sizeof...(T)>());
        }
        template <typename F>
        constexpr auto reduce(F f) const
        {
            return reduce_impl(f, nullptr, std::make_index_sequence<sizeof...(T)>());
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
            auto item = at(make_const(I0));
            auto current = f(left, item);
            return reduce_impl(f, current, std::integer_sequence<S, I...>());
        }

    public:
        template <AnyConst Begin, AnyConst End = std::integral_constant<size_t, sizeof...(T)>>
        requires(Begin::value >= 0 && End::value <= sizeof...(T) && Begin::value <= End::value) constexpr auto slice(Begin, End = End()) const
        {
            return immutable::tuple(slice_impl<Begin, End>(std::make_index_sequence<sizeof...(T)>()));
        }

    private:
        template <AnyConst Begin, AnyConst End, typename S>
        constexpr auto slice_impl(std::integer_sequence<S> &&) const
        {
            return std::make_tuple();
        }
        template <AnyConst Begin, AnyConst End, typename S, S I0, S... I>
        constexpr auto slice_impl(std::integer_sequence<S, I0, I...> &&) const
        {
            if constexpr (I0 < Begin::value)
            {
                return slice_impl<Begin, End>(std::integer_sequence<S, I...>());
            }
            else if constexpr (I0 < End::value)
            {
                return std::tuple_cat(std::make_tuple(at(make_const(I0))), slice_impl<Begin, End>(std::integer_sequence<S, I...>()));
            }
            else
            {
                return std::make_tuple();
            }
        }

    public:
        template <AnyConst Begin, AnyConst Size, BaseOf<tuple_t> Tuple>
        requires(Begin::value >= 0 && Begin::value + Size::value <= sizeof...(T)) constexpr auto splice(Begin, Size, const Tuple t) const
        {
            constexpr auto begin1 = Begin::value + Size::value;
            constexpr auto end1 = sizeof...(T);
            return immutable::tuple(std::tuple_cat(
                slice(make_const(0), Begin()).to_tuple(),
                t.to_tuple(),
                slice(make_const(begin1), make_const(end1)).to_tuple()));
        }

        template <AnyConst I, typename It>
        constexpr auto set(I, const It it) const
        {
            return splice(I(), make_const(1), immutable::tuple<It>(it));
        }
    };

    template <typename... T>
    tuple(T...) -> tuple<T...>;
} // namespace immutable

#endif