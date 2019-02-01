#pragma once

#include <mutex>
#include <set>
#include "rx/internal/thread/thread_manager.h"
#include "rx/scheduler.h"
#include "rx/run_loop_thread.h"

namespace rx {
namespace internal {

class NewThreadScheduler : public Scheduler {
public:
    NewThreadScheduler(ThreadManager& thread_manager);

    void Schedule(Work work) override;

private:
    ThreadManager& thread_manager_;
};

}
}