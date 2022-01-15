# Basic Types

_[Template Library by Ross Smith](index.html)_

```c++
#include "rs-tl/types.hpp"
namespace RS::TL;
```

## Contents

* TOC
{:toc}

## Constants

```c++
constexpr size_t npos = std::string::npos;
```

Imported for convenience.

## Mixin classes

These are intended to be used as base classes, following the well known CRTP
pattern, and will supply the boilerplate to define several common member or
non-member functions given certain pre-defined functions of the derived
type `T`.

```c++
template <typename T> struct TotalOrder;
```

Given `==` and `<` operators for `T`, this defines the other comparison
operators: `!= > <= >=`.

## Static assert support

```c++
template <typename T> constexpr bool dependent_false = false;
```

Allows compile time failure in an `if constexpr` branch.

## Type traits

```c++
template <typename T> constexpr bool is_iterator;
```

True if `T` is an iterator.

```c++
template <typename T> constexpr bool is_range;
```

True if `T` is a range, defined as having `begin(T)` and `end(T)` functions
that can be found either by ADL or in `namespace std`.

```c++
template <typename T> using RangeIterator = [iterator type];
template <typename T> using RangeValue = [value type];
```

The iterator and value types of a range, or `void` if `T` is not a range.

```c++
template <typename T> constexpr bool is_input_iterator;
template <typename T> constexpr bool is_input_range;
template <typename T> constexpr bool is_output_iterator;
template <typename T> constexpr bool is_output_range;
template <typename T> constexpr bool is_forward_iterator;
template <typename T> constexpr bool is_forward_range;
template <typename T> constexpr bool is_bidirectional_iterator;
template <typename T> constexpr bool is_bidirectional_range;
template <typename T> constexpr bool is_random_access_iterator;
template <typename T> constexpr bool is_random_access_range;
```

These indicate whether an iterator type, or the iterator type of a range,
meets the requirements for one of the standard iterator concepts. Each of
these will be true if the iterator's category is at least equal to the named
category.
