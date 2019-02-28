#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include "rx/internal/thread/thread_manager.h"
#include "rx/scheduler.h"

namespace rx {
namespace internal {

class TimerManager {
public:
    using TimerWork = std::function<void(int)>;

public:
    TimerManager(ThreadManager& thread_manager);
    ~TimerManager();

    TimerManager(const TimerManager&) = delete;
    TimerManager& operator=(const TimerManager&) = delete;

    int SetTimer(std::chrono::steady_clock::time_point time_point, std::shared_ptr<Scheduler> scheduler, TimerWork timer_work);
    void ResetTimer(int id);

private:
    class Timer {
    public:
        int id{};
        std::chrono::steady_clock::time_point time_point;
        std::shared_ptr<Scheduler> scheduler;
        TimerWork work;
    };
    
    class ThreadSharedState {
    public:
        std::mutex lock;
        std::condition_variable event;
        std::deque<Timer> timers;
        std::atomic<bool> is_stopped{};
    };

private:
    static void TimerThreadWork(const std::shared_ptr<ThreadSharedState>& state);

private:
    std::shared_ptr<Thread> timer_thread_;
    std::shared_ptr<ThreadSharedState> state_;
    std::atomic<int> timer_id_seed_{};
};

}
}