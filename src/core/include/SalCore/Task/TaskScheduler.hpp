#pragma once

#include <SalCore/Task/Task.hpp>

namespace sal {
    class TaskScheduler {
    public:
        TaskScheduler() = default;

        TaskScheduler(const TaskScheduler &) = delete;
        TaskScheduler &operator=(const TaskScheduler &) = delete;

        ~TaskScheduler();

        void Spawn(Task task);

        template <typename F>
        void Spawn(F &&function) {
            Spawn(std::forward<F>(function)());
        }

        void Defer(Task task);

        template <typename F>
        void Defer(F &&function) {
            Defer(std::forward<F>(function)());
        }

        void Delay(double seconds, Task task);

        template <typename F>
        void Delay(double seconds, F &&function) {
            Delay(seconds, std::forward<F>(function)());
        }

        void Update(double deltaTime);

        void Shutdown();

        double GetTime() const;

        bool IsEmpty() const;

    private:
        double _time = 0.0;

        std::vector<Task::Handle> _tasks;
        std::vector<Task::Handle> _pendingTasks;
    };
}
