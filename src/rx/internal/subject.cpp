#include "rx/internal/subject.h"
#include <algorithm>
#include "rx/internal/subscription_core.h"
#include "rx/internal/subscription_creation.h"

namespace rx {
namespace internal {

class SubjectImpl : public Observable, public Observer {
public:
    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override {

        auto subscription_core = std::make_shared<SubjectSubscriptionCore>(std::dynamic_pointer_cast<SubjectImpl>(shared_from_this()), observer);
        observer_items_.push_back(ObserverItem{ observer, subscription_core });
        return MakeSubscription(subscription_core);
    }

    void OnNext(const std::any& value) override {
        for (const auto& each_item : observer_items_) {
            each_item.observer->OnNext(value);
        }
    }

    void OnError(const Error& error) override {

        for (const auto& each_item : observer_items_) {

            each_item.observer->OnError(error);

            auto subscription_core = each_item.subscription_core.lock();
            if (subscription_core != nullptr) {
                subscription_core->FinishSubscription();
            }
        }
    }

    void OnCompleted() override {

        for (const auto& each_item : observer_items_) {

            each_item.observer->OnCompleted();

            auto subscription_core = each_item.subscription_core.lock();
            if (subscription_core != nullptr) {
                subscription_core->FinishSubscription();
            }
        }
    }

private:
    class SubjectSubscriptionCore : public SubscriptionCore {
    public:
        SubjectSubscriptionCore(std::shared_ptr<SubjectImpl> subject, std::shared_ptr<Observer> observer) : 
            subject_(std::move(subject)),
            observer_(std::move(observer)) {

        }

    protected:
        void OnUnsubscribe() override {
            auto subject = subject_.lock();
            if (subject != nullptr) {
                subject->RemoveObserver(observer_);
            }
        }

    private:
        std::weak_ptr<SubjectImpl> subject_;
        std::shared_ptr<Observer> observer_;
    };

    class ObserverItem {
    public:
        std::shared_ptr<Observer> observer;
        std::weak_ptr<SubscriptionCore> subscription_core;
    };

private:
    void RemoveObserver(const std::shared_ptr<Observer>& observer) {
        auto end = std::remove_if(observer_items_.begin(), observer_items_.end(), [&observer](const ObserverItem& item) {
            return item.observer == observer;
        });
        observer_items_.erase(end, observer_items_.end());
    }

private:
    std::vector<ObserverItem> observer_items_;
};


std::shared_ptr<Subject> Subject::Create() {
    return std::make_shared<Subject>();
}


Subject::Subject() : impl_(std::make_shared<SubjectImpl>()) {

}


Subject::~Subject() {

}


std::shared_ptr<Observable> Subject::GetObservable() {
    return impl_;
}


std::shared_ptr<Observer> Subject::GetObserver() {
    return impl_;
}

}
}