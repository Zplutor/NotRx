#pragma once

#include <chrono>
#include <optional>
#include "rx/internal/observable.h"
#include "rx/scheduler.h"

namespace rx {
namespace internal {

class TimerCreator : public Observable {
public:
    TimerCreator(
        std::chrono::steady_clock::duration delay, 
        std::optional<std::chrono::steady_clock::duration> interval,
        std::shared_ptr<Scheduler> scheduler);

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override;

private:
    std::chrono::steady_clock::duration delay_;
    std::optional<std::chrono::steady_clock::duration> interval_;
    std::shared_ptr<Scheduler> scheduler_;
};

}
}