#pragma once

#include <atomic>
#include "rx/internal/observable.h"
#include "rx/internal/subscription_core.h"
#include "rx/internal/subscription_creation.h"
#include "rx/scheduler.h"

namespace rx {
namespace internal {

class ObserveOnOperator : public Observable {
public:
    ObserveOnOperator(std::shared_ptr<Observable> source, std::shared_ptr<Scheduler> scheduler) : 
        source_(std::move(source)),
        scheduler_(std::move(scheduler)) {

    }

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override {
        auto observe_on_observer = std::make_shared<ObserveOnObserver>(observer, scheduler_);
        auto source_subscription = source_->Subscribe(observe_on_observer);
        observe_on_observer->AttachSourceSubscription(source_subscription);
        return MakeSubscription(observe_on_observer);
    }

private:
    class ObserveOnObserver : public Observer, public SubscriptionCore, public std::enable_shared_from_this<ObserveOnObserver> {
    public:
        ObserveOnObserver(std::shared_ptr<Observer> next_observer, std::shared_ptr<Scheduler> scheduler) :
            next_observer_(std::move(next_observer)),
            scheduler_(std::move(scheduler)) { }

        void OnNext(const std::any& value) override {
            scheduler_->Schedule(std::bind(&ObserveOnObserver::OnNextOnScheduler, shared_from_this(), value));
        }

        void OnError(const Error& error) override {
            scheduler_->Schedule(std::bind(&ObserveOnObserver::OnErrorOnScheduler, shared_from_this(), error));
        }

        void OnCompleted() override {
            scheduler_->Schedule(std::bind(&ObserveOnObserver::OnCompletedOnScheduler, shared_from_this()));
        }

        void AttachSourceSubscription(std::shared_ptr<Subscription> source_subscription) {
            source_subscription_ = std::move(source_subscription);
        }

    protected:
        void OnUnsubscribe() override {
            is_subscribed_.store(false);
            source_subscription_->Unsubscribe();
        }

    private:
        void OnNextOnScheduler(const std::any& value) {
            if (is_subscribed_.load()) {
                next_observer_->OnNext(value);
            }
        }

        void OnErrorOnScheduler(const Error& error) {
            if (is_subscribed_.load()) {
                next_observer_->OnError(error);
                FinishSubscription();
            }
        }

        void OnCompletedOnScheduler() {
            if (is_subscribed_.load()) {
                next_observer_->OnCompleted();
                FinishSubscription();
            }
        }

    private:
        std::shared_ptr<Subscription> source_subscription_;
        std::shared_ptr<Observer> next_observer_;
        std::shared_ptr<Scheduler> scheduler_;
        std::atomic<bool> is_subscribed_{ true };
    };

private:
    std::shared_ptr<Observable> source_;
    std::shared_ptr<Scheduler> scheduler_;
};

}
}