# Template Library

By Ross Smith

_[GitHub repository](https://github.com/CaptainCrowbar/rs-tl)_

## Overview

```c++
#include "rs-tl.hpp"
namespace RS::TL;
```

This is my personal template library of algorithms, containers, and
metaprogramming utilities.

This is a headers-only library. The CMake file includes an `install` target to
copy the headers into `/usr/local/include` or the equivalent. Headers can be
included individually as required, or the entire library can be included
using `"rs-tl.hpp"`.

[My unit test library](https://github.com/CaptainCrowbar/rs-unit-test) is used
for this library's unit tests.

## Index

* [Algorithms](algorithm.html)
* [Basic types](types.html)
* [Bitwise operations](binary.html)
* [Enumeration types](enum.html)
* [Fixed-size binary numbers](fixed-binary.html)
* [Iterator types and algorithms](iterator.html)
* [Logging](log.html)
* [Metaprogramming utilities](meta.html)
* [Scope guards](guard.html)
* [Thread](thread.html)
* [UUID](uuid.html)
* Containers
    * [Bounded array on the stack](bounded-array.html)
    * [Compact array optimized for small size](compact-array.html)
    * [Indexed table with multiple keys](index-table.html)
    * [Mirror map with two-way lookup](mirror-map.html)
    * [Stack](stack.html)
    * [Topological order](topological-order.html)
* [Version information](version.html)
