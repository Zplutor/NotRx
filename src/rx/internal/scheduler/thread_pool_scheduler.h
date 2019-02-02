#pragma once

#include <mutex>
#include <vector>
#include "rx/run_loop_thread.h"
#include "rx/scheduler.h"

namespace rx {
namespace internal {

class ThreadManager;

class ThreadPoolScheduler : public Scheduler, public std::enable_shared_from_this<ThreadPoolScheduler> {
public:
    ThreadPoolScheduler(ThreadManager& thread_manager);

    void Schedule(Work work) override;

private:
    class ThreadItem {
    public:
        bool IsBusy() const {
            return work_count > 0;
        }

    public:
        std::shared_ptr<RunLoopThread> thread;
        int work_count{};
    };

    class Worker {
    public:
        void operator()();

    public:
        Work work;
        std::size_t thread_index{};
        std::weak_ptr<ThreadPoolScheduler> scheduler;
    };

private:
    std::pair<std::shared_ptr<RunLoopThread>, std::size_t> PickFreeThread();
    std::pair<std::shared_ptr<RunLoopThread>, std::size_t> CreateNewThread();
    void ThreadWorkEnd(std::size_t index);

private:
    ThreadManager& thread_manager_;
    std::vector<ThreadItem> thread_items_;
    std::mutex lock_;
};

}
}