#include "rx/internal/subscription_creation.h"
#include "rx/internal/base_subscription.h"

namespace rx {
namespace internal {
namespace {

std::shared_ptr<Subscription> g_empty_subscription_instance;
std::once_flag g_empty_subscription_instance_once_flag;

}


std::shared_ptr<Subscription> GetEmptySubscription() {

    std::call_once(g_empty_subscription_instance_once_flag, []() {
        g_empty_subscription_instance = std::make_shared<BaseSubscription>();
    });
    return g_empty_subscription_instance;
}


std::shared_ptr<Subscription> MakeSubscription(std::shared_ptr<SubscriptionCore> core) {
    return std::make_shared<BaseSubscription>(std::move(core));
}

}
}