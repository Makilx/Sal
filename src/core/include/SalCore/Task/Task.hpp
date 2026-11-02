#pragma once

#include <SalCore/Precompiled.hpp>

namespace sal {
    class TaskScheduler;

    class Task {
    public:
        struct promise_type;

    private:
        using Handle = std::coroutine_handle<promise_type>;

        class WaitAwaitable {
        public:
            explicit WaitAwaitable(double seconds);

            bool await_ready() const noexcept;
            void await_suspend(Handle handle) const noexcept;
            void await_resume() const noexcept;

        private:
            double _seconds;
        };

        explicit Task(Handle handle);

        friend class TaskScheduler;

    private:
        Handle _handle = nullptr;

    public:
        Task() = default;

        Task(const Task &) = delete;
        Task &operator=(const Task &) = delete;

        Task(Task &&other) noexcept;
        Task &operator=(Task &&other) noexcept;

        ~Task();

        static WaitAwaitable Wait(double seconds);
    };

    struct Task::promise_type {
        TaskScheduler *scheduler = nullptr;
        double wakeTime = 0.0;
        std::exception_ptr exception;

        Task get_return_object() noexcept;

        std::suspend_always initial_suspend() noexcept;
        std::suspend_always final_suspend() noexcept;

        void return_void() noexcept;
        void unhandled_exception() noexcept;
    };
}
