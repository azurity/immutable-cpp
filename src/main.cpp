#include <iostream>
#include <stdio.h>
#include <iomanip>

// if you want to analysis the binary generated from this code, this macro will help you to do this in an easy way.
// #define FOR_REVERSE_ANALYSIS

struct example
{
    int a;
    int b;
    int c;
};

#include <immutable/array.hpp>

void array_test()
{
    namespace im = immutable;
#ifndef FOR_REVERSE_ANALYSIS
    std::cout << std::boolalpha << "array test:" << std::endl;
#else
    printf("array test:\n");
#endif
    {
        constexpr auto arr = im::array{1};
        constexpr bool result = std::is_base_of_v<im::array_t, decltype(arr)>;
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\ttype_hint: " << result << std::endl;
#else
        printf("\ttype_hint: %d\n", result);
#endif
    }
    {
        constexpr auto arr = im::array{1}.to_array();
        constexpr bool result = std::is_same_v<const std::array<int, 1>, decltype(arr)>;
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tunpack: " << result << std::endl;
#else
        printf("\tunpack: %d\n", result);
#endif
    }
    {
        constexpr auto arr = im::array{1, 2};
        constexpr bool result = (arr.size() == 2);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tsize: " << result << std::endl;
#else
        printf("\tsize: %d\n", result);
#endif
    }
    {
        constexpr auto arr = im::array{1, 2};
        constexpr auto newArr = arr.push_back(4);
        constexpr auto result = (newArr.size() - arr.size() == 1);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tpush: " << result << std::endl;
#else
        printf("\tpush: %d\n", result);
#endif
    }
    {
        constexpr auto arr = im::array{1, 2};
        constexpr auto result = (arr.at(make_const(1)) == 2);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tat: " << result << std::endl;
#else
        printf("\tat: %d\n", result);
#endif
    }
    {
        constexpr auto arr = im::array{1, 2};
        constexpr auto sum = arr.reduce(
            [](auto acc, auto value) constexpr {
                return acc + value;
            },
            1);
        constexpr auto sum_short = arr.reduce(
            [](auto acc, auto value) constexpr {
                return acc + value;
            });
        constexpr bool result = (sum == 4 && sum_short == 3);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\treduce: " << result << std::endl;
#else
        printf("\treduce: %d\n", result);
#endif
    }
    {
        constexpr auto arr = im::array{1, 2, 3};
        constexpr auto newArr = arr.slice(make_const(1), make_const(2));
        constexpr bool result = (newArr.size() == 1 && newArr.at(make_const(0)) == 2);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tslice: " << result << std::endl;
#else
        printf("\tslice: %d\n", result);
#endif
    }
    {
        constexpr auto arr = im::array{1, 2, 3};
        constexpr auto arr2 = im::array{1, 2, 3};
        constexpr auto newArr = arr.splice(make_const(1), make_const(1), arr2);
        constexpr bool result = (newArr.size() == 5 && newArr.at(make_const(2)) == 2);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tsplice: " << result << std::endl;
#else
        printf("\tsplice: %d\n", result);
#endif
    }
    {
        constexpr auto arr = im::array{1, 2};
        constexpr auto newArr = arr.set(make_const(1), 3);
        constexpr bool result = (newArr.at(make_const(1)) == 3);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tset: " << result << std::endl;
#else
        printf("\tset: %d\n", result);
#endif
    }
}

#include <immutable/tuple.hpp>

void tuple_test()
{
    namespace im = immutable;
#ifndef FOR_REVERSE_ANALYSIS
    std::cout << std::boolalpha << "tuple test:" << std::endl;
#else
    printf("tuple test:\n");
#endif
    {
        constexpr auto tup = im::tuple{1};
        constexpr bool result = std::is_base_of_v<im::tuple_t, decltype(tup)>;
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\ttype_hint: " << result << std::endl;
#else
        printf("\ttype_hint: %d\n", result);
#endif
    }
    {
        constexpr auto tup = im::tuple{1}.to_tuple();
        constexpr bool result = std::is_same_v<const std::tuple<int>, decltype(tup)>;
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tunpack: " << result << std::endl;
#else
        printf("\tunpack: %d\n", result);
#endif
    }
    {
        constexpr auto tup = im::tuple{1, 2};
        constexpr bool result = (tup.size() == 2);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tsize: " << result << std::endl;
#else
        printf("\tsize: %d\n", result);
#endif
    }
    {
        constexpr auto tup = im::tuple{1, 2};
        constexpr auto newTup = tup.push_back('c');
        constexpr bool result = (newTup.size() - tup.size() == 1);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tpush: " << result << std::endl;
#else
        printf("\tpush: %d\n", result);
#endif
    }
    {
        constexpr auto tup = im::tuple{1, 2};
        constexpr bool result = (tup.at(make_const(1)) == 2);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tat: " << result << std::endl;
#else
        printf("\tat: %d\n", result);
#endif
    }
    {
        constexpr auto tup = im::tuple{1, 2};
        constexpr auto sum = tup.reduce(
            [](auto acc, auto value) constexpr {
                return acc + value;
            },
            1);
        constexpr auto sum_short = tup.reduce(
            [](auto acc, auto value) constexpr {
                if constexpr (std::is_same_v<std::nullptr_t, decltype(acc)>)
                {
                    return value;
                }
                else
                {
                    return acc + value;
                }
            });
        constexpr bool result = (sum == 4 && sum_short == 3);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\treduce: " << result << std::endl;
#else
        printf("\treduce: %d\n", result);
#endif
    }
    {
        constexpr auto tup = im::tuple{1, 2, 3};
        constexpr auto newTup = tup.slice(make_const(1), make_const(2));
        constexpr bool result = (newTup.size() == 1 && newTup.at(make_const(0)) == 2);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tslice: " << result << std::endl;
#else
        printf("\tslice: %d\n", result);
#endif
    }
    {
        constexpr auto tup = im::tuple{1, 2, 3};
        constexpr auto tup2 = im::tuple{1, 2, 3};
        constexpr auto newTup = tup.splice(make_const(1), make_const(1), tup2);
        constexpr bool result = (newTup.size() == 5 && newTup.at(make_const(2)) == 2);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tsplice: " << result << std::endl;
#else
        printf("\tsplice: %d\n", result);
#endif
    }
    {
        constexpr auto tup = im::tuple{1, 2};
        constexpr auto newTup = tup.set(make_const(1), 3);
        constexpr bool result = (newTup.at(make_const(1)) == 3);
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tset: " << result << std::endl;
#else
        printf("\tset: %d\n", result);
#endif
    }
}

#include <immutable/update.hpp>

void update_test()
{
    namespace im = immutable;
    namespace op = immutable::op;
#ifndef FOR_REVERSE_ANALYSIS
    std::cout << std::boolalpha << "update test:" << std::endl;
#else
    printf("update test:\n");
#endif
    {
        // equal to Js: const arr = [1]; const newArr = update(arr, {$push: [1, 2] });
        constexpr auto arr = im::array{1};
        constexpr auto newArr = op::update(
            arr,
            (op::Push, 1, 2));
        constexpr auto tup = im::tuple{12};
        // equal to Js: const tup = [12]; const newTup = update(tup, {$push: [1, 'c'] });
        constexpr auto newTup = op::update(
            tup,
            (op::Push, 1, 'c'));
        constexpr bool result = newArr.size() == 3 && newArr.at(make_const(2)) == 2 && newTup.size() == 3 && newTup.at(make_const(2)) == 'c';
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tpush op: " << result << std::endl;
#else
        printf("\tpush op: %d\n", result);
#endif
    }
    {
        // equal to Js: const arr = [1, 2]; const newArr = update(arr, {1: {$set: 3 } });
        constexpr auto arr = im::array{1, 2};
        constexpr auto newArr = op::update(
            arr,
            (op::attr(make_const(1)),
             (op::Set, 3)));
        // equal to Js: const tup = [1, 2]; const newTup = update(tup, {1: {$set: 'c' } });
        constexpr auto tup = im::tuple{1, 2};
        constexpr auto newTup = op::update(
            tup,
            (op::attr(make_const(1)),
             (op::Set, 'c')));
        // equal to Js: const obj = {a: 1, b: 2, c: 3}; const newobj = update(obj, {b: {$set: 0 } });
        constexpr auto obj = example{1, 2, 3};
        constexpr auto newObj = op::update(
            obj,
            (op::attr(make_const(&example::b)),
             (op::Set, 0)));
        constexpr bool result = newArr.at(make_const(1)) == 3 && newTup.at(make_const(1)) == 'c' && newObj.b == 0;
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tattr op: " << result << std::endl;
        std::cout << "\tset op: " << result << std::endl;
#else
        printf("\tattr op: %d\n", result);
        printf("\tset op: %d\n", result);
#endif
    }
    {
        // equal to Js: const arr = [1, 2, 3, 4]; const newArr = update(arr, {3: {$set: 1 }, 2: {$set: 2 } });
        constexpr auto arr = im::array{1, 2, 3, 4};
        constexpr auto newArr = op::update(
            arr,
            (op::attr(make_const(1)),
             (op::Set, 3),
             op::attr(make_const(2)),
             (op::Set, 2)));
        constexpr bool result = newArr.at(make_const(1)) == 3 && newArr.at(make_const(2)) == 2;
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tattr list op: " << result << std::endl;
#else
        printf("\tattr list op: %d\n", result);
#endif
    }
    {
        // equal to Js: const arr = [1, 2, 3]; const newArr = update(arr, {$splice: [[1, 0, 4], [3, 1]] });
        constexpr auto arr = im::array{1, 2, 3};
        constexpr auto newArr = op::update(
            arr,
            (op::Splice,
             im::tuple{make_const(1), make_const(0), 4},
             im::tuple{make_const(3), make_const(1)}));
        // equal to Js: const tup = [1, 2, 3]; const newTup = update(tup, {$splice: [[1, 0, 'c'], [3, 1]] });
        constexpr auto tup = im::tuple{1, 2, 3};
        constexpr auto newTup = op::update(
            tup,
            (op::Splice,
             im::tuple{make_const(1), make_const(0), 'c'},
             im::tuple{make_const(3), make_const(1)}));
        constexpr bool result = newArr.size() == 3 && newArr.at(make_const(1)) == 4 && newTup.size() == 3 && newTup.at(make_const(1)) == 'c';
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tsplice op: " << result << std::endl;
#else
        printf("\tsplice op: %d\n", result);
#endif
    }
    {
        // equal to Js: const arr = [1, 2]; const newArr = update(arr, {1: {$apply: function(x){ return x + 1; } } });
        constexpr auto arr = im::array{1, 2};
        constexpr auto newArr = op::update(
            arr,
            (op::attr(make_const(1)),
             (op::Apply, [](auto x) { return x + 1; })));
        // equal to Js: const tup = [1, 2]; const newTup = update(arr, {1: {$apply: function(x){ return x.toString(); } } });
        constexpr auto tup = im::tuple{1, 2};
        constexpr auto newTup = op::update(
            tup,
            (op::attr(make_const(1)),
             (op::Apply, [](auto x) { return char(x + '0'); })));
        // equal to Js: const obj = {a: 1, b: 2, c: 3}; const newObj = update(arr, {b: {$apply: function(x){ return x + 1; } } });
        constexpr auto obj = example{1, 2, 3};
        constexpr auto newObj = op::update(
            obj,
            (op::attr(make_const(&example::b)),
             (op::Apply, [](auto x) { return x + 1; })));
        constexpr bool result = newArr.at(make_const(1)) == 3 && newTup.at(make_const(1)) == '2' && newObj.b == 3;
#ifndef FOR_REVERSE_ANALYSIS
        std::cout << "\tapply op: " << result << std::endl;
#else
        printf("\tapply op: %d\n", result);
#endif
    }
}

using namespace std;

int main(int argc, char **argv)
{
    array_test();
    tuple_test();
    update_test();
#ifndef FOR_REVERSE_ANALYSIS
    cout << "test finish!" << endl;
#else
    printf("test finish!\n");
#endif
    return 0;
}
