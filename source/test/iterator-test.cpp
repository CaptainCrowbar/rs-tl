#include "rs-tl/iterator.hpp"
#include "rs-unit-test.hpp"
#include <algorithm>
#include <set>
#include <vector>

using namespace RS::TL;
using namespace RS::UnitTest;

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
