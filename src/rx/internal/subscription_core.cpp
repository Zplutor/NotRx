#include "rx/internal/subscription_core.h"
#include <vector>

namespace rx {
namespace internal {

void SubscriptionCore::Unsubscribe() {

    if (SetFinish()) {
        OnUnsubscribe();
        NotifyFinish();
    }
}


void SubscriptionCore::FinishSubscription() {

    if (SetFinish()) {
        NotifyFinish();
    }
}


bool SubscriptionCore::SetFinish() {
    
    std::scoped_lock<std::mutex> lock(lock_);
    if (is_finished_) {
        return false;
    }
    is_finished_ = true;
    return true;
}


int SubscriptionCore::RegisterFinishCallback(FinishCallback callback) {

    std::scoped_lock<std::mutex> lock(lock_);
    if (is_finished_) {
        return {};
    }

    auto id = ++id_seed_;
    finish_callbacks_[id] = std::move(callback);
    return id;
}


void SubscriptionCore::UnregisterFinishCallback(int id) {

    std::scoped_lock<std::mutex> lock(lock_);
    finish_callbacks_.erase(id);
}


void SubscriptionCore::NotifyFinish() {

    std::map<int, FinishCallback> callbacks;
    {
        std::scoped_lock<std::mutex> lock(lock_);
        callbacks = std::move(finish_callbacks_);
    }

    for (const auto& each_pair : callbacks) {
        each_pair.second(each_pair.first);
    }
}

}
}