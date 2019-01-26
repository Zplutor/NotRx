#pragma once

#include <any>
#include "rx/error.h"

namespace rx {
namespace internal {

class Observer {
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