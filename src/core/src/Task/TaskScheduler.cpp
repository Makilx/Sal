#include <SalCore/Task/TaskScheduler.hpp>

namespace sal {
    TaskScheduler::~TaskScheduler() {
        Shutdown();
    }

    void TaskScheduler::Spawn(Task task) {
        if (!task._handle) {
            return;
        }

        Task::Handle handle = std::exchange(task._handle, nullptr);

        handle.promise().scheduler = this;
        handle.promise().wakeTime = _time;

        _pendingTasks.push_back(handle);
    }

    void TaskScheduler::Defer(Task task) {
        if (!task._handle) {
            return;
        }

        Task::Handle handle = std::exchange(task._handle, nullptr);

        handle.promise().scheduler = this;
        handle.promise().wakeTime = _time;

        _pendingTasks.push_back(handle);
    }

    void TaskScheduler::Delay(double seconds, Task task) {
        if (!task._handle) {
            return;
        }

        Task::Handle handle = std::exchange(task._handle, nullptr);

        handle.promise().scheduler = this;
        handle.promise().wakeTime = _time + std::max(seconds, 0.0);

        _pendingTasks.push_back(handle);
    }

    void TaskScheduler::Update(double deltaTime) {
        _time += std::max(deltaTime, 0.0);

        std::vector<Task::Handle> pendingTasks;
        pendingTasks.swap(_pendingTasks);

        _tasks.insert(
            _tasks.end(),
            pendingTasks.begin(),
            pendingTasks.end());

        for (std::size_t i = 0; i < _tasks.size();) {
            Task::Handle handle = _tasks[i];

            if (handle.promise().wakeTime > _time) {
                ++i;
                continue;
            }

            handle.resume();

            if (handle.done()) {
                std::exception_ptr exception = handle.promise().exception;

                handle.destroy();

                _tasks.erase(_tasks.begin() + i);

                if (exception) {
                    std::rethrow_exception(exception);
                }

                continue;
            }

            ++i;
        }
    }

    void TaskScheduler::Shutdown() {
        for (Task::Handle handle : _tasks) {
            handle.destroy();
        }

        for (Task::Handle handle : _pendingTasks) {
            handle.destroy();
        }

        _tasks.clear();
        _pendingTasks.clear();
    }

    double TaskScheduler::GetTime() const {
        return _time;
    }

    bool TaskScheduler::IsEmpty() const {
        return _tasks.empty() && _pendingTasks.empty();
    }

    void Task::WaitAwaitable::await_suspend(Handle handle) const noexcept {
        auto &promise = handle.promise();

        promise.wakeTime = promise.scheduler->GetTime() + _seconds;
    }
}
