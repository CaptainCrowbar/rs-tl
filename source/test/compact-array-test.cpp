#include "rs-tl/compact-array.hpp"
#include "rs-unit-test.hpp"
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace RS::TL;
using namespace RS::UnitTest;

namespace {

    std::string repeat_string(const std::string& str, int n, const std::string& delimiter) {
        if (n == 0)
            return {};
        else if (n == 1)
            return str;
        std::string sd = str + delimiter;
        std::string result;
        for (int i = 0; i < n; ++i)
            result += sd;
        result.resize(result.size() - delimiter.size());
        return result;
    }

}

void test_rs_tl_compact_array_construction() {

    using ICA = CompactArray<int, 5>;

    ICA a;
    std::string s;

    TEST(a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 0u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_THROW(a.at(0), std::out_of_range);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[]");

    ICA b(5, 42);

    TEST(! b.empty());
    TEST(b.is_compact());
    TEST_EQUAL(b.size(), 5u);
    TEST_EQUAL(b.capacity(), 5u);
    TEST_EQUAL(b[0], 42);
    TEST_EQUAL(b[4], 42);
    TEST_EQUAL(b.at(0), 42);
    TEST_EQUAL(b.at(4), 42);
    TEST_THROW(b.at(5), std::out_of_range);
    TRY(s = format_range(b));
    TEST_EQUAL(s, "[42,42,42,42,42]");

    TRY(a = b);
    TEST(! a.empty());
    TEST(! b.empty());
    TEST(a.is_compact());
    TEST(b.is_compact());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(b.size(), 5u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_EQUAL(b.capacity(), 5u);
    TEST_EQUAL(a[0], 42);
    TEST_EQUAL(a[4], 42);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42]");

    TRY(a = std::move(b));
    TEST(! a.empty());
    TEST(b.empty());
    TEST(a.is_compact());
    TEST(b.is_compact());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(b.size(), 0u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_EQUAL(b.capacity(), 5u);
    TEST_EQUAL(a[0], 42);
    TEST_EQUAL(a[4], 42);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42]");

    ICA c(10, 42);

    TEST(! c.empty());
    TEST(! c.is_compact());
    TEST_EQUAL(c.size(), 10u);
    TEST_EQUAL(c.capacity(), 16u);
    TEST_EQUAL(c[0], 42);
    TEST_EQUAL(c[9], 42);
    TRY(s = format_range(c));
    TEST_EQUAL(s, "[42,42,42,42,42,42,42,42,42,42]");

    TRY(a = c);
    TEST(! a.empty());
    TEST(! c.empty());
    TEST(! a.is_compact());
    TEST(! c.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(c.size(), 10u);
    TEST_EQUAL(a.capacity(), 16u);
    TEST_EQUAL(c.capacity(), 16u);
    TEST_EQUAL(a[0], 42);
    TEST_EQUAL(a[4], 42);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42,42,42,42,42,42]");

    TRY(a = std::move(c));
    TEST(! a.empty());
    TEST(c.empty());
    TEST(! a.is_compact());
    TEST(c.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(c.size(), 0u);
    TEST_EQUAL(a.capacity(), 16u);
    TEST_EQUAL(c.capacity(), 5u);
    TEST_EQUAL(a[0], 42);
    TEST_EQUAL(a[4], 42);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42,42,42,42,42,42]");

    TRY((a = ICA{1,2,3,4,5}));
    TEST(! a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_EQUAL(a[0], 1);
    TEST_EQUAL(a[4], 5);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[1,2,3,4,5]");
    TEST_EQUAL(a.front(), 1);
    TEST_EQUAL(a.back(), 5);

    TRY((a = ICA{1,2,3,4,5,6,7,8,9,10}));
    TEST(! a.empty());
    TEST(! a.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(a.capacity(), 16u);
    TEST_EQUAL(a[0], 1);
    TEST_EQUAL(a[9], 10);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[1,2,3,4,5,6,7,8,9,10]");

    TRY((a = {1,2,3,4,5}));
    TEST(! a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_EQUAL(a[0], 1);
    TEST_EQUAL(a[4], 5);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[1,2,3,4,5]");

    TRY((a = {1,2,3,4,5,6,7,8,9,10}));
    TEST(! a.empty());
    TEST(! a.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(a.capacity(), 16u);
    TEST_EQUAL(a[0], 1);
    TEST_EQUAL(a[9], 10);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[1,2,3,4,5,6,7,8,9,10]");

    std::vector<int> v = {1,2,3,4,5};
    TRY(a = ICA(v.begin(), v.end()));
    TEST(! a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_EQUAL(a[0], 1);
    TEST_EQUAL(a[4], 5);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[1,2,3,4,5]");

    v = {1,2,3,4,5,6,7,8,9,10};
    TRY(a = ICA(v.begin(), v.end()));
    TEST(! a.empty());
    TEST(! a.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(a.capacity(), 16u);
    TEST_EQUAL(a[0], 1);
    TEST_EQUAL(a[9], 10);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[1,2,3,4,5,6,7,8,9,10]");

    TRY(a.clear());
    TEST(a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 0u);
    TEST_EQUAL(a.capacity(), 5u);

}

void test_rs_tl_compact_array_capacity() {

    using ICA = CompactArray<int, 5>;

    ICA a;
    std::string s;

    TRY(a.resize(5, 42));
    TEST(! a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_EQUAL(a[0], 42);
    TEST_EQUAL(a[4], 42);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42]");

    TRY(a.resize(10));
    TEST(! a.empty());
    TEST(! a.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(a.capacity(), 16u);
    TEST_EQUAL(a[0], 42);
    TEST_EQUAL(a[9], 0);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42,0,0,0,0,0]");

    TRY(a.shrink_to_fit());
    TEST(! a.empty());
    TEST(! a.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(a.capacity(), 10u);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42,0,0,0,0,0]");

    TRY(a.resize(4));
    TEST(! a.empty());
    TEST(! a.is_compact());
    TEST_EQUAL(a.size(), 4u);
    TEST_EQUAL(a.capacity(), 10u);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42]");

    TRY(a.shrink_to_fit());
    TEST(! a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 4u);
    TEST_EQUAL(a.capacity(), 5u);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42]");

    TRY(a.clear());
    TEST(a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 0u);
    TEST_EQUAL(a.capacity(), 5u);

}

void test_rs_tl_compact_array_insertion() {

    using ICA = CompactArray<int, 5>;
    using SCA = CompactArray<std::string, 5>;

    ICA a;
    SCA b;
    ICA::iterator ai;
    SCA::iterator bi;
    std::string s;
    std::vector<int> v;

    for (size_t i = 1; i <= 16; ++i) {
        TRY(a.push_back(42));
        TEST_EQUAL(a.size(), i);
        TEST_EQUAL(a.capacity(), i <= 5 ? 5u : i <= 8 ? 8u : 16u);
        TEST_EQUAL(a[i - 1], 42);
        TRY(s = format_range(a));
        TEST_EQUAL(s, "[" + repeat_string("42", i, ",") + "]");
    }

    for (size_t i = 1; i <= 16; ++i) {
        size_t n = 16 - i;
        TRY(a.pop_back());
        TEST_EQUAL(a.size(), n);
        TEST_EQUAL(a.capacity(), 16u);
        if (n)
            TEST_EQUAL(a[n - 1], 42);
        TRY(s = format_range(a));
        TEST_EQUAL(s, "[" + repeat_string("42", n, ",") + "]");
    }

    TEST(a.empty());
    TEST_EQUAL(a.size(), 0u);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[]");

    for (size_t i = 1; i <= 16; ++i) {
        TRY(b.emplace_back(3, 'a'));
        TEST_EQUAL(b.size(), i);
        TEST_EQUAL(b.capacity(), i <= 5 ? 5u : i <= 8 ? 8u : 16u);
        TEST_EQUAL(b[i - 1], "aaa");
        TRY(s = format_range(b));
        TEST_EQUAL(s, "[" + repeat_string("aaa", i, ",") + "]");
    }

    for (size_t i = 1; i <= 16; ++i) {
        size_t n = 16 - i;
        TRY(b.pop_back());
        TEST_EQUAL(b.size(), n);
        TEST_EQUAL(b.capacity(), 16u);
        if (n)
            TEST_EQUAL(b[n - 1], "aaa");
        TRY(s = format_range(b));
        TEST_EQUAL(s, "[" + repeat_string("aaa", n, ",") + "]");
    }

    TEST(b.empty());
    TEST_EQUAL(b.size(), 0u);
    TRY(s = format_range(b));
    TEST_EQUAL(s, "[]");

    TRY((a = {1,2,3}));
    TEST_EQUAL(a.size(), 3u);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[1,2,3]");

    TRY(ai = a.insert(a.begin() + 3, 10));  TEST_EQUAL(a.size(), 4u);   TEST_EQUAL(ai - a.begin(), 3);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,10]");
    TRY(ai = a.insert(a.begin() + 3, 20));  TEST_EQUAL(a.size(), 5u);   TEST_EQUAL(ai - a.begin(), 3);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,20,10]");
    TRY(ai = a.insert(a.begin() + 3, 30));  TEST_EQUAL(a.size(), 6u);   TEST_EQUAL(ai - a.begin(), 3);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,30,20,10]");
    TRY(ai = a.insert(a.begin() + 3, 40));  TEST_EQUAL(a.size(), 7u);   TEST_EQUAL(ai - a.begin(), 3);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,40,30,20,10]");
    TRY(ai = a.insert(a.begin() + 3, 50));  TEST_EQUAL(a.size(), 8u);   TEST_EQUAL(ai - a.begin(), 3);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,50,40,30,20,10]");
    TRY(ai = a.insert(a.begin() + 3, 60));  TEST_EQUAL(a.size(), 9u);   TEST_EQUAL(ai - a.begin(), 3);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,60,50,40,30,20,10]");
    TRY(ai = a.insert(a.begin() + 3, 70));  TEST_EQUAL(a.size(), 10u);  TEST_EQUAL(ai - a.begin(), 3);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,70,60,50,40,30,20,10]");
    TRY(ai = a.insert(a.begin() + 3, 80));  TEST_EQUAL(a.size(), 11u);  TEST_EQUAL(ai - a.begin(), 3);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,80,70,60,50,40,30,20,10]");
    TRY(ai = a.insert(a.begin() + 3, 90));  TEST_EQUAL(a.size(), 12u);  TEST_EQUAL(ai - a.begin(), 3);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,90,80,70,60,50,40,30,20,10]");

    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 11u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,80,70,60,50,40,30,20,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 10u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,70,60,50,40,30,20,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 9u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,60,50,40,30,20,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 8u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,50,40,30,20,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 7u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,40,30,20,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 6u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,30,20,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 5u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,20,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 4u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 3u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3]");

    TRY((b = {"x","y","z"}));
    TEST_EQUAL(b.size(), 3u);
    TRY(s = format_range(b));
    TEST_EQUAL(s, "[x,y,z]");

    TRY(bi = b.emplace(b.begin() + 3, 3, 'a'));  TEST_EQUAL(b.size(), 4u);   TEST_EQUAL(bi - b.begin(), 3);  TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'b'));  TEST_EQUAL(b.size(), 5u);   TEST_EQUAL(bi - b.begin(), 3);  TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,bbb,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'c'));  TEST_EQUAL(b.size(), 6u);   TEST_EQUAL(bi - b.begin(), 3);  TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,ccc,bbb,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'd'));  TEST_EQUAL(b.size(), 7u);   TEST_EQUAL(bi - b.begin(), 3);  TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,ddd,ccc,bbb,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'e'));  TEST_EQUAL(b.size(), 8u);   TEST_EQUAL(bi - b.begin(), 3);  TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,eee,ddd,ccc,bbb,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'f'));  TEST_EQUAL(b.size(), 9u);   TEST_EQUAL(bi - b.begin(), 3);  TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,fff,eee,ddd,ccc,bbb,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'g'));  TEST_EQUAL(b.size(), 10u);  TEST_EQUAL(bi - b.begin(), 3);  TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,ggg,fff,eee,ddd,ccc,bbb,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'h'));  TEST_EQUAL(b.size(), 11u);  TEST_EQUAL(bi - b.begin(), 3);  TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,hhh,ggg,fff,eee,ddd,ccc,bbb,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'i'));  TEST_EQUAL(b.size(), 12u);  TEST_EQUAL(bi - b.begin(), 3);  TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,iii,hhh,ggg,fff,eee,ddd,ccc,bbb,aaa]");

    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 11u);  TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,hhh,ggg,fff,eee,ddd,ccc,bbb,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 10u);  TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,ggg,fff,eee,ddd,ccc,bbb,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 9u);   TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,fff,eee,ddd,ccc,bbb,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 8u);   TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,eee,ddd,ccc,bbb,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 7u);   TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,ddd,ccc,bbb,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 6u);   TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,ccc,bbb,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 5u);   TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,bbb,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 4u);   TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 3u);   TRY(s = format_range(b));  TEST_EQUAL(s, "[x,y,z]");

    TRY(a.clear());
    TEST(a.empty());

    v = {1,2,3,4};      TRY(a.insert(a.begin(), v.begin(), v.end()));      TEST_EQUAL(a.size(), 4u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4]");
    v = {5,6,7,8};      TRY(a.insert(a.begin() + 2, v.begin(), v.end()));  TEST_EQUAL(a.size(), 8u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,5,6,7,8,3,4]");
    v = {9,10,11,12};   TRY(a.insert(a.begin() + 4, v.begin(), v.end()));  TEST_EQUAL(a.size(), 12u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,5,6,9,10,11,12,7,8,3,4]");
    v = {13,14,15,16};  TRY(a.insert(a.begin() + 6, v.begin(), v.end()));  TEST_EQUAL(a.size(), 16u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,5,6,9,10,13,14,15,16,11,12,7,8,3,4]");

    TRY(a.erase(a.begin() + 6, a.begin() + 10));  TEST_EQUAL(a.size(), 12u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,5,6,9,10,11,12,7,8,3,4]");
    TRY(a.erase(a.begin() + 4, a.begin() + 8));   TEST_EQUAL(a.size(), 8u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,5,6,7,8,3,4]");
    TRY(a.erase(a.begin() + 2, a.begin() + 6));   TEST_EQUAL(a.size(), 4u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4]");
    TRY(a.erase(a.begin(), a.begin() + 4));       TEST_EQUAL(a.size(), 0u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[]");

    TEST(a.empty());

    v = {1,2,3,4};      TRY(a.insert(a.begin(), v.begin(), v.end()));  TEST_EQUAL(a.size(), 4u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4]");
    v = {5,6,7,8};      TRY(a.insert(a.begin(), v.begin(), v.end()));  TEST_EQUAL(a.size(), 8u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[5,6,7,8,1,2,3,4]");
    v = {9,10,11,12};   TRY(a.insert(a.begin(), v.begin(), v.end()));  TEST_EQUAL(a.size(), 12u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[9,10,11,12,5,6,7,8,1,2,3,4]");
    v = {13,14,15,16};  TRY(a.insert(a.begin(), v.begin(), v.end()));  TEST_EQUAL(a.size(), 16u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[13,14,15,16,9,10,11,12,5,6,7,8,1,2,3,4]");

    TRY(a.erase(a.begin(), a.begin() + 4));  TEST_EQUAL(a.size(), 12u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[9,10,11,12,5,6,7,8,1,2,3,4]");
    TRY(a.erase(a.begin(), a.begin() + 4));  TEST_EQUAL(a.size(), 8u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[5,6,7,8,1,2,3,4]");
    TRY(a.erase(a.begin(), a.begin() + 4));  TEST_EQUAL(a.size(), 4u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4]");
    TRY(a.erase(a.begin(), a.begin() + 4));  TEST_EQUAL(a.size(), 0u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[]");

    TEST(a.empty());

    v = {1,2,3,4};      TRY(a.insert(a.end(), v.begin(), v.end()));  TEST_EQUAL(a.size(), 4u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4]");
    v = {5,6,7,8};      TRY(a.insert(a.end(), v.begin(), v.end()));  TEST_EQUAL(a.size(), 8u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4,5,6,7,8]");
    v = {9,10,11,12};   TRY(a.insert(a.end(), v.begin(), v.end()));  TEST_EQUAL(a.size(), 12u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4,5,6,7,8,9,10,11,12]");
    v = {13,14,15,16};  TRY(a.insert(a.end(), v.begin(), v.end()));  TEST_EQUAL(a.size(), 16u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]");

    TRY(a.erase(a.end() - 4, a.end()));  TEST_EQUAL(a.size(), 12u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4,5,6,7,8,9,10,11,12]");
    TRY(a.erase(a.end() - 4, a.end()));  TEST_EQUAL(a.size(), 8u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4,5,6,7,8]");
    TRY(a.erase(a.end() - 4, a.end()));  TEST_EQUAL(a.size(), 4u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4]");
    TRY(a.erase(a.end() - 4, a.end()));  TEST_EQUAL(a.size(), 0u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[]");

    TEST(a.empty());

    v = {1,2,3,4};      TRY(a.append(v));  TEST_EQUAL(a.size(), 4u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4]");
    v = {5,6,7,8};      TRY(a.append(v));  TEST_EQUAL(a.size(), 8u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4,5,6,7,8]");
    v = {9,10,11,12};   TRY(a.append(v));  TEST_EQUAL(a.size(), 12u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4,5,6,7,8,9,10,11,12]");
    v = {13,14,15,16};  TRY(a.append(v));  TEST_EQUAL(a.size(), 16u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]");

    TRY(a.clear());
    TEST(a.empty());

    v = {1,2,3,4};      TRY(a.append(std::move(v)));  TEST_EQUAL(a.size(), 4u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4]");
    v = {5,6,7,8};      TRY(a.append(std::move(v)));  TEST_EQUAL(a.size(), 8u);   TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4,5,6,7,8]");
    v = {9,10,11,12};   TRY(a.append(std::move(v)));  TEST_EQUAL(a.size(), 12u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4,5,6,7,8,9,10,11,12]");
    v = {13,14,15,16};  TRY(a.append(std::move(v)));  TEST_EQUAL(a.size(), 16u);  TRY(s = format_range(a));  TEST_EQUAL(s, "[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]");

}

void test_rs_tl_compact_array_accounting() {

    using CI = Counted<int>;
    using CA = CompactArray<CI, 5>;

    const std::vector<int> v5 = {1,2,3,4,5};
    const std::vector<int> v10 = {1,2,3,4,5,6,7,8,9,10};

    CA a;                                                TEST_EQUAL(CI::count(), 0);
    CA b(5, 42);                                         TEST_EQUAL(CI::count(), 5);
    TRY(a = b);                                          TEST_EQUAL(CI::count(), 10);
    TRY(a = std::move(b));                               TEST_EQUAL(CI::count(), 5);
    CA c(10, 42);                                        TEST_EQUAL(CI::count(), 15);
    TRY(a = c);                                          TEST_EQUAL(CI::count(), 20);
    TRY(a = std::move(c));                               TEST_EQUAL(CI::count(), 10);
    TRY((a = CA{1,2,3,4,5}));                            TEST_EQUAL(CI::count(), 5);
    TRY((a = CA{1,2,3,4,5,6,7,8,9,10}));                 TEST_EQUAL(CI::count(), 10);
    TRY((a = {1,2,3,4,5}));                              TEST_EQUAL(CI::count(), 5);
    TRY((a = {1,2,3,4,5,6,7,8,9,10}));                   TEST_EQUAL(CI::count(), 10);
    TRY(a = CA(v5.begin(), v5.end()));                   TEST_EQUAL(CI::count(), 5);
    TRY(a = CA(v10.begin(), v10.end()));                 TEST_EQUAL(CI::count(), 10);
    TRY(a.clear());                                      TEST_EQUAL(CI::count(), 0);
    TRY(a.resize(5, 42));                                TEST_EQUAL(CI::count(), 5);
    TRY(a.resize(10));                                   TEST_EQUAL(CI::count(), 10);
    TRY(a.shrink_to_fit());                              TEST_EQUAL(CI::count(), 10);
    TRY(a.resize(4));                                    TEST_EQUAL(CI::count(), 4);
    TRY(a.shrink_to_fit());                              TEST_EQUAL(CI::count(), 4);
    TRY(a.clear());                                      TEST_EQUAL(CI::count(), 0);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 1);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 2);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 3);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 4);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 5);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 6);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 7);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 8);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 9);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 10);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 9);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 8);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 7);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 6);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 5);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 4);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 3);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 2);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 1);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 0);
    TRY((a = {1,2,3}));                                  TEST_EQUAL(CI::count(), 3);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 4);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 5);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 6);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 7);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 8);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 9);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 10);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 9);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 8);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 7);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 6);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 5);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 4);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 3);
    TRY(a.clear());                                      TEST_EQUAL(CI::count(), 0);
    TRY(a.insert(a.begin(), v5.begin(), v5.end()));      TEST_EQUAL(CI::count(), 5);
    TRY(a.insert(a.begin(), v5.begin(), v5.end()));      TEST_EQUAL(CI::count(), 10);
    TRY(a.insert(a.begin(), v5.begin(), v5.end()));      TEST_EQUAL(CI::count(), 15);
    TRY(a.insert(a.begin(), v5.begin(), v5.end()));      TEST_EQUAL(CI::count(), 20);
    TRY(a.erase(a.begin(), a.begin() + 5));              TEST_EQUAL(CI::count(), 15);
    TRY(a.erase(a.begin(), a.begin() + 5));              TEST_EQUAL(CI::count(), 10);
    TRY(a.erase(a.begin(), a.begin() + 5));              TEST_EQUAL(CI::count(), 5);
    TRY(a.erase(a.begin(), a.begin() + 5));              TEST_EQUAL(CI::count(), 0);
    TRY(a.insert(a.begin(), v5.begin(), v5.end()));      TEST_EQUAL(CI::count(), 5);
    TRY(a.insert(a.begin() + 1, v5.begin(), v5.end()));  TEST_EQUAL(CI::count(), 10);
    TRY(a.insert(a.begin() + 2, v5.begin(), v5.end()));  TEST_EQUAL(CI::count(), 15);
    TRY(a.insert(a.begin() + 3, v5.begin(), v5.end()));  TEST_EQUAL(CI::count(), 20);
    TRY(a.erase(a.begin() + 3, a.begin() + 8));          TEST_EQUAL(CI::count(), 15);
    TRY(a.erase(a.begin() + 2, a.begin() + 7));          TEST_EQUAL(CI::count(), 10);
    TRY(a.erase(a.begin() + 1, a.begin() + 6));          TEST_EQUAL(CI::count(), 5);
    TRY(a.erase(a.begin(), a.begin() + 5));              TEST_EQUAL(CI::count(), 0);
    TRY(a.insert(a.end(), v5.begin(), v5.end()));        TEST_EQUAL(CI::count(), 5);
    TRY(a.insert(a.end(), v5.begin(), v5.end()));        TEST_EQUAL(CI::count(), 10);
    TRY(a.insert(a.end(), v5.begin(), v5.end()));        TEST_EQUAL(CI::count(), 15);
    TRY(a.insert(a.end(), v5.begin(), v5.end()));        TEST_EQUAL(CI::count(), 20);
    TRY(a.erase(a.end() - 5, a.end()));                  TEST_EQUAL(CI::count(), 15);
    TRY(a.erase(a.end() - 5, a.end()));                  TEST_EQUAL(CI::count(), 10);
    TRY(a.erase(a.end() - 5, a.end()));                  TEST_EQUAL(CI::count(), 5);
    TRY(a.erase(a.end() - 5, a.end()));                  TEST_EQUAL(CI::count(), 0);
    TRY(a.append(v5));                                   TEST_EQUAL(CI::count(), 5);
    TRY(a.append(v5));                                   TEST_EQUAL(CI::count(), 10);
    TRY(a.append(v5));                                   TEST_EQUAL(CI::count(), 15);
    TRY(a.append(v5));                                   TEST_EQUAL(CI::count(), 20);
    TRY(a.append(std::move(v5)));                        TEST_EQUAL(CI::count(), 25);

}

void test_rs_tl_compact_array_keys() {

    using ICA = CompactArray<int, 5>;
    using Omap = std::map<ICA, std::string>;
    using Umap = std::unordered_map<ICA, std::string>;

    ICA a;
    Omap omap;
    Umap umap;
    std::string s;

    for (int i = 5; i >= 1; --i) {
        TRY(a.clear());
        for (int j = i; j <= 2 * i; ++j)
            TRY(a.push_back(j));
        TEST_EQUAL(a.size(), size_t(i + 1));
        s.assign(i, char('@' + i));
        TRY(omap[a] = s);
        TRY(umap[a] = s);
    }

    TEST_EQUAL(omap.size(), 5u);
    TEST_EQUAL(umap.size(), 5u);

    auto it = omap.begin();

    TEST_EQUAL(format_range(it->first), "[1,2]");           TEST_EQUAL(it->second, "A");      ++it;
    TEST_EQUAL(format_range(it->first), "[2,3,4]");         TEST_EQUAL(it->second, "BB");     ++it;
    TEST_EQUAL(format_range(it->first), "[3,4,5,6]");       TEST_EQUAL(it->second, "CCC");    ++it;
    TEST_EQUAL(format_range(it->first), "[4,5,6,7,8]");     TEST_EQUAL(it->second, "DDDD");   ++it;
    TEST_EQUAL(format_range(it->first), "[5,6,7,8,9,10]");  TEST_EQUAL(it->second, "EEEEE");  ++it;

    TEST(it == omap.end());

}
