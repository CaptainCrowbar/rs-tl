# Algorithms

_[Template Library by Ross Smith](index.html)_

```c++
#include "rs-tl/algorithm.hpp"
namespace RS::TL;
```

## Contents

* TOC
{:toc}

## Container algorithms

```c++
template <typename Container, typename T>
    void remove_in(Container& con, const T& t);
template <typename Container, typename Predicate>
    void remove_in_if(Container& con, Predicate p);
template <typename Container, typename Predicate>
    void remove_in_if_not(Container& con, Predicate p);
template <typename Container>
    void unique_in(Container& con);
template <typename Container, typename BinaryPredicate>
    void unique_in(Container& con, BinaryPredicate eq);
template <typename Container>
    void sort_unique_in(Container& con);
template <typename Container, typename BinaryPredicate>
    void sort_unique_in(Container& con, BinaryPredicate cmp);
```

These carry out the same algorithms as the similarly named STL functions,
except that unwanted elements are removed from the container rather than
shuffled to the end. The `sort_unique_in()` functions perform a sort followed
by removing equivalent elements from the container; like `std::sort()`, its
predicate has less-than semantics, where that of `unique_in()`, like
`std::unique()`, has equality semantics.

## Difference

```c++
template <typename RandomAccessRange> struct DiffEntry {
    using iterator = [range const iterator];
    using subrange = Irange<iterator>;
    subrange del;
    subrange ins;
};
template <typename RandomAccessRange>
    using DiffList = std::vector<DiffEntry<RandomAccessRange>>;
```

Supporting types.

```c++
template <typename RandomAccessRange>
    DiffList<RandomAccessRange> diff(const RandomAccessRange& lhs,
        const RandomAccessRange& rhs);
template <typename RandomAccessRange, typename EqualityPredicate>
    DiffList<RandomAccessRange> diff(const RandomAccessRange& lhs,
        const RandomAccessRange& rhs, EqualityPredicate eq);
```

This is an implementation of the widely used diff algorithm, based on
[Eugene Myers' 1986 paper](http://xmailserver.org/diff2.pdf).

The return value is a list of diffs, each consisting of two pairs of
iterators. The `del` member is a subrange of `lhs` indicating which elements
have been removed, and the `ins` member is a subrange of `rhs` indicating
which elements have been inserted in the same location. At least one subrange
in each diff entry will be non-empty.

Complexity: _O(k(m+n))_, where _m_ and _n_ are the lengths of the input ranges
and _k_ is the number of differences.

## Edit distance

```c++
template <typename ForwardRange1, typename ForwardRange2>
    int edit_distance(const ForwardRange1& range1,
        const ForwardRange2& range2);
template <typename ForwardRange1, typename ForwardRange2, typename T>
    T edit_distance(const ForwardRange1& range1,
        const ForwardRange2& range2, T ins, T del, T sub);
```

These return the edit distance (Levenshtein distance) between two ranges,
based on the number of insertions, deletions, and substitutions required to
transform one range into the other. By default, each operation is given a
weight of 1; optionally, explicit weights can be given to each operation. The
weight type `T` must be an arithmetic type. Behaviour is undefined if any of
the weights are negative.

Complexity: _O(mn)_, where _m_ and _n_ are the lengths of the input ranges.

## Subsets

```c++
template <typename RandomAccessContainer> std::vector<RandomAccessContainer>
    subsets(const RandomAccessContainer& con);
template <typename RandomAccessContainer> std::vector<RandomAccessContainer>
    subsets(const RandomAccessContainer& con, int k);
```

Return a list of all subsets of the container, or all subsets of a given size.
Container elements are always treated as distinct, with no equality checking,
so these will always return exactly _2<sup>n</sup>_ or _C(n,k)=n!/k!(n-k)!_
subsets (where _n_ is the container size).

No promises are made about what order the subsets will be listed in. For the
second function, behaviour is undefined if `k<0` or `k>n`.

Complexity: _O(2<sup>n</sup>)_ for the first version; _O(kC(n,k))_ for the
second.
