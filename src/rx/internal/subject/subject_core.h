#pragma once

#include <mutex>
#include "rx/internal/observable.h"
#include "rx/internal/observer.h"
#include "rx/internal/subscription_core.h"
#include "rx/work.h"

namespace rx {
namespace internal {

class SubjectCore : public Observable, public Observer {
public:
    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override;

    void OnNext(const std::any& value) override;
    void OnError(const Error& error) override;
    void OnCompleted() override;

protected:
    virtual Work OnSubscribeExtension(const std::shared_ptr<Observer>& observer) { return {}; }
    virtual void OnNextExtension(const std::any& value) { }
    virtual void OnErrorExtension(const Error& error) { }
    virtual void OnCompletedExtension() { }

private:
    class SubjectSubscriptionCore : public SubscriptionCore {
    public:
        SubjectSubscriptionCore(std::shared_ptr<SubjectCore> subject, std::shared_ptr<Observer> observer);

    protected:
        void OnUnsubscribe() override;

    private:
        std::weak_ptr<SubjectCore> subject_core_;
        std::shared_ptr<Observer> observer_;
    };

    class ObserverItem {
    public:
        std::shared_ptr<Observer> observer;
        std::weak_ptr<SubscriptionCore> subscription_core;
    };

private:
    void RemoveObserver(const std::shared_ptr<Observer>& observer);

private:
    std::vector<ObserverItem> observer_items_;
    std::mutex lock_;
};

}
}