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
        return source_->Subscribe(std::make_shared<MapObserver>(observer, map_operator_));
    }

private:
    class MapObserver : public Observer {
    public:
        MapObserver(const std::shared_ptr<Observer>& next_observer, std::function<std::any(const std::any&)> map_operator) :
            next_observer_(next_observer),
            map_operator_(map_operator) {

        }

        void OnNext(const std::any& value) override {
            next_observer_->OnNext(map_operator_(value));
        }

        void OnError(const Error& error) override {
            next_observer_->OnError(error);
        }

        void OnCompleted() override {
            next_observer_->OnCompleted();
        }

    private:
        std::shared_ptr<Observer> next_observer_;
        std::function<std::any(const std::any&)> map_operator_;
    };

private:
    std::shared_ptr<Observable> source_;
    std::function<std::any(const std::any&)> map_operator_;
};

}
}