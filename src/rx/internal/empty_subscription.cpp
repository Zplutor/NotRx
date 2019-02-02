#include "rx/internal/empty_subscription.h"
#include <mutex>

namespace rx {
namespace internal {
namespace {

std::shared_ptr<EmptySubscription> g_empty_subscription_instance;
std::once_flag g_empty_subscription_instance_once_flag;

}

std::shared_ptr<EmptySubscription> EmptySubscription::Instance() {

    std::call_once(g_empty_subscription_instance_once_flag, []() {
        g_empty_subscription_instance = std::make_shared<EmptySubscription>();
    });

    return g_empty_subscription_instance;
}

}
}