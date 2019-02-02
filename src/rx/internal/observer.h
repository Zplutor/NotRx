#pragma once

#include <any>
#include <memory>
#include "rx/error.h"
#include "rx/observer_functions.h"

namespace rx {
namespace internal {

class Observer {
public:
    static std::shared_ptr<Observer> Create(rx::OnNext<std::any> on_next, rx::OnError on_error, rx::OnCompleted on_completed);

public:
    Observer() = default;
    virtual ~Observer() = default;

    Observer(const Observer&) = delete;
    Observer& operator=(const Observer&) = delete;

    virtual void OnNext(const std::any& value) = 0;
    virtual void OnError(const Error& error) = 0;
    virtual void OnCompleted() = 0;
};

}
}