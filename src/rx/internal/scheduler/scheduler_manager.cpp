#include "rx/internal/scheduler/scheduler_manager.h"
#include "rx/internal/scheduler/new_thread_scheduler.h"
#include "rx/internal/scheduler/main_thread_scheduler.h"
#include "rx/internal/scheduler/thread_pool_scheduler.h"
#include "rx/internal/scheduler/timer_scheduler.h"

namespace rx {
namespace internal {

SchedulerManager& SchedulerManager::Instance() {
    static SchedulerManager instance;
    return instance;
}


TimerManager& SchedulerManager::GetTimerManager() {

    std::call_once(timer_manager_once_flag_, [this]() {
        timer_manager_ = std::make_unique<TimerManager>(GetThreadManager());
    });

    return *timer_manager_;
}


std::shared_ptr<Scheduler> SchedulerManager::GetMainThreadScheduler() {

    std::call_once(main_thread_scheduler_once_flag_, [this]() {
        main_thread_scheduler_ = std::make_shared<MainThreadScheduler>(thread_manager_);
    });

    return main_thread_scheduler_;
}


std::shared_ptr<Scheduler> SchedulerManager::GetNewThreadScheduler() {

    std::call_once(new_thread_scheduler_once_flag_, [this]() {
        new_thread_scheduler_ = std::make_shared<NewThreadScheduler>(thread_manager_);
    });

    return new_thread_scheduler_;
}


std::shared_ptr<Scheduler> SchedulerManager::GetThreadPoolScheduler() {

    std::call_once(thread_pool_scheduler_once_flag_, [this]() {
        new_thread_scheduler_ = std::make_shared<ThreadPoolScheduler>(thread_manager_);
    });

    return new_thread_scheduler_;
}


std::shared_ptr<Scheduler> SchedulerManager::GetTimerScheduler() {

    std::call_once(timer_scheduler_once_flag_, [this]() {
        timer_scheduler_ = std::make_shared<TimerScheduler>(thread_manager_);
    });

    return timer_scheduler_;
}

}
}