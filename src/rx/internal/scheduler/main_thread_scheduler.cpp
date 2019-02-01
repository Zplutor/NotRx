#include "rx/internal/scheduler/main_thread_scheduler.h"

namespace rx {
namespace internal {

MainThreadScheduler::MainThreadScheduler(ThreadManager& thread_manager) : 
    thread_manager_(thread_manager) {

}


void MainThreadScheduler::Schedule(Work work) {
    thread_manager_.GetMainThread()->DoWork(std::move(work));
}

}
}