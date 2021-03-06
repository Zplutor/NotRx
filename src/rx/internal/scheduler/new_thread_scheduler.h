#pragma once

#include "rx/internal/thread/thread_manager.h"
#include "rx/scheduler.h"

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