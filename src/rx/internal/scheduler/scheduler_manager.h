#pragma once

#include "rx/scheduler.h"

namespace rx {
namespace internal {

class SchedulerManager {
public:
    static SchedulerManager& Instance();

    std::shared_ptr<Scheduler> GetNewThreadScheduler();

private:
    std::shared_ptr<Scheduler> new_thread_scheduler_;
};

}
}