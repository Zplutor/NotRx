#pragma once

#include <atomic>
#include "rx/internal/observable.h"
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
        return std::make_shared<ObserveOnSubscription>(observe_on_observer, source_subscription);
    }

private:
    class ObserveOnObserver : public Observer, public std::enable_shared_from_this<ObserveOnObserver> {
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

        void Unsubscribe() {
            is_subscribed_.store(false);
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
            }
        }

        void OnCompletedOnScheduler() {
            if (is_subscribed_.load()) {
                next_observer_->OnCompleted();
            }
        }

    private:
        std::shared_ptr<Observer> next_observer_;
        std::shared_ptr<Scheduler> scheduler_;
        std::atomic<bool> is_subscribed_{ true };
    };

    class ObserveOnSubscription : public Subscription {
    public:
        ObserveOnSubscription(std::shared_ptr<ObserveOnObserver> observer, std::shared_ptr<Subscription> source_subscription) :
            observer_(std::move(observer)), source_subscription_(std::move(source_subscription)) { }

        void Unsubscribe() override {
            source_subscription_->Unsubscribe();
            observer_->Unsubscribe();
        }

    private:
        std::shared_ptr<ObserveOnObserver> observer_;
        std::shared_ptr<Subscription> source_subscription_;
    };

private:
    std::shared_ptr<Observable> source_;
    std::shared_ptr<Scheduler> scheduler_;
};

}
}