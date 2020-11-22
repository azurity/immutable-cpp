#pragma once
#ifndef _IMMUTABLE_UPDATE_HPP_
#define _IMMUTABLE_UPDATE_HPP_

#include "utils.hpp"
#include "tuple.hpp"

namespace immutable::op
{
    struct attr_t
    {
    };

    struct op_t
    {
    };
    constexpr op_t null_op = op_t();

    template <typename T, BaseOf<op_t> Op>
    constexpr auto update(const T t, Op op)
    {
        return op(t, nullptr);
    }

    template <AnyConst T, AnyConst I>
    constexpr auto access(T t, I)
    {
        return (&t)->*I::value;
    }
#if __has_include("array.hpp")
    template <BaseOf<array_t> T, AnyConst I>
    constexpr auto access(T t, I i)
    {
        return t.at(i);
    }
#endif
#if __has_include("tuple.hpp")
    template <BaseOf<tuple_t> T, AnyConst I>
    constexpr auto access(T t, I i)
    {
        return t.at(i);
    }
#endif

    template <typename... T>
    struct push : public op_t
    {
        const tuple<T...> desc;

        constexpr push() : op_t(), desc() {}
        constexpr push(tuple<T...> t) : op_t(), desc(t) {}

        template <typename It>
        constexpr auto operator,(const It it) const
        {
            return op::push<T..., It>{desc.push_back(it)};
        }

        template <typename C>
        constexpr auto operator()(const C c, std::nullptr_t) const
        {
            return desc.reduce(
                [](auto left, auto it) constexpr {
                    return left.push_back(it);
                },
                c);
        }
        template <typename C, AnyConst I>
        constexpr auto operator()(const C c, I i) const
        {
            constexpr auto t = access(c, i);
            return operator()(make_const(t), nullptr);
        }
    };
    constexpr auto Push = push<>();

    template <BaseOf<attr_t>... T>
    struct attr_list;

    template <AnyConst Attr, BaseOf<op_t> Op = op_t>
    struct attr : public op_t, public attr_t
    {
        using op_type = Op;
        const Attr pattr;
        const Op op;
        constexpr attr(Attr pattr) : op_t(), attr_t(), pattr(pattr), op() {}
        constexpr attr(Attr pattr, Op op) : op_t(), attr_t(), pattr(pattr), op(op) {}

        template <typename NewOp>
        requires(std::is_same_v<Op, op_t>) constexpr auto operator,(NewOp newOp) const
        {
            return op::attr<Attr, NewOp>(pattr, newOp);
        }
        template <typename NewOp>
        requires(!std::is_same_v<Op, op_t>) constexpr auto operator,(NewOp newOp) const
        {
            return op::attr_list(*this, newOp);
        }

        template <typename C>
        constexpr auto operator()(const C c, std::nullptr_t) const
        {
            return op.operator()(c, pattr);
        }
        template <typename C, AnyConst I>
        constexpr auto operator()(const C c, I i) const
        {
            auto t = access(c, i);
            return operator()(t, nullptr);
        }
    };

    template <BaseOf<attr_t>... T>
    struct attr_list : public op_t
    {
        const tuple<T...> ops;
        static const size_t Last = sizeof...(T) - 1;
        using LastType = std::tuple_element_t<Last, std::tuple<T...>>;

        constexpr attr_list(T... args) : ops(args...) {}
        constexpr attr_list(tuple<T...> ops) : ops(ops) {}

        template <typename NewOp>
        requires(std::is_same_v<op_t, typename LastType::op_type>) constexpr auto operator,(NewOp op) const
        {
            constexpr auto last = Last;
            return op::attr_list(immutable::tuple(std::tuple_cat(
                ops.slice(make_const(0), make_const(last)).to_tuple(),
                std::make_tuple((ops.at(make_const(last)), op)))));
        }
        template <typename NewAttr>
        requires(!std::is_same_v<op_t, typename LastType::op_type>) constexpr auto operator,(NewAttr newAttr) const
        {
            return op::attr_list(ops.push_back(newAttr));
        }

        template <typename C>
        constexpr auto operator()(const C c, std::nullptr_t) const
        {
            return ops.reduce(
                [](auto left, auto op) constexpr {
                    return op.operator()(left, nullptr);
                },
                c);
        }
        template <typename C, AnyConst I>
        constexpr auto operator()(const C c, I i) const
        {
            constexpr auto t = access(c, i);
            return operator()(make_const(t), nullptr);
        }
    };

    struct op_slot
    {
    };

    template <typename T>
    struct set : public op_t
    {
        const T t;
        constexpr set() : op_t(), t(op_slot()) {}
        constexpr set(T t) : op_t(), t(t) {}

        template <typename It>
        requires(std::is_same_v<T, op_slot>) constexpr auto operator,(It it) const
        {
            return op::set(it);
        }

        template <typename C, AnyConst I>
        requires(!std::is_same_v<T, op_slot>) constexpr auto operator()(C c, I i) const
        {
            auto newC = c;
            (&newC)->*I::value = t;
            return newC;
        }
#if __has_include("array.hpp")
        template <BaseOf<array_t> C, AnyConst I>
        requires(!std::is_same_v<T, op_slot>) constexpr auto operator()(const C c, I i) const
        {
            return c.set(i, t);
        }
#endif
#if __has_include("tuple.hpp")
        template <BaseOf<tuple_t> C, AnyConst I>
        requires(!std::is_same_v<T, op_slot>) constexpr auto operator()(const C c, I i) const
        {
            return c.set(i, t);
        }
#endif
    };
    constexpr auto Set = set<op_slot>();

    template <BaseOf<tuple_t>... T>
    struct splice : public op_t
    {
        const tuple<T...> desc;

        constexpr splice() : op_t(), desc() {}
        constexpr splice(tuple<T...> t) : op_t(), desc(t) {}

        template <BaseOf<tuple_t> It>
        constexpr auto operator,(const It it) const
        {
            return op::splice<T..., It>{desc.push_back(it)};
        }

        template <typename C, AnyConst I>
        constexpr auto operator()(const C c, I i) const
        {
            const auto t = access(c, i);
            return c.set(i, operator()(t, nullptr));
        }
#if __has_include("array.hpp")
        template <BaseOf<array_t> C>
        constexpr auto operator()(const C c, std::nullptr_t) const
        {
            return desc.reduce(
                [](auto left, auto it) constexpr {
                    auto content = it.slice(make_const(2))
                                       .reduce(
                                           [](auto left, auto it) {
                                               return left.push_back(it);
                                           },
                                           array<typename C::Base::value_type, 0>{});
                    return left.splice(it.at(make_const(0)), it.at(make_const(1)), content);
                },
                c);
        }
#endif
#if __has_include("tuple.hpp")
        template <BaseOf<tuple_t> C>
        constexpr auto operator()(const C c, std::nullptr_t) const
        {
            return desc.reduce(
                [](auto left, auto it) constexpr {
                    auto content = it.slice(make_const(2));
                    return left.splice(it.at(make_const(0)), it.at(make_const(1)), content);
                },
                c);
        }
#endif
    };
    constexpr auto Splice = splice<>();

    template <typename T>
    struct apply : public op_t
    {
        const T t;
        constexpr apply() : op_t(), t(op_slot()) {}
        constexpr apply(T t) : op_t(), t(t) {}

        template <typename It>
        requires(std::is_same_v<T, op_slot>) constexpr auto operator,(It it) const
        {
            return op::apply(it);
        }

        template <typename C, AnyConst I>
        requires(!std::is_same_v<T, op_slot>) constexpr auto operator()(C c, I i) const
        {
            auto newC = c;
            (&newC)->*I::value = t.operator()((&newC)->*I::value);
            return newC;
        }
#if __has_include("array.hpp")
        template <BaseOf<array_t> C, AnyConst I>
        requires(!std::is_same_v<T, op_slot>) constexpr auto operator()(const C c, I i) const
        {
            return c.set(i, t.operator()(access(c, i)));
        }
#endif
#if __has_include("tuple.hpp")
        template <BaseOf<tuple_t> C, AnyConst I>
        requires(!std::is_same_v<T, op_slot>) constexpr auto operator()(const C c, I i) const
        {
            return c.set(i, t.operator()(access(c, i)));
        }
#endif
    };
    constexpr auto Apply = apply<op_slot>();
} // namespace immutable::op

#endif
