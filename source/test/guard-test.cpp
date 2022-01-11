#include "rs-tl/guard.hpp"
#include "rs-unit-test.hpp"
#include <stdexcept>

using namespace RS::TL;

void test_rs_tl_scope_guards() {

    int n = 0;
    {
        n = 1;
        auto guard = on_scope_exit([&] { n = 2; });
        TEST_EQUAL(n, 1);
    }
    TEST_EQUAL(n, 2);

    n = 0;
    try {
        n = 1;
        auto guard = on_scope_exit([&] { n = 2; });
        TEST_EQUAL(n, 1);
        throw std::runtime_error("fail");
    }
    catch (...) {}
    TEST_EQUAL(n, 2);

    n = 0;
    {
        n = 1;
        auto guard = on_scope_success([&] { n = 2; });
        TEST_EQUAL(n, 1);
    }
    TEST_EQUAL(n, 2);

    n = 0;
    try {
        n = 1;
        auto guard = on_scope_success([&] { n = 2; });
        TEST_EQUAL(n, 1);
        throw std::runtime_error("fail");
    }
    catch (...) {}
    TEST_EQUAL(n, 1);

    n = 0;
    {
        n = 1;
        auto guard = on_scope_fail([&] { n = 2; });
        TEST_EQUAL(n, 1);
    }
    TEST_EQUAL(n, 1);

    n = 0;
    try {
        n = 1;
        auto guard = on_scope_fail([&] { n = 2; });
        TEST_EQUAL(n, 1);
        throw std::runtime_error("fail");
    }
    catch (...) {}
    TEST_EQUAL(n, 2);

}
