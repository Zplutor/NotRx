#include "rx/internal/scheduler/new_thread_scheduler.h"
#include <thread>

namespace rx {
namespace internal {

void NewThreadScheduler::Schedule(Work work) {

    std::thread new_thread(work);
    new_thread.detach();
}

}
}