#pragma once

#include "rx/scheduler.h"

namespace rx {
namespace internal {

class NewThreadScheduler : public Scheduler {
public:
    void Schedule(Work work) override;
};

}
}