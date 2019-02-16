#pragma once

#include "rx/internal/observable.h"
#include "rx/internal/observer.h"

namespace rx {
namespace internal {

class FirstOperator : public Observable {
public:
    FirstOperator(std::shared_ptr<Observable> source) : source_(std::move(source)) { }

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override {
        auto first_observer = std::make_shared<FirstObserver>(observer);
        return source_->Subscribe(first_observer);
    }

private:
    class FirstObserver : public Observer {
    public:
        FirstObserver(std::shared_ptr<Observer> next_observer) : next_observer_(next_observer) { }

        void OnNext(const std::any& value) override {
            if (!is_finished_) {
                is_finished_ = true;
                next_observer_->OnNext(value);
                next_observer_->OnCompleted();
            }
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
        bool is_finished_{};
    };

private:
    std::shared_ptr<Observable> source_;
};

}
}