#pragma once

#include "rs-tl/iterator.hpp"
#include "rs-tl/types.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <type_traits>
#include <vector>

namespace RS::TL {

    // Container algorithms

    template <typename Container, typename T>
    void remove_in(Container& con, const T& t) {
        con.erase(std::remove(con.begin(), con.end(), t), con.end());
    }

    template <typename Container, typename Predicate>
    void remove_in_if(Container& con, Predicate p) {
        con.erase(std::remove_if(con.begin(), con.end(), p), con.end());
    }

    template <typename Container, typename Predicate>
    void remove_in_if_not(Container& con, Predicate p) {
        using T = std::decay_t<decltype(*con.begin())>;
        con.erase(std::remove_if(con.begin(), con.end(), [p] (const T& x) { return ! p(x); }), con.end());
    }

    template <typename Container>
    void unique_in(Container& con) {
        con.erase(std::unique(con.begin(), con.end()), con.end());
    }

    template <typename Container, typename BinaryPredicate>
    void unique_in(Container& con, BinaryPredicate eq) {
        con.erase(std::unique(con.begin(), con.end(), eq), con.end());
    }

    template <typename Container>
    void sort_unique_in(Container& con) {
        std::sort(con.begin(), con.end());
        unique_in(con);
    }

    template <typename Container, typename BinaryPredicate>
    void sort_unique_in(Container& con, BinaryPredicate cmp) {
        using T = std::decay_t<decltype(*con.begin())>;
        std::sort(con.begin(), con.end(), cmp);
        unique_in(con, [cmp] (const T& a, const T& b) { return ! cmp(a, b); });
    }

    // Myers difference algorithm
    // http://xmailserver.org/diff2.pdf

    template <typename RandomAccessRange>
    struct DiffEntry {
        using iterator = RangeIterator<const RandomAccessRange>;
        using subrange = Irange<iterator>;
        subrange del;
        subrange ins;
    };

    template <typename RandomAccessRange> using DiffList = std::vector<DiffEntry<RandomAccessRange>>;

    namespace Detail {

        struct DiffHelper {

        public:

            using match_function = std::function<bool(ptrdiff_t, ptrdiff_t)>;
            using insert_function = std::function<void(ptrdiff_t, ptrdiff_t, ptrdiff_t, ptrdiff_t)>;

            DiffHelper(ptrdiff_t l_size, ptrdiff_t r_size, match_function match, insert_function insert):
            fwd_path_(2 * (l_size + r_size + 1)),
            rev_path_(2 * (l_size + r_size + 1)),
            fwd_(fwd_path_.begin() + l_size + r_size + 1),
            rev_(rev_path_.begin() + l_size + r_size + 1),
            l_changes_(l_size, false),
            r_changes_(r_size, false),
            l_size_(l_size),
            r_size_(r_size),
            match_(match),
            insert_(insert) {}

            void resolve() {
                longest_common(0, l_size_, 0, r_size_);
                collect_diffs();
            }

        private:

            using offset_list = std::vector<ptrdiff_t>;
            using offset_iterator = offset_list::iterator;

            offset_list fwd_path_;
            offset_list rev_path_;
            offset_iterator fwd_;
            offset_iterator rev_;
            std::vector<bool> l_changes_;
            std::vector<bool> r_changes_;
            ptrdiff_t l_size_;
            ptrdiff_t r_size_;
            match_function match_;
            insert_function insert_;

            void collect_diffs() {
                ptrdiff_t lpos = 0, rpos = 0;
                for (; lpos != l_size_ && rpos != r_size_; ++lpos, ++rpos) {
                    ptrdiff_t lstart = lpos, rstart = rpos;
                    for (; lpos != l_size_ && l_changes_[lpos]; ++lpos) {}
                    for (; rpos != r_size_ && r_changes_[rpos]; ++rpos) {}
                    if (lstart != lpos || rstart != rpos)
                        insert_(lstart, lpos, rstart, rpos);
                }
                if (lpos != l_size_ || rpos != r_size_)
                    insert_(lpos, l_size_, rpos, r_size_);
            }

            void longest_common(ptrdiff_t l1, ptrdiff_t l2, ptrdiff_t r1, ptrdiff_t r2) {
                for (; l1 < l2 && r1 < r2 && match_(l1, r1); ++l1, ++r1) {}
                for (; l1 < l2 && r1 < r2 && match_(l2 - 1, r2 - 1); --l2, --r2) {}
                if (l1 == l2) {
                    std::fill(r_changes_.begin() + r1, r_changes_.begin() + r2, true);
                    return;
                }
                if (r1 == r2) {
                    std::fill(l_changes_.begin() + l1, l_changes_.begin() + l2, true);
                    return;
                }
                fwd_[1] = l1;
                rev_[-1] = l2;
                ptrdiff_t d1 = l1 - r1, d2 = l2 - r2, delta = d2 - d1;
                for (ptrdiff_t d = 0;; ++d) {
                    for (ptrdiff_t k = - d; k <= d; k = k + 2) {
                        ptrdiff_t i = k == - d ? fwd_[k + 1] : fwd_[k - 1] + 1;
                        if (k < d)
                            i = std::max(i, fwd_[k + 1]);
                        for (ptrdiff_t j = i - k - d1; i < l2 && j < r2 && match_(i, j); ++i, ++j) {}
                        fwd_[k] = i;
                        if ((delta & 1) && k > delta - d && k < delta + d && rev_[k - delta] <= fwd_[k]) {
                            longest_common(l1, fwd_[k], r1, fwd_[k] - k - d1);
                            longest_common(fwd_[k], l2, fwd_[k] - k - d1, r2);
                            return;
                        }
                    }
                    for (ptrdiff_t k = - d; k <= d; k = k + 2) {
                        ptrdiff_t i = k == d ? rev_[k - 1] : rev_[k + 1] - 1;
                        if (k > - d)
                            i = std::min(i, rev_[k - 1]);
                        for (ptrdiff_t j = i - k - d2; i > l1 && j > r1 && match_(i - 1, j - 1); --i, --j) {}
                        rev_[k] = i;
                        if (! (delta & 1) && k >= - d - delta && k <= d - delta && rev_[k] <= fwd_[k + delta]) {
                            longest_common(l1, fwd_[k + delta], r1, fwd_[k + delta] - k - d2);
                            longest_common(fwd_[k + delta], l2, fwd_[k + delta] - k - d2, r2);
                            return;
                        }
                    }
                }
            }

        };

    }

    template <typename RandomAccessRange, typename EqualityPredicate>
    DiffList<RandomAccessRange> diff(const RandomAccessRange& lhs, const RandomAccessRange& rhs, EqualityPredicate eq) {

        using std::begin;
        using std::end;

        auto l_begin = begin(lhs);
        auto l_end = end(lhs);
        auto l_size = std::distance(l_begin, l_end);
        auto r_begin = begin(rhs);
        auto r_end = end(rhs);
        auto r_size = std::distance(r_begin, r_end);

        DiffList<RandomAccessRange> diffs;

        Detail::DiffHelper dh(l_size, r_size,
            [eq,l_begin,r_begin] (ptrdiff_t i, ptrdiff_t j) { return eq(l_begin[i], r_begin[j]); },
            [&diffs,l_begin,r_begin] (ptrdiff_t lpos1, ptrdiff_t lpos2, ptrdiff_t rpos1, ptrdiff_t rpos2) {
                diffs.push_back({{l_begin + lpos1, l_begin + lpos2}, {r_begin + rpos1, r_begin + rpos2}});
            });

        dh.resolve();

        return diffs;

    }

    template <typename RandomAccessRange>
    DiffList<RandomAccessRange> diff(const RandomAccessRange& lhs, const RandomAccessRange& rhs) {
        return diff(lhs, rhs, std::equal_to<RangeValue<RandomAccessRange>>());
    }

    // Edit distance (Levenshtein distance)

    template <typename ForwardRange1, typename ForwardRange2, typename T>
    T edit_distance(const ForwardRange1& range1, const ForwardRange2& range2, T ins, T del, T sub) {

        static_assert(std::is_arithmetic_v<T>);

        using std::begin;
        using std::end;

        const ptrdiff_t n1 = std::distance(begin(range1), end(range1));
        const ptrdiff_t n2 = std::distance(begin(range2), end(range2));

        std::vector<T> buf1(n2 + 1);
        std::vector<T> buf2(n2 + 1);

        for (ptrdiff_t j = 1; j <= n2; ++j)
            buf1[j] = buf1[j - 1] + ins;

        auto p = begin(range1);

        for (ptrdiff_t i = 1; i <= n1; ++i, ++p) {
            buf2[0] = del * T(i);
            auto q = begin(range2);
            for (ptrdiff_t j = 1; j <= n2; ++j, ++q)
                buf2[j] = std::min({buf2[j - 1] + ins, buf1[j] + del, buf1[j - 1] + (*p == *q ? T(0) : sub)});
            buf1.swap(buf2);
        }

        return buf1[n2];

    }

    template <typename ForwardRange1, typename ForwardRange2>
    int edit_distance(const ForwardRange1& range1, const ForwardRange2& range2) {
        return edit_distance(range1, range2, 1, 1, 1);
    }

    // Subsets

    namespace Detail {

        class Bitset {
        public:
            Bitset() = default;
            explicit Bitset(size_t bits): words_((bits + 63) / 64, 0) {}
            void operator++() noexcept {
                for (auto& word: words_)
                    if (++word != 0)
                        break;
            }
            bool operator[](size_t index) const noexcept {
                return ((words_[index / 64] >> (index % 64)) & size_t(1)) == 1;
            }
        private:
            std::vector<uint64_t> words_; // little endian
        };

    }

    template <typename RandomAccessContainer>
    std::vector<RandomAccessContainer> subsets(const RandomAccessContainer& con) {

        std::vector<RandomAccessContainer> subs;
        RandomAccessContainer sub;
        Detail::Bitset bits(con.size());
        size_t n = con.size();

        for (;;) {
            sub.clear();
            for (size_t i = 0; i < n; ++i)
                if (bits[i])
                    sub.insert(sub.end(), con[i]);
            subs.push_back(sub);
            if (sub.size() == n)
                break;
            ++bits;
        }

        return subs;

    }

    template <typename RandomAccessContainer>
    std::vector<RandomAccessContainer> subsets(const RandomAccessContainer& con, int k) {

        std::vector<RandomAccessContainer> subs;
        RandomAccessContainer sub(k, {});
        std::vector<int> indices(k);
        std::iota(indices.begin(), indices.end(), 0);
        int n = int(con.size());

        for (;;) {

            std::transform(indices.begin(), indices.end(), sub.begin(),
                [&con] (int i) { return con[i]; });
            subs.push_back(sub);
            int stop = n;
            int i;

            for (i = k - 1; i >= 0; --i, --stop) {
                ++indices[i];
                if (indices[i] < stop)
                    break;
            }

            if (i < 0)
                break;

            for (++i; i < k; ++i)
                indices[i] = indices[i - 1] + 1;

        }

        return subs;

    }


}
