#pragma once

#include <atomic>
#include "rx/internal/observable.h"
#include "rx/scheduler.h"

namespace rx {
namespace internal {

class SubscribeOnOperator : public Observable {
public:
    SubscribeOnOperator(std::shared_ptr<Observable> source, std::shared_ptr<Scheduler> scheduler) :
        source_(std::move(source)), scheduler_(std::move(scheduler)) { }

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override {
        auto subscription = std::make_shared<SubscribeOnSubscription>(source_, observer, scheduler_);
        subscription->Subscribe();
        return subscription;
    }

private:
    class SubscribeOnSubscription : public Subscription, public std::enable_shared_from_this<SubscribeOnSubscription> {
    public:
        SubscribeOnSubscription(std::shared_ptr<Observable> source, std::shared_ptr<Observer> observer, std::shared_ptr<Scheduler> scheduler) :
            source_(std::move(source)), observer_(std::move(observer)), scheduler_(std::move(scheduler)) { }

        void Subscribe() {
            scheduler_->Schedule(std::bind(&SubscribeOnSubscription::SubscribeOnScheduler, shared_from_this()));
        }

        void Unsubscribe() override {
            is_unsubscribed_.store(true);
            scheduler_->Schedule(std::bind(&SubscribeOnSubscription::UnsubscribeOnScheduler, shared_from_this()));
        }

    private:
        void SubscribeOnScheduler() {
            if (is_unsubscribed_.load()) {
                return;
            }
            source_subscription_ = source_->Subscribe(observer_);
        }

        void UnsubscribeOnScheduler() {
            if (source_subscription_ != nullptr) {
                source_subscription_->Unsubscribe();
            }
        }

    private:
        std::shared_ptr<Observable> source_;
        std::shared_ptr<Observer> observer_;
        std::shared_ptr<Scheduler> scheduler_;
        std::shared_ptr<Subscription> source_subscription_;
        std::atomic<bool> is_unsubscribed_{};
    };

private:
    std::shared_ptr<Observable> source_;
    std::shared_ptr<Scheduler> scheduler_;
};

}
}