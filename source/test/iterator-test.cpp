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
