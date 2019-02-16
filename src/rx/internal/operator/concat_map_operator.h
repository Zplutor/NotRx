#pragma once

#include <set>
#include <map>
#include "rx/internal/general_observer.h"
#include "rx/internal/observable.h"
#include "rx/internal/subscription_core.h"
#include "rx/internal/subscription_creation.h"

namespace rx {
namespace internal {

class ConcatMapOperator : public Observable {
public:
    using Operator = std::function<std::shared_ptr<Observable>(const std::any&)>;

public:
    ConcatMapOperator(std::shared_ptr<Observable> source, Operator op) : 
        source_(std::move(source)),
        operator_(std::move(op)) {

    }

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override {
        auto flat_map_observer = std::make_shared<ConcatMapObserver>(observer, operator_);
        auto source_subscription = source_->Subscribe(flat_map_observer);
        flat_map_observer->AttachSourceSubscription(source_subscription);
        return MakeSubscription(flat_map_observer);
    }

private:
    class ConcatMapObserver : public Observer, public SubscriptionCore {
    public:
        ConcatMapObserver(std::shared_ptr<Observer> next_observer, Operator op) : 
            next_observer_(std::move(next_observer)),
            operator_(std::move(op)) {

        }

        void AttachSourceSubscription(std::shared_ptr<Subscription> source_subscription) {
            source_subscription_ = std::move(source_subscription);
        }

        void OnNext(const std::any& value) override {

            auto observable = operator_(value);

            auto subscription_id = total_subscription_count_;
            total_subscription_count_++;

            auto subscription = observable->Subscribe(std::make_shared<GeneralObserver>([this](const std::any& value) {
                if (!is_finished_) {
                    next_observer_->OnNext(value);
                }
            }, 
            [this, subscription_id](const Error& error) {
                SubscriptionEnded(subscription_id);
                CheckError(error);
            }, 
            [this, subscription_id]() {
                SubscriptionEnded(subscription_id);
                CheckCompleted();
            }));

            if (ended_subscriptions_.find(subscription_id) == ended_subscriptions_.end()) {
                emitting_subscriptions_[subscription_id] = subscription;
            }
        }

        void OnError(const Error& error) override {
            CheckError(error);
        }

        void OnCompleted() override {
            is_source_completed_ = true;
            CheckCompleted();
        }

    protected:
        void OnUnsubscribe() override {
            source_subscription_->Unsubscribe();
            for (const auto& each_pair : emitting_subscriptions_) {
                each_pair.second->Unsubscribe();
            }
        }

    private:
        void SubscriptionEnded(std::size_t id) {
            emitting_subscriptions_.erase(id);
            ended_subscriptions_.insert(id);
        }

        void CheckError(const Error& error) {
            if (!is_finished_) {
                is_finished_ = true;
                next_observer_->OnError(error);
                FinishSubscription();
            }
        }

        void CheckCompleted() {
            if (!is_finished_) {
                if (is_source_completed_ && ended_subscriptions_.size() == total_subscription_count_) {
                    is_finished_ = true;
                    next_observer_->OnCompleted();
                    FinishSubscription();
                }
            }
        }

    private:
        std::shared_ptr<Subscription> source_subscription_;
        std::shared_ptr<Observer> next_observer_;
        Operator operator_;
        std::size_t total_subscription_count_{};
        std::set<std::size_t> ended_subscriptions_;
        std::map<std::size_t, std::shared_ptr<Subscription>> emitting_subscriptions_;
        bool is_finished_{};
        bool is_source_completed_{};
    };

private:
    std::shared_ptr<Observable> source_;
    Operator operator_;
};

}
}