#pragma once

#include <optional>
#include "rx/internal/observable.h"
#include "rx/internal/observer.h"

namespace rx {
namespace internal {

class LastOperator : public Observable {
public:
    LastOperator(std::shared_ptr<Observable> source) : source_(std::move(source)) {

    }

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override {
        return source_->Subscribe(std::make_shared<LastObserver>(observer));
    }

private:
    class LastObserver : public Observer {
    public:
        LastObserver(std::shared_ptr<Observer> next_observer) : next_observer_(std::move(next_observer)) {

        }

        void OnNext(const std::any& value) override {
            last_value_ = value;
        }

        void OnError(const Error& error) override {
            next_observer_->OnError(error);
        }

        void OnCompleted() override {
            if (last_value_) {
                next_observer_->OnNext(*last_value_);
            }
            next_observer_->OnCompleted();
        }

    private:
        std::shared_ptr<Observer> next_observer_;
        std::optional<std::any> last_value_;
    };

private:
    std::shared_ptr<Observable> source_;
};

}
}