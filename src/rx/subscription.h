#pragma once

#include <memory>
#include "rx/internal/subscription.h"

namespace rx {

class Subscription {
public:
    explicit Subscription(const std::shared_ptr<internal::Subscription>& impl) :
        impl_(impl) {
    
    }

    void Unsubscribe() {
        impl_->Unsubscribe();
    }

private:
    std::shared_ptr<internal::Subscription> impl_;
};

}