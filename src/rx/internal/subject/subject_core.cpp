#include "rx/internal/subject/subject_core.h"
#include <algorithm>
#include "rx/internal/subscription_creation.h"

namespace rx {
namespace internal {

std::shared_ptr<Subscription> SubjectCore::Subscribe(const std::shared_ptr<Observer>& observer) {

    Work extension_work;
    auto subscription_core = std::make_shared<SubjectSubscriptionCore>(std::dynamic_pointer_cast<SubjectCore>(shared_from_this()), observer);
    {
        std::scoped_lock<std::mutex> lock(lock_);
        extension_work = OnSubscribeExtension(observer);
        observer_items_.push_back(ObserverItem{ observer, subscription_core });
    }
    if (extension_work) {
        extension_work();
    }
    return MakeSubscription(subscription_core);
}


void SubjectCore::OnNext(const std::any& value) {

    std::vector<ObserverItem> observer_items;
    {
        std::scoped_lock<std::mutex> lock(lock_);
        OnNextExtension(value);
        observer_items = observer_items_;
    }

    for (const auto& each_item : observer_items) {
        each_item.observer->OnNext(value);
    }
}


void SubjectCore::OnError(const Error& error) {

    std::vector<ObserverItem> observer_items;
    {
        std::scoped_lock<std::mutex> lock(lock_);
        OnErrorExtension(error);
        observer_items = observer_items_;
    }

    for (const auto& each_item : observer_items) {

        each_item.observer->OnError(error);

        auto subscription_core = each_item.subscription_core.lock();
        if (subscription_core != nullptr) {
            subscription_core->FinishSubscription();
        }
    }
}


void SubjectCore::OnCompleted() {

    std::vector<ObserverItem> observer_items;
    {
        std::scoped_lock<std::mutex> lock(lock_);
        OnCompletedExtension();
        observer_items = observer_items_;
    }

    for (const auto& each_item : observer_items) {

        each_item.observer->OnCompleted();

        auto subscription_core = each_item.subscription_core.lock();
        if (subscription_core != nullptr) {
            subscription_core->FinishSubscription();
        }
    }
}


void SubjectCore::RemoveObserver(const std::shared_ptr<Observer>& observer) {
    std::scoped_lock<std::mutex> lock(lock_);
    auto end = std::remove_if(observer_items_.begin(), observer_items_.end(), [&observer](const ObserverItem& item) {
        return item.observer == observer;
    });
    observer_items_.erase(end, observer_items_.end());
}


SubjectCore::SubjectSubscriptionCore::SubjectSubscriptionCore(
    std::shared_ptr<SubjectCore> subject,
    std::shared_ptr<Observer> observer) 
    :
    subject_core_(std::move(subject)),
    observer_(std::move(observer)) { }


void SubjectCore::SubjectSubscriptionCore::OnUnsubscribe() {
    auto subject = subject_core_.lock();
    if (subject != nullptr) {
        subject->RemoveObserver(observer_);
    }
}

}
}