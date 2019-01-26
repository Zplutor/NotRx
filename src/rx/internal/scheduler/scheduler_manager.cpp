#include "rx/internal/scheduler/scheduler_manager.h"
#include "rx/internal/scheduler/new_thread_scheduler.h"

namespace rx {
namespace internal {

SchedulerManager& SchedulerManager::Instance() {
    static SchedulerManager instance;
    return instance;
}


std::shared_ptr<Scheduler> SchedulerManager::GetNewThreadScheduler() {
    if (new_thread_scheduler_ == nullptr) {
        new_thread_scheduler_ = std::make_shared<NewThreadScheduler>();
    }
    return new_thread_scheduler_;
}

}
}