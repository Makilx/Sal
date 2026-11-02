#include <SalCore/Task/Task.hpp>

namespace sal {
    Task::WaitAwaitable::WaitAwaitable(double seconds)
        : _seconds(seconds) {}

    bool Task::WaitAwaitable::await_ready() const noexcept {
        return _seconds <= 0.0;
    }

    void Task::WaitAwaitable::await_resume() const noexcept {}

    Task::Task(Handle handle)
        : _handle(handle) {}

    Task::Task(Task &&other) noexcept
        : _handle(std::exchange(other._handle, nullptr)) {}

    Task &Task::operator=(Task &&other) noexcept {
        if (this != &other) {
            if (_handle) {
                _handle.destroy();
            }

            _handle = std::exchange(other._handle, nullptr);
        }

        return *this;
    }

    Task::~Task() {
        if (_handle) {
            _handle.destroy();
        }
    }

    Task::WaitAwaitable Task::Wait(double seconds) {
        return WaitAwaitable(seconds);
    }

    Task Task::promise_type::get_return_object() noexcept {
        return Task{Handle::from_promise(*this)};
    }

    std::suspend_always Task::promise_type::initial_suspend() noexcept {
        return {};
    }

    std::suspend_always Task::promise_type::final_suspend() noexcept {
        return {};
    }

    void Task::promise_type::return_void() noexcept {}

    void Task::promise_type::unhandled_exception() noexcept {
        exception = std::current_exception();
    }
}
