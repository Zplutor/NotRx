#include "rx/internal/creator/merge_creator.h"
#include <atomic>
#include "rx/internal/subscription_core.h"
#include "rx/internal/subscription_creation.h"

namespace rx {
namespace internal {
namespace {

class MergeObserver : public Observer, public SubscriptionCore {
public:
    MergeObserver(std::shared_ptr<Observer> next_observer, std::size_t merge_count) :
        next_observer_(std::move(next_observer)), merge_count_(merge_count) { }

    void AttachSourceSubscriptions(std::vector<std::shared_ptr<Subscription>> subscriptions) {
        source_subscriptions_ = std::move(subscriptions);
    }

    void OnNext(const std::any& value) override {
        if (!is_finished_.load()) {
            next_observer_->OnNext(value);
        }
    }

    void OnError(const Error& error) override {
        if (!is_finished_.load()) {
            is_finished_.store(true);
            next_observer_->OnError(error);
            FinishSubscription();
        }
    }

    void OnCompleted() override {
        if (!is_finished_.load()) {
            if (merge_count_.fetch_sub(1) == 1) {
                next_observer_->OnCompleted();
                FinishSubscription();
            }
        }
    }

protected:
    void OnUnsubscribe() override {
        for (const auto& each_subscription : source_subscriptions_) {
            each_subscription->Unsubscribe();
        }
    }

private:
    std::vector<std::shared_ptr<Subscription>> source_subscriptions_;
    std::shared_ptr<Observer> next_observer_;
    std::atomic<std::size_t> merge_count_{};
    std::atomic<bool> is_finished_{};
};

}


MergeCreator::MergeCreator(std::vector<std::shared_ptr<Observable>> observables) : 
    observables_(std::move(observables)) { }


std::shared_ptr<Subscription> MergeCreator::Subscribe(const std::shared_ptr<Observer>& observer) {

    auto merge_observer = std::make_shared<MergeObserver>(observer, observables_.size());

    std::vector<std::shared_ptr<Subscription>> subscriptions;
    for (const auto& each_observable : observables_) {
        auto subscription = each_observable->Subscribe(merge_observer);
        subscriptions.push_back(subscription);
    }

    merge_observer->AttachSourceSubscriptions(std::move(subscriptions));

    return MakeSubscription(merge_observer);
}

}
}