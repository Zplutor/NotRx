#include "rx/internal/scheduler/timer_scheduler.h"

namespace rx {
namespace internal {

TimerScheduler::TimerScheduler(ThreadManager& thread_manager) : 
    thread_(thread_manager.CreateRunLoopThread()) {

}


void TimerScheduler::Schedule(Work work) {
    thread_->DoWork(std::move(work));
}

}
}