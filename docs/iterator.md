# Iterator Types and Algorithms

_[Template Library by Ross Smith](index.html)_

```c++
#include "rs-tl/iterator.hpp"
namespace RS::TL;
```

## Contents

* TOC
{:toc}

## Iterator classes

```c++
template <typename Container> class AppendIterator;
```

An output iterator that appends items to the container.

```c++
template <typename Container> AppendIterator<Container> append(Container& c);
template <typename Container> AppendIterator<Container> overwrite(Container& c);
```

The `append()` function returns an append iterator for the container. The
`overwrite()` function is a convenience function that clears the container
and then returns an append iterator.

## Mixin classes

These are intended to be used as base classes, following the well known CRTP
pattern, and will supply the boilerplate to define several common member or
non-member functions given certain pre-defined functions of the derived
type `T`.

```c++
template <typename T, typename CV> struct InputIterator;
template <typename T> struct OutputIterator;
template <typename T, typename CV> struct ForwardIterator;
template <typename T, typename CV> struct BidirectionalIterator;
template <typename T, typename CV> struct RandomAccessIterator;
```

`CV` is either `V` or `const V`, where `V` is the value type, to determine
whether a mutable or const iterator is generated. In the table below, `t` and
`u` are objects of type `T`, `x` is an object of the iterator's value type,
and `n` is an integer.

| Mixin                    | Requires           | Defines                                                                  |
| -----                    | --------           | -------                                                                  |
| `InputIterator`          | `*t ++t t==u`      | `t-> t++ t!=u`                                                           |
| `OutputIterator`         | `t=x`              | `*t ++t t++`                                                             |
| `ForwardIterator`        | `*t ++t t==u`      | `t-> t++ t!=u`                                                           |
| `BidirectionalIterator`  | `*t ++t --t t==u`  | `t-> t++ t-- t!=u`                                                       |
| `RandomAccessIterator`   | `*t t+=n t-u`      | `t-> t[n] ++t t++ --t t-- t-=n t+n n+t t-n t==u t!=u t<u t>u t<=u t>=u`  |

## Range classes

```c++
template <typename Iterator> struct Irange {
    Iterator first, second;
    Iterator begin() const { return first; }
    Iterator end() const { return second; }
    bool empty() const noexcept { return first == second; }
};
template <typename Iterator> Irange<Iterator>
    irange(Iterator i, Iterator j);
template <typename Iterator> Irange<Iterator>
    irange(std::pair<Iterator, Iterator> p);
```

This turns a pair of iterators into a usable range.
