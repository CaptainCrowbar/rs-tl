#pragma once

#include "rs-tl/types.hpp"
#include <chrono>

namespace RS::TL {

    class Waiter {
    public:
        using clock = std::chrono::system_clock;
        using duration = clock::duration;
        using time_point = clock::time_point;
        virtual ~Waiter() noexcept = 0;
        virtual bool poll() { return do_wait_for({}); }
        virtual void wait() { while (! wait_for(std::chrono::seconds(1))) {} }
        virtual bool wait_until(time_point t) { return do_wait_for(t - clock::now()); }
        template <typename R, typename P> bool wait_for(std::chrono::duration<R, P> t);
    protected:
        virtual bool do_wait_for(duration t) { return wait_until(clock::now() + t); }
    };

        inline Waiter::~Waiter() noexcept {}

        template <typename R, typename P>
        bool Waiter::wait_for(std::chrono::duration<R, P> t) {
            using namespace std::chrono;
            auto dt = duration_cast<duration>(t);
            return do_wait_for(dt);
        }

}
