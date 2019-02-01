#pragma once

#include <functional>
#include <memory>
#include "rx/work.h"

namespace rx {

class Scheduler {
public:
    static std::shared_ptr<Scheduler> MainThread();
    static std::shared_ptr<Scheduler> NewThread();

    static void RunMainThread();

public:
    Scheduler() = default;
    virtual ~Scheduler() = default;

    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;

    virtual void Schedule(Work work) = 0;
};

}