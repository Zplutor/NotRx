#pragma once

#include "rx/internal/thread/thread_manager.h"
#include "rx/scheduler.h"
#include "rx/run_loop_thread.h"

namespace rx {
namespace internal {

class TimerScheduler : public Scheduler {
public:
    TimerScheduler(ThreadManager& thread_manager);

    void Schedule(Work work) override;

private:
    std::shared_ptr<RunLoopThread> thread_;
};

}
}