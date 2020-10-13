#ifndef _FIBERIO_SRC_SCHEDULER_H_
#define _FIBERIO_SRC_SCHEDULER_H_

#include <uv.h>
#include <boost/fiber/all.hpp>
#include <chrono>
#include <deque>
#include <mutex>

namespace fiberio {

class scheduler : public boost::fibers::algo::algorithm
{
public:
    scheduler();

    ~scheduler();

    void awakened(boost::fibers::context* fiber) noexcept;

    boost::fibers::context* pick_next() noexcept;

    bool has_ready_fibers() const noexcept;

    void suspend_until(
        std::chrono::steady_clock::time_point const& abs_time) noexcept;

    void notify() noexcept;

    // Non-copyable and non-movable
    scheduler(const scheduler&) = delete;
    scheduler& operator=(const scheduler&) = delete;
    scheduler(scheduler&&) = delete;
    scheduler& operator=(scheduler&&) = delete;

private:
    bool should_wake_up();
    bool suspend_enter();
    void suspend_exit();
    bool update_notify();

    std::deque<boost::fibers::context*> queue_;
    uv_loop_t* loop_;
    uv_timer_t* timer_;
    uv_async_t* async_;
    bool wake_up_;
    bool suspended_;
    std::mutex mutex_;
};

}

#endif
