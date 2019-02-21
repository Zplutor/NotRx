#pragma once

#include <functional>
#include <map>
#include <mutex>

namespace rx {
namespace internal {

class SubscriptionCore {
public:
    using FinishCallback = std::function<void(int)>;

public:
    SubscriptionCore() = default;
    virtual ~SubscriptionCore() = default;

    SubscriptionCore(const SubscriptionCore&) = delete;
    SubscriptionCore& operator=(const SubscriptionCore&) = delete;

    void Unsubscribe();
    void FinishSubscription();

    std::pair<bool, int> RegisterFinishCallback(FinishCallback callback);
    void UnregisterFinishCallback(int id);

protected:
    virtual void OnUnsubscribe() = 0;

private:
    bool SetFinish();
    void NotifyFinish();

private:
    bool is_finished_{};
    int id_seed_{};
    std::map<int, FinishCallback> finish_callbacks_;
    std::mutex lock_;
};

}
}