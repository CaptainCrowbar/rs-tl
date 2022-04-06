#include "rs-tl/time.hpp"
#include "rs-unit-test.hpp"
#include <chrono>
#include <thread>

using namespace RS::TL;
using namespace std::chrono;
using namespace std::literals;

namespace {

    class Thing:
    public Waiter {
    protected:
        bool do_wait_for(duration t) override {
            std::this_thread::sleep_for(t);
            return false;
        }
    };

}

void test_rs_tl_time_waiter() {

    Thing t;

    TEST(! t.wait_for(50ms));

}
