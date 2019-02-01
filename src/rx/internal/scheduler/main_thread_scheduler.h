#pragma once

#include "rx/internal/thread/thread_manager.h"
#include "rx/scheduler.h"

namespace rx {
namespace internal {

class MainThreadScheduler : public Scheduler {
public:
    MainThreadScheduler(ThreadManager&);

    void Schedule(Work work) override;

private:
    ThreadManager& thread_manager_;
};

}
}