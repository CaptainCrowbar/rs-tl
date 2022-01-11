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
using `"rs-skeleton.hpp"`.

[My unit test library](https://github.com/CaptainCrowbar/rs-unit-test) is used
for this library's unit tests.

## Index

* [Algorithms](algorithm.html)
* [Bitwise operations](binary.html)
* [Basic types](types.html)
* [Iterator types and algorithms](iterator.html)
* [Metaprogramming utilities](meta.html)
* [Scope guards](guard.html)
* Containers
    * [Stack](stack.html)
    * [Topological order](topological-order.html)
* [Version information](version.html)
