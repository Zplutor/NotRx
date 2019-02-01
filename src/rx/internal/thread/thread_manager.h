#pragma once

#include <memory>
#include <mutex>
#include <vector>
#include "rx/internal/thread/main_thread.h"
#include "rx/internal/thread/thread.h"
#include "rx/run_loop_thread.h"

namespace rx {
namespace internal {

class ThreadManager {
public:
    ~ThreadManager();

    std::shared_ptr<MainThread> GetMainThread();
    std::shared_ptr<RunLoopThread> CreateRunLoopThread();
    std::shared_ptr<Thread> CreateThread(Work work);

private:
    void ThreadEnd(Thread* thread);

private:
    std::shared_ptr<MainThread> main_thread_;
    std::once_flag main_thread_once_flag_;

    std::vector<std::shared_ptr<Thread>> threads_;
    bool is_joining_threads_{};
    std::mutex threads_lock_;
};

}
}