#include "rs-tl/algorithm.hpp"
#include "rs-unit-test.hpp"
#include <iterator>
#include <string>
#include <vector>

using namespace RS::TL;
using namespace std::literals;

namespace {

    template <typename Range, typename DiffList>
    std::string report_diff(const Range& lhs, const Range& rhs, const DiffList& delta) {
        std::string list;
        for (auto& d: delta) {
            list += "A" + std::to_string(d.del.begin() - std::begin(lhs));
            if (d.del.begin() != d.del.end())
                list += "+" + std::to_string(d.del.end() - d.del.begin());
            list += "=>B" + std::to_string(d.ins.begin() - std::begin(rhs));
            if (d.ins.begin() != d.ins.end())
                list += "+" + std::to_string(d.ins.end() - d.ins.begin());
            list += ";";
        }
        return list;
    }

}

void test_rs_tl_algorithm_diff() {

    using vector_type = std::vector<int>;
    using diff_list = decltype(diff(vector_type(), vector_type()));

    vector_type a, b;
    diff_list delta;

    TRY(delta = diff(a, b));
    TEST_EQUAL(delta.size(), 0u);

    a = {1,2,3};
    b = {1,2,3,4,5};
    TRY(delta = diff(a, b));
    TEST_EQUAL(delta.size(), 1u);
    TEST_EQUAL(report_diff(a, b, delta),
        "A3=>B3+2;"
    );

    a = {1,2,3,4,5};
    b = {3,4,5};
    TRY(delta = diff(a, b));
    TEST_EQUAL(delta.size(), 1u);
    TEST_EQUAL(report_diff(a, b, delta),
        "A0+2=>B0;"
    );

    a = {2,4,6};
    b = {1,2,3,4,5,6};
    TRY(delta = diff(a, b));
    TEST_EQUAL(delta.size(), 3u);
    TEST_EQUAL(report_diff(a, b, delta),
        "A0=>B0+1;"
        "A1=>B2+1;"
        "A2=>B4+1;"
    );

    a = {1,2,3,4,5,6};
    b = {1,3,5};
    TRY(delta = diff(a, b));
    TEST_EQUAL(delta.size(), 3u);
    TEST_EQUAL(report_diff(a, b, delta),
        "A1+1=>B1;"
        "A3+1=>B2;"
        "A5+1=>B3;"
    );

    a = {1,2,3,4,5,6,10,11,12};
    b = {1,2,3,7,8,9,10,11,12};
    TRY(delta = diff(a, b));
    TEST_EQUAL(delta.size(), 1u);
    TEST_EQUAL(report_diff(a, b, delta),
        "A3+3=>B3+3;"
    );

}

void test_rs_tl_algorithm_edit_distance() {

    TEST_EQUAL(edit_distance(""s,       ""s),                  0);
    TEST_EQUAL(edit_distance("Hello"s,  ""s),                  5);
    TEST_EQUAL(edit_distance(""s,       "Hello"s),             5);
    TEST_EQUAL(edit_distance("Hello"s,  "Hello"s),             0);
    TEST_EQUAL(edit_distance("Hello"s,  "hello"s),             1);
    TEST_EQUAL(edit_distance("Hell"s,   "Hello"s),             1);
    TEST_EQUAL(edit_distance("Hello"s,  "Hell"s),              1);
    TEST_EQUAL(edit_distance("Hello"s,  "Hel"s),               2);
    TEST_EQUAL(edit_distance("Hello"s,  "He"s),                3);
    TEST_EQUAL(edit_distance("Hello"s,  "H"s),                 4);
    TEST_EQUAL(edit_distance("Hello"s,  "World"s),             4);
    TEST_EQUAL(edit_distance(""s,       ""s,        3, 2, 5),  0);
    TEST_EQUAL(edit_distance("Hello"s,  ""s,        3, 2, 5),  10);
    TEST_EQUAL(edit_distance(""s,       "Hello"s,   3, 2, 5),  15);
    TEST_EQUAL(edit_distance("Hello"s,  "Hello"s,   3, 2, 5),  0);
    TEST_EQUAL(edit_distance("Hell"s,   "Hello"s,   3, 2, 5),  3);
    TEST_EQUAL(edit_distance("Hello"s,  "Hell"s,    3, 2, 5),  2);
    TEST_EQUAL(edit_distance("Hello"s,  "Hel"s,     3, 2, 5),  4);
    TEST_EQUAL(edit_distance("Hello"s,  "He"s,      3, 2, 5),  6);
    TEST_EQUAL(edit_distance("Hello"s,  "H"s,       3, 2, 5),  8);
    TEST_EQUAL(edit_distance("Hello"s,  "hello"s,   3, 2, 1),  1);
    TEST_EQUAL(edit_distance("Hello"s,  "hello"s,   3, 2, 2),  2);
    TEST_EQUAL(edit_distance("Hello"s,  "hello"s,   3, 2, 3),  3);
    TEST_EQUAL(edit_distance("Hello"s,  "hello"s,   3, 2, 4),  4);
    TEST_EQUAL(edit_distance("Hello"s,  "hello"s,   3, 2, 5),  5);
    TEST_EQUAL(edit_distance("Hello"s,  "hello"s,   3, 2, 6),  5);
    TEST_EQUAL(edit_distance("Hello"s,  "World"s,   1, 1, 5),  8);
    TEST_EQUAL(edit_distance("Hello"s,  "World"s,   2, 2, 5),  16);
    TEST_EQUAL(edit_distance("Hello"s,  "World"s,   3, 3, 5),  20);
    TEST_EQUAL(edit_distance("Hello"s,  "World"s,   4, 4, 5),  20);
    TEST_EQUAL(edit_distance("Hello"s,  "World"s,   3, 2, 1),  4);
    TEST_EQUAL(edit_distance("Hello"s,  "World"s,   3, 2, 2),  8);
    TEST_EQUAL(edit_distance("Hello"s,  "World"s,   3, 2, 3),  12);
    TEST_EQUAL(edit_distance("Hello"s,  "World"s,   3, 2, 4),  16);
    TEST_EQUAL(edit_distance("Hello"s,  "World"s,   3, 2, 5),  20);
    TEST_EQUAL(edit_distance("Hello"s,  "World"s,   3, 2, 6),  20);

}
