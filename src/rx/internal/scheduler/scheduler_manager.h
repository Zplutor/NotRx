#pragma once

#include <mutex>
#include "rx/internal/thread/thread_manager.h"
#include "rx/scheduler.h"

namespace rx {
namespace internal {

class SchedulerManager {
public:
    static SchedulerManager& Instance();

public:
    ThreadManager& GetThreadManager() {
        return thread_manager_;
    }

    std::shared_ptr<Scheduler> GetMainThreadScheduler();
    std::shared_ptr<Scheduler> GetNewThreadScheduler();
    std::shared_ptr<Scheduler> GetThreadPoolScheduler();

private:
    ThreadManager thread_manager_;

    std::shared_ptr<Scheduler> main_thread_scheduler_;
    std::once_flag main_thread_scheduler_once_flag_;

    std::shared_ptr<Scheduler> new_thread_scheduler_;
    std::once_flag new_thread_scheduler_once_flag_;

    std::shared_ptr<Scheduler> thread_pool_scheduler_;
    std::once_flag thread_pool_scheduler_once_flag_;
};

}
}