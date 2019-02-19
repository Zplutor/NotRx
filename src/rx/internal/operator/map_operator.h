#pragma once

#include "rx/internal/general_observer.h"
#include "rx/internal/observable.h"

namespace rx {
namespace internal {

class MapOperator : public Observable {
public:
    MapOperator(const std::shared_ptr<Observable>& source, std::function<std::any(const std::any&)> map_operator) :
        source_(source),
        map_operator_(map_operator) {

    }

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override {
        auto map_observer = std::make_shared<MapObserver>(observer, map_operator_);
        auto subscription = source_->Subscribe(map_observer);
        map_observer->AttachSourceSubscription(subscription);
        return subscription;
    }

private:
    class MapObserver : public Observer {
    public:
        MapObserver(std::shared_ptr<Observer> next_observer, std::function<std::any(const std::any&)> map_operator) :
            next_observer_(std::move(next_observer)),
            map_operator_(std::move(map_operator)) {

        }

        void AttachSourceSubscription(std::shared_ptr<Subscription> subscription) {
            source_subscription_ = std::move(subscription);
        }

        void OnNext(const std::any& value) override {

            if (is_finished_) {
                return;
            }

            std::any mapped_value;
            try {
                mapped_value = map_operator_(value);
            }
            catch (const Error& error) {

                is_finished_ = true;
                if (source_subscription_ != nullptr) {
                    source_subscription_->Unsubscribe();
                }

                next_observer_->OnError(error);
                return;
            }

            next_observer_->OnNext(mapped_value);
        }

        void OnError(const Error& error) override {
            if (!is_finished_) {
                next_observer_->OnError(error);
            }
        }

        void OnCompleted() override {
            if (!is_finished_) {
                next_observer_->OnCompleted();
            }
        }

    private:
        std::shared_ptr<Observer> next_observer_;
        std::function<std::any(const std::any&)> map_operator_;
        std::shared_ptr<Subscription> source_subscription_;
        bool is_finished_{};
    };

private:
    std::shared_ptr<Observable> source_;
    std::function<std::any(const std::any&)> map_operator_;
};

}
}