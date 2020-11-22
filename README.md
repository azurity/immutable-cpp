# immutable-cpp

A header only meta-programming library.

Try to provide the same functions as [immutability-helper](https://github.com/kolodny/immutability-helper).

## Why

You may have realized that meta-programming with `constexpr` in C++ is similar to immutable programming.

Immutable programming has been widely used in web frontend development, and there are some awesome library to do this.

I like to use immutability-helper, so I write this library to let me write C++ easily.

## How to use

Add `include/immutable` to your project. This project use xmake to provide a demo. You can use what you like.

You need `C++20` (maybe `C++17` & `-fconcepts` is also ok). I test this use g++9.

## API

all function is `constexpr`, all function will return a new immutable value.

### `immutable/utils.hpp`

namespace is `immutable`

- `make_const`: a macro to make a `std::integral_constant` easy.

> - `const` means the argument is a `std::integral_constant`
> - `any` means a common type is ok.

### `immutable/array.hpp`

namespace is `immutable`

- `array`: like `std::array`
    - a type hint: it inherit `array_t`
    - `to_array()`: get a `std::array`
    - `size()`: inherit from `std::array`, get the size
    - `psuh_back(any)`: a constexpr push_back
    - `at(const i)`: get the item at position `i`
    - `reduce(func, any [left])`: do reduce use the `func`
    - `slice(const begin, const end)`: get a slice
    - `splice(const begin, const size, any list)`: same as `splice` in javascript
    - `set(const i, any value)`: make a copy that value of position `i` is `value`

### `immutable/tuple.hpp`

namespace is `immutable`

- `tuple`: like `std::tuple`
    - a type hint: it inherit `tuple_t`
    - `to_tuple()`: get a `std::tuple`
    - `size()`: get the size
    - `psuh_back(any)`: a constexpr push_back
    - `at(const i)`: get the item at position `i`
    - `reduce(func, any [left])`: do reduce use the `func`
    - `slice(const begin, const end)`: get a slice
    - `splice(const begin, const size, any list)`: same as `splice` in javascript
    - `set(const i, any value)`: make a copy that value of position `i` is `value`


### `immutable/update.hpp`

namespace is `immutable::op`

> you need include this file after container's header.

- `update(any value, op)`: the function like `update()` in `immutability-helper`
- `Push`: like `$push`, see example
- `attr(const attr)`: do access, see example
- `Set`: like `$set`, see example
- `Splice`: like `$splice`, see example
- `Apply`: like `$apply`, see example

## Examples

Assuming you have written the follow code.
```c++
#include "immutable/array.hpp"
#include "immutable/tuple.hpp"
#include "immutable/update.hpp"
using namespace immutable;
using namespace immutable::op;
```

### Simple push

in js
```js
const initialArray = [1, 2, 3];
const newArray = update(initialArray, {$push: [4]});
```
in C++
```c++
constexpr auto initialArray = array{1, 2, 3};
constexpr auto newArray = update(initialArray, (Push, 4));
```

### Nested collections

in js
```js
const collection = [1, 2, [12, 17, 15]];
const newCollection = update(collection, {2: {$splice: [[1, 1, 13, 14]]}});
```
in C++
```c++
constexpr auto collection = tuple{1, 2, array{12, 17, 15}};
constexpr auto newCollection = update(collection, (attr(make_const(2)), (Splice, tuple{make_const(1), make_const(1), 13, 14})));
```

### Updating a value based on its current one

in js
```js
const obj = {a: 5, b: 3};
const newObj = update(obj, {b: {$apply: function(x) {return x * 2;}}});
const newObj2 = update(obj, {b: {$set: obj.b * 2}});
```
in C++
```c++
struct Obj { int a; int b; } constexpr obj{5, 3};
constexpr auto newObj = update(obj, (attr(make_const(&Obj::b)), (Apply, [](auto x){ return x * 2; })));
constexpr auto newObj2 = update(obj, (attr(make_const(&Obj::b)), (Set, obj.b * 2)));
```

**for more example, you can see `main.cpp`**

## Roadmap

- [ ] more container type
    - [ ] string
    - [ ] set
    - [ ] map
    - [ ] bitset
- [ ] more operator
    - [ ] $merge
    - [ ] $add
    - [ ] $remove
    - [ ] $toggle
- [ ] more detailed constraints
