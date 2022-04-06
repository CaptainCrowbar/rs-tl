# Timing

_[Template Library by Ross Smith](index.html)_

```c++
#include "rs-tl/time.hpp"
namespace RS::TL;
```

## Timed-wait base class

```c++
class Waiter {
public:
    using Waiter::clock = std::chrono::system_clock;
    using Waiter::duration = clock::duration;
    using Waiter::time_point = clock::time_point;
    virtual Waiter::~Waiter() noexcept = 0;
    virtual bool Waiter::poll();
    virtual void Waiter::wait();
    virtual bool Waiter::wait_until(time_point t);
    template <typename R, typename P>
        bool Waiter::wait_for(std::chrono::duration<R, P> t);
protected:
    virtual bool Waiter::do_wait_for(duration t);
};
```

A general purpose base class for waitable objects. The `poll()` function
returns true if whatever condition was being waited on is ready, false if it
is not. The `wait_for()` and `wait_until()` functions return true on success,
false on timeout. The `wait()` function will wait indefinitely.

A derived class must implement at least one of `do_wait_for()` or
`wait_until()`, whichever is more convenient for the particular class; the
default implementations call each other. The default implementations of
`poll()` and `wait()` call `so_wait_for()`; inheritors may optionally also
implement either or both of `poll()` or `wait()`, if a more efficient
implementation is possible.
