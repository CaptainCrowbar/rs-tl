#include "rs-tl/iterator.hpp"
#include "rs-unit-test.hpp"
#include <algorithm>
#include <functional>
#include <set>
#include <vector>

using namespace RS::TL;
using namespace RS::UnitTest;

void test_rs_tl_iterator_container_algorithms() {

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
