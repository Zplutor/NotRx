#pragma once

#include <memory>
#include "rx/internal/subscription.h"

namespace rx {

class Subscription {
public:
    static Subscription Empty();

public:
    explicit Subscription(const std::shared_ptr<internal::Subscription>& impl) :
        handle_(impl) {
    
    }

    void Unsubscribe() {
        handle_->Unsubscribe();
    }

    const std::shared_ptr<internal::Subscription>& GetHandle() const {
        return handle_;
    }

private:
    std::shared_ptr<internal::Subscription> handle_;
};

}