#include "rx/scheduler.h"
#include "rx/internal/scheduler/scheduler_manager.h"

namespace rx {

std::shared_ptr<Scheduler> Scheduler::MainThread() {
    return internal::SchedulerManager::Instance().GetMainThreadScheduler();
}


std::shared_ptr<Scheduler> Scheduler::NewThread() {
    return internal::SchedulerManager::Instance().GetNewThreadScheduler();
}


void Scheduler::RunMainThread() {
    internal::SchedulerManager::Instance().GetThreadManager().GetMainThread()->Run();
}

}