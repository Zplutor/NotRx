#pragma once

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
        return source_->Subscribe(std::make_shared<ObserveOnObserver>(observer, scheduler_));
    }

private:
    class ObserveOnObserver : public Observer {
    public:
        ObserveOnObserver(std::shared_ptr<Observer> next_observer, std::shared_ptr<Scheduler> scheduler) : 
            next_observer_(next_observer),
            scheduler_(scheduler) {

        }

        void OnNext(const std::any& value) override {
            scheduler_->Schedule(std::bind(&Observer::OnNext, next_observer_, value));
        }

        void OnError(const Error& error) override {
            scheduler_->Schedule(std::bind(&Observer::OnError, next_observer_, error));
        }

        void OnCompleted() override {
            scheduler_->Schedule(std::bind(&Observer::OnCompleted, next_observer_));
        }

    private:
        std::shared_ptr<Observer> next_observer_;
        std::shared_ptr<Scheduler> scheduler_;
    };

private:
    std::shared_ptr<Observable> source_;
    std::shared_ptr<Scheduler> scheduler_;
};

}
}