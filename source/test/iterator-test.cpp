#include "rs-tl/iterator.hpp"
#include "rs-unit-test.hpp"
#include <algorithm>
#include <iterator>
#include <set>
#include <vector>

using namespace RS::TL;
using namespace RS::UnitTest;

class Forward:
public FlexibleIterator<Forward, const int, std::forward_iterator_tag> {
public:
    Forward() = default;
    Forward(const std::vector<int>& v, size_t i): ptr_(v.data() + i) {}
    const int& operator*() const { return *ptr_; }
    Forward& operator++() { ++ptr_; return *this; }
    bool operator==(const Forward& i) const { return ptr_ == i.ptr_; }
private:
    const int* ptr_;
};

class Bidirectional:
public FlexibleIterator<Bidirectional, const int, std::bidirectional_iterator_tag> {
public:
    Bidirectional() = default;
    Bidirectional(const std::vector<int>& v, size_t i): ptr_(v.data() + i) {}
    const int& operator*() const { return *ptr_; }
    Bidirectional& operator++() { ++ptr_; return *this; }
    Bidirectional& operator--() { --ptr_; return *this; }
    bool operator==(const Bidirectional& i) const { return ptr_ == i.ptr_; }
private:
    const int* ptr_;
};

class Random:
public FlexibleIterator<Random, const int, std::random_access_iterator_tag> {
public:
    Random() = default;
    Random(const std::vector<int>& v, size_t i): ptr_(v.data() + i) {}
    const int& operator*() const { return *ptr_; }
    Random& operator++() { ++ptr_; return *this; }
    Random& operator--() { --ptr_; return *this; }
    Random& operator+=(ptrdiff_t n) { ptr_ += n; return *this; }
    ptrdiff_t operator-(const Random& i) const { return ptr_ - i.ptr_; }
    bool operator==(const Random& i) const { return ptr_ == i.ptr_; }
private:
    const int* ptr_;
};

void test_rs_tl_iterator_mixins() {

    TEST_TYPE(std::iterator_traits<Forward>::iterator_category, std::forward_iterator_tag);
    TEST_TYPE(std::iterator_traits<Bidirectional>::iterator_category, std::bidirectional_iterator_tag);
    TEST_TYPE(std::iterator_traits<Random>::iterator_category, std::random_access_iterator_tag);

    std::vector<int> v = {1,2,3,4,5};

    auto fr = irange(Forward(v, 0), Forward(v, v.size()));
    TEST_EQUAL(format_range(fr), "[1,2,3,4,5]");
    auto br = irange(Bidirectional(v, 0), Bidirectional(v, v.size()));
    TEST_EQUAL(format_range(br), "[1,2,3,4,5]");
    auto rr = irange(Random(v, 0), Random(v, v.size()));
    TEST_EQUAL(format_range(rr), "[1,2,3,4,5]");

    auto it = Random(v, 0);
    TEST_EQUAL(it[0], 1);
    TEST_EQUAL(it[4], 5);

}

void test_rs_tl_iterator_append_overwrite() {

    std::vector<int> u = {10,20,30,40,50};
    std::vector<int> v = {25,20,15,10,5};
    std::vector<int> w;
    std::set<int> s;

    TRY(std::copy(u.begin(), u.end(), append(w)));
    TEST_EQUAL(w.size(), 5u);
    TEST_EQUAL(format_range(w), "[10,20,30,40,50]");
    TRY(std::copy(v.begin(), v.end(), append(w)));
    TEST_EQUAL(w.size(), 10u);
    TEST_EQUAL(format_range(w), "[10,20,30,40,50,25,20,15,10,5]");
    TRY(std::copy(u.begin(), u.end(), overwrite(w)));
    TEST_EQUAL(w.size(), 5u);
    TEST_EQUAL(format_range(w), "[10,20,30,40,50]");

    TRY(std::copy(u.begin(), u.end(), append(s)));
    TEST_EQUAL(s.size(), 5u);
    TEST_EQUAL(format_range(s), "[10,20,30,40,50]");
    TRY(std::copy(v.begin(), v.end(), append(s)));
    TEST_EQUAL(s.size(), 8u);
    TEST_EQUAL(format_range(s), "[5,10,15,20,25,30,40,50]");
    TRY(std::copy(u.begin(), u.end(), overwrite(s)));
    TEST_EQUAL(s.size(), 5u);
    TEST_EQUAL(format_range(s), "[10,20,30,40,50]");

}

void test_rs_tl_iterator_subrange() {

    std::vector<int> v = {10,20,30,40,50};

    TEST_EQUAL(format_range(subrange(v, 0)), "[10,20,30,40,50]");
    TEST_EQUAL(format_range(subrange(v, 1)), "[20,30,40,50]");
    TEST_EQUAL(format_range(subrange(v, 2)), "[30,40,50]");
    TEST_EQUAL(format_range(subrange(v, 3)), "[40,50]");
    TEST_EQUAL(format_range(subrange(v, 4)), "[50]");
    TEST_EQUAL(format_range(subrange(v, 5)), "[]");
    TEST_EQUAL(format_range(subrange(v, 6)), "[]");
    TEST_EQUAL(format_range(subrange(v, -1)), "[50]");
    TEST_EQUAL(format_range(subrange(v, -2)), "[40,50]");
    TEST_EQUAL(format_range(subrange(v, -3)), "[30,40,50]");
    TEST_EQUAL(format_range(subrange(v, -4)), "[20,30,40,50]");
    TEST_EQUAL(format_range(subrange(v, -5)), "[10,20,30,40,50]");
    TEST_EQUAL(format_range(subrange(v, -6)), "[10,20,30,40,50]");

    TEST_EQUAL(format_range(subrange(v, 0, 0)), "[]");
    TEST_EQUAL(format_range(subrange(v, 0, 1)), "[10]");
    TEST_EQUAL(format_range(subrange(v, 0, 2)), "[10,20]");
    TEST_EQUAL(format_range(subrange(v, 0, 3)), "[10,20,30]");
    TEST_EQUAL(format_range(subrange(v, 0, 4)), "[10,20,30,40]");
    TEST_EQUAL(format_range(subrange(v, 0, 5)), "[10,20,30,40,50]");
    TEST_EQUAL(format_range(subrange(v, 0, 6)), "[10,20,30,40,50]");
    TEST_EQUAL(format_range(subrange(v, 1, 0)), "[]");
    TEST_EQUAL(format_range(subrange(v, 1, 1)), "[20]");
    TEST_EQUAL(format_range(subrange(v, 1, 2)), "[20,30]");
    TEST_EQUAL(format_range(subrange(v, 1, 3)), "[20,30,40]");
    TEST_EQUAL(format_range(subrange(v, 1, 4)), "[20,30,40,50]");
    TEST_EQUAL(format_range(subrange(v, 1, 5)), "[20,30,40,50]");
    TEST_EQUAL(format_range(subrange(v, 2, 0)), "[]");
    TEST_EQUAL(format_range(subrange(v, 2, 1)), "[30]");
    TEST_EQUAL(format_range(subrange(v, 2, 2)), "[30,40]");
    TEST_EQUAL(format_range(subrange(v, 2, 3)), "[30,40,50]");
    TEST_EQUAL(format_range(subrange(v, 2, 4)), "[30,40,50]");
    TEST_EQUAL(format_range(subrange(v, 3, 0)), "[]");
    TEST_EQUAL(format_range(subrange(v, 3, 1)), "[40]");
    TEST_EQUAL(format_range(subrange(v, 3, 2)), "[40,50]");
    TEST_EQUAL(format_range(subrange(v, 3, 3)), "[40,50]");
    TEST_EQUAL(format_range(subrange(v, 4, 0)), "[]");
    TEST_EQUAL(format_range(subrange(v, 4, 1)), "[50]");
    TEST_EQUAL(format_range(subrange(v, 4, 2)), "[50]");
    TEST_EQUAL(format_range(subrange(v, 5, 0)), "[]");
    TEST_EQUAL(format_range(subrange(v, 5, 1)), "[]");
    TEST_EQUAL(format_range(subrange(v, 6, 0)), "[]");
    TEST_EQUAL(format_range(subrange(v, 6, 1)), "[]");

    TEST_EQUAL(format_range(subrange(v, 0, -6)), "[]");
    TEST_EQUAL(format_range(subrange(v, 0, -5)), "[]");
    TEST_EQUAL(format_range(subrange(v, 0, -4)), "[10]");
    TEST_EQUAL(format_range(subrange(v, 0, -3)), "[10,20]");
    TEST_EQUAL(format_range(subrange(v, 0, -2)), "[10,20,30]");
    TEST_EQUAL(format_range(subrange(v, 0, -1)), "[10,20,30,40]");
    TEST_EQUAL(format_range(subrange(v, 1, -5)), "[]");
    TEST_EQUAL(format_range(subrange(v, 1, -4)), "[]");
    TEST_EQUAL(format_range(subrange(v, 1, -3)), "[20]");
    TEST_EQUAL(format_range(subrange(v, 1, -2)), "[20,30]");
    TEST_EQUAL(format_range(subrange(v, 1, -1)), "[20,30,40]");
    TEST_EQUAL(format_range(subrange(v, 2, -4)), "[]");
    TEST_EQUAL(format_range(subrange(v, 2, -3)), "[]");
    TEST_EQUAL(format_range(subrange(v, 2, -2)), "[30]");
    TEST_EQUAL(format_range(subrange(v, 2, -1)), "[30,40]");
    TEST_EQUAL(format_range(subrange(v, 3, -3)), "[]");
    TEST_EQUAL(format_range(subrange(v, 3, -2)), "[]");
    TEST_EQUAL(format_range(subrange(v, 3, -1)), "[40]");
    TEST_EQUAL(format_range(subrange(v, 4, -2)), "[]");
    TEST_EQUAL(format_range(subrange(v, 4, -1)), "[]");
    TEST_EQUAL(format_range(subrange(v, 5, -1)), "[]");
    TEST_EQUAL(format_range(subrange(v, 6, -1)), "[]");

    TEST_EQUAL(format_range(subrange(v, -5, 0)), "[]");
    TEST_EQUAL(format_range(subrange(v, -5, 1)), "[10]");
    TEST_EQUAL(format_range(subrange(v, -5, 2)), "[10,20]");
    TEST_EQUAL(format_range(subrange(v, -5, 3)), "[10,20,30]");
    TEST_EQUAL(format_range(subrange(v, -5, 4)), "[10,20,30,40]");
    TEST_EQUAL(format_range(subrange(v, -5, 5)), "[10,20,30,40,50]");
    TEST_EQUAL(format_range(subrange(v, -5, 6)), "[10,20,30,40,50]");
    TEST_EQUAL(format_range(subrange(v, -4, 0)), "[]");
    TEST_EQUAL(format_range(subrange(v, -4, 1)), "[20]");
    TEST_EQUAL(format_range(subrange(v, -4, 2)), "[20,30]");
    TEST_EQUAL(format_range(subrange(v, -4, 3)), "[20,30,40]");
    TEST_EQUAL(format_range(subrange(v, -4, 4)), "[20,30,40,50]");
    TEST_EQUAL(format_range(subrange(v, -4, 5)), "[20,30,40,50]");
    TEST_EQUAL(format_range(subrange(v, -3, 0)), "[]");
    TEST_EQUAL(format_range(subrange(v, -3, 1)), "[30]");
    TEST_EQUAL(format_range(subrange(v, -3, 2)), "[30,40]");
    TEST_EQUAL(format_range(subrange(v, -3, 3)), "[30,40,50]");
    TEST_EQUAL(format_range(subrange(v, -3, 4)), "[30,40,50]");
    TEST_EQUAL(format_range(subrange(v, -2, 0)), "[]");
    TEST_EQUAL(format_range(subrange(v, -2, 1)), "[40]");
    TEST_EQUAL(format_range(subrange(v, -2, 2)), "[40,50]");
    TEST_EQUAL(format_range(subrange(v, -2, 3)), "[40,50]");
    TEST_EQUAL(format_range(subrange(v, -1, 0)), "[]");
    TEST_EQUAL(format_range(subrange(v, -1, 1)), "[50]");
    TEST_EQUAL(format_range(subrange(v, -1, 2)), "[50]");

    TEST_EQUAL(format_range(subrange(v, -5, -5)), "[]");
    TEST_EQUAL(format_range(subrange(v, -5, -4)), "[10]");
    TEST_EQUAL(format_range(subrange(v, -5, -3)), "[10,20]");
    TEST_EQUAL(format_range(subrange(v, -5, -2)), "[10,20,30]");
    TEST_EQUAL(format_range(subrange(v, -5, -1)), "[10,20,30,40]");
    TEST_EQUAL(format_range(subrange(v, -4, -4)), "[]");
    TEST_EQUAL(format_range(subrange(v, -4, -3)), "[20]");
    TEST_EQUAL(format_range(subrange(v, -4, -2)), "[20,30]");
    TEST_EQUAL(format_range(subrange(v, -4, -1)), "[20,30,40]");
    TEST_EQUAL(format_range(subrange(v, -3, -3)), "[]");
    TEST_EQUAL(format_range(subrange(v, -3, -2)), "[30]");
    TEST_EQUAL(format_range(subrange(v, -3, -1)), "[30,40]");
    TEST_EQUAL(format_range(subrange(v, -2, -2)), "[]");
    TEST_EQUAL(format_range(subrange(v, -2, -1)), "[40]");
    TEST_EQUAL(format_range(subrange(v, -1, -1)), "[]");

}
