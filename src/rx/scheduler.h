#pragma once

#include <functional>
#include <memory>

namespace rx {

class Scheduler {
public:
    using Work = std::function<void()>;

public:
    static std::shared_ptr<Scheduler> NewThread();

public:
    Scheduler() = default;
    virtual ~Scheduler() = default;

    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;

    virtual void Schedule(Work work) = 0;
};

}