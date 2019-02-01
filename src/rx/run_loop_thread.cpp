#include "rx/run_loop_thread.h"
#include "rx/internal/scheduler/scheduler_manager.h"
#include "rx/internal/thread/thread_manager.h"

namespace rx {

std::shared_ptr<RunLoopThread> RunLoopThread::Main() {
    return internal::SchedulerManager::Instance().GetThreadManager().GetMainThread();
}


std::shared_ptr<RunLoopThread> RunLoopThread::Create() {
    return internal::SchedulerManager::Instance().GetThreadManager().CreateRunLoopThread();
}

}