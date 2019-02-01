#include "rx/internal/scheduler/new_thread_scheduler.h"

namespace rx {
namespace internal {

NewThreadScheduler::NewThreadScheduler(ThreadManager& thread_manager) : 
    thread_manager_(thread_manager) {

}


void NewThreadScheduler::Schedule(Work work) {
    thread_manager_.CreateThread(std::move(work));
}

}
}