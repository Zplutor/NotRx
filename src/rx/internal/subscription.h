#pragma once

namespace rx {
namespace internal {

class Subscription {
public:
    Subscription() = default;
    virtual ~Subscription() = default;

    Subscription(const Subscription&) = delete;
    Subscription& operator=(const Subscription&) = delete;

    virtual void Unsubscribe() = 0;
};

}
}