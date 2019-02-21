#pragma once

#include <atomic>
#include "rx/internal/observable.h"
#include "rx/internal/subscription_creation.h"
#include "rx/internal/subscription_core.h"
#include "rx/scheduler.h"

namespace rx {
namespace internal {

class SubscribeOnOperator : public Observable {
public:
    SubscribeOnOperator(std::shared_ptr<Observable> source, std::shared_ptr<Scheduler> scheduler) :
        source_(std::move(source)), scheduler_(std::move(scheduler)) { }

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override {
        auto subscription_core = std::make_shared<SubscribeOnSubscriptionCore>(source_, observer, scheduler_);
        subscription_core->Subscribe();
        return MakeSubscription(subscription_core);
    }

private:
    class SubscribeOnSubscriptionCore : public SubscriptionCore, public std::enable_shared_from_this<SubscribeOnSubscriptionCore> {
    public:
        SubscribeOnSubscriptionCore(std::shared_ptr<Observable> source, std::shared_ptr<Observer> observer, std::shared_ptr<Scheduler> scheduler) :
            source_(std::move(source)), observer_(std::move(observer)), scheduler_(std::move(scheduler)) { }

        ~SubscribeOnSubscriptionCore() {
            if (source_subscription_ != nullptr) {
                auto source_core = source_subscription_->GetCore();
                if (source_core != nullptr) {
                    source_core->UnregisterFinishCallback(source_subscription_finish_callback_id_);
                }
            }
        }

        void Subscribe() {
            scheduler_->Schedule(std::bind(&SubscribeOnSubscriptionCore::SubscribeOnScheduler, shared_from_this()));
        }

        void OnUnsubscribe() override {
            is_unsubscribed_.store(true);
            scheduler_->Schedule(std::bind(&SubscribeOnSubscriptionCore::UnsubscribeOnScheduler, shared_from_this()));
        }

    private:
        void SubscribeOnScheduler() {
            if (is_unsubscribed_.load()) {
                return;
            }

            source_subscription_ = source_->Subscribe(observer_);

            auto source_core = source_subscription_->GetCore();
            if (source_core != nullptr) {

                auto weak_pointer = weak_from_this();
                auto register_result = source_core->RegisterFinishCallback([weak_pointer](int) {
                    auto this_core = weak_pointer.lock();
                    this_core->FinishSubscription();
                });

                if (register_result.first) {
                    source_subscription_finish_callback_id_ = register_result.second;
                }
                else {
                    //Source subscription has finished, finish this subscription as well.
                    FinishSubscription();
                }
            }
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
        int source_subscription_finish_callback_id_{};
        std::atomic<bool> is_unsubscribed_{};
    };

private:
    std::shared_ptr<Observable> source_;
    std::shared_ptr<Scheduler> scheduler_;
};

}
}