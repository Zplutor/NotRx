#pragma once

#include <any>
#include <memory>
#include "rx/internal/observer.h"
#include "rx/internal/subscription.h"
#include "rx/observer_functions.h"

namespace rx {
namespace internal {

class Observable : public std::enable_shared_from_this<Observable> {
public:
    Observable() = default;
    virtual ~Observable() = default;

    Observable(const Observable&) = delete;
    Observable& operator=(const Observable&) = delete;

    std::shared_ptr<Subscription> Subscribe(OnNext<std::any> on_next, OnError on_error, OnCompleted on_completed);
    virtual std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) = 0;

    std::shared_ptr<Observable> Last();
    std::shared_ptr<Observable> Map(std::function<std::any(const std::any&)> map_operator);
};

}
}