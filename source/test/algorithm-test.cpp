#include "rs-tl/algorithm.hpp"
#include "rs-unit-test.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <string>
#include <vector>

using namespace RS::TL;
using namespace std::literals;

void test_rs_tl_algorithm_container_algorithms() {

    const auto is_upper = [] (char c) { return c >= 'A' && c <= 'Z'; };
    const auto is_lower = [] (char c) { return c >= 'a' && c <= 'z'; };
    const auto is_alpha = [=] (char c) { return is_upper(c) || is_lower(c); };
    const auto same_case = [=] (char a, char b) { return (is_lower(a) && is_lower(b)) || (is_upper(a) && is_upper(b)); };

    std::string s;

    s = "";                 TRY(remove_in(s, 'c'));                        TEST_EQUAL(s, "");
    s = "xyzxyzxyz";        TRY(remove_in(s, 'c'));                        TEST_EQUAL(s, "xyzxyzxyz");
    s = "abcabcabc";        TRY(remove_in(s, 'c'));                        TEST_EQUAL(s, "ababab");
    s = "";                 TRY(remove_in_if(s, is_alpha));                TEST_EQUAL(s, "");
    s = "1234567890";       TRY(remove_in_if(s, is_alpha));                TEST_EQUAL(s, "1234567890");
    s = "abc123abc123";     TRY(remove_in_if(s, is_alpha));                TEST_EQUAL(s, "123123");
    s = "";                 TRY(remove_in_if_not(s, is_alpha));            TEST_EQUAL(s, "");
    s = "abcdefghijklm";    TRY(remove_in_if_not(s, is_alpha));            TEST_EQUAL(s, "abcdefghijklm");
    s = "abc123abc123";     TRY(remove_in_if_not(s, is_alpha));            TEST_EQUAL(s, "abcabc");
    s = "";                 TRY(unique_in(s));                             TEST_EQUAL(s, "");
    s = "abcdeabcde";       TRY(unique_in(s));                             TEST_EQUAL(s, "abcdeabcde");
    s = "abbcccddddeeeee";  TRY(unique_in(s));                             TEST_EQUAL(s, "abcde");
    s = "";                 TRY(unique_in(s, same_case));                  TEST_EQUAL(s, "");
    s = "AbCaBcAbCaBc";     TRY(unique_in(s, same_case));                  TEST_EQUAL(s, "AbCaBcAbCaBc");
    s = "ABCabcABCabc";     TRY(unique_in(s, same_case));                  TEST_EQUAL(s, "AaAa");
    s = "";                 TRY(sort_unique_in(s));                        TEST_EQUAL(s, "");
    s = "zyxwvutsrqpon";    TRY(sort_unique_in(s));                        TEST_EQUAL(s, "nopqrstuvwxyz");
    s = "abcdeabcdabcaba";  TRY(sort_unique_in(s));                        TEST_EQUAL(s, "abcde");
    s = "";                 TRY(sort_unique_in(s, std::greater<char>()));  TEST_EQUAL(s, "");
    s = "nopqrstuvwxyz";    TRY(sort_unique_in(s, std::greater<char>()));  TEST_EQUAL(s, "zyxwvutsrqpon");
    s = "abcdeabcdabcaba";  TRY(sort_unique_in(s, std::greater<char>()));  TEST_EQUAL(s, "edcba");

}

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

void test_rs_tl_algorithm_subsets() {

    std::string s = "abcde";
    std::vector<std::string> v;

    TRY(v = subsets(s));
    TEST_EQUAL(v.size(), 32u);
    REQUIRE(v.size() == 32u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "");
    TEST_EQUAL(v[1], "a");
    TEST_EQUAL(v[2], "ab");
    TEST_EQUAL(v[3], "abc");
    TEST_EQUAL(v[4], "abcd");
    TEST_EQUAL(v[5], "abcde");
    TEST_EQUAL(v[6], "abce");
    TEST_EQUAL(v[7], "abd");
    TEST_EQUAL(v[8], "abde");
    TEST_EQUAL(v[9], "abe");
    TEST_EQUAL(v[10], "ac");
    TEST_EQUAL(v[11], "acd");
    TEST_EQUAL(v[12], "acde");
    TEST_EQUAL(v[13], "ace");
    TEST_EQUAL(v[14], "ad");
    TEST_EQUAL(v[15], "ade");
    TEST_EQUAL(v[16], "ae");
    TEST_EQUAL(v[17], "b");
    TEST_EQUAL(v[18], "bc");
    TEST_EQUAL(v[19], "bcd");
    TEST_EQUAL(v[20], "bcde");
    TEST_EQUAL(v[21], "bce");
    TEST_EQUAL(v[22], "bd");
    TEST_EQUAL(v[23], "bde");
    TEST_EQUAL(v[24], "be");
    TEST_EQUAL(v[25], "c");
    TEST_EQUAL(v[26], "cd");
    TEST_EQUAL(v[27], "cde");
    TEST_EQUAL(v[28], "ce");
    TEST_EQUAL(v[29], "d");
    TEST_EQUAL(v[30], "de");
    TEST_EQUAL(v[31], "e");

    TRY(v = subsets(s, 0));
    TEST_EQUAL(v.size(), 1u);
    REQUIRE(v.size() == 1u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "");

    TRY(v = subsets(s, 1));
    TEST_EQUAL(v.size(), 5u);
    REQUIRE(v.size() == 5u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "a");
    TEST_EQUAL(v[1], "b");
    TEST_EQUAL(v[2], "c");
    TEST_EQUAL(v[3], "d");
    TEST_EQUAL(v[4], "e");

    TRY(v = subsets(s, 2));
    TEST_EQUAL(v.size(), 10u);
    REQUIRE(v.size() == 10u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "ab");
    TEST_EQUAL(v[1], "ac");
    TEST_EQUAL(v[2], "ad");
    TEST_EQUAL(v[3], "ae");
    TEST_EQUAL(v[4], "bc");
    TEST_EQUAL(v[5], "bd");
    TEST_EQUAL(v[6], "be");
    TEST_EQUAL(v[7], "cd");
    TEST_EQUAL(v[8], "ce");
    TEST_EQUAL(v[9], "de");

    TRY(v = subsets(s, 3));
    TEST_EQUAL(v.size(), 10u);
    REQUIRE(v.size() == 10u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "abc");
    TEST_EQUAL(v[1], "abd");
    TEST_EQUAL(v[2], "abe");
    TEST_EQUAL(v[3], "acd");
    TEST_EQUAL(v[4], "ace");
    TEST_EQUAL(v[5], "ade");
    TEST_EQUAL(v[6], "bcd");
    TEST_EQUAL(v[7], "bce");
    TEST_EQUAL(v[8], "bde");
    TEST_EQUAL(v[9], "cde");

    TRY(v = subsets(s, 4));
    TEST_EQUAL(v.size(), 5u);
    REQUIRE(v.size() == 5u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "abcd");
    TEST_EQUAL(v[1], "abce");
    TEST_EQUAL(v[2], "abde");
    TEST_EQUAL(v[3], "acde");
    TEST_EQUAL(v[4], "bcde");

    TRY(v = subsets(s, 5));
    TEST_EQUAL(v.size(), 1u);
    REQUIRE(v.size() == 1u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "abcde");

}
