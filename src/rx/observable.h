#pragma once

#include <memory>
#include "rx/internal/observable.h"
#include "rx/observer.h"
#include "rx/observer_functions.h"
#include "rx/subscription.h"

namespace rx {

class Scheduler;

template<typename T>
class Observable {
public:
    explicit Observable(std::shared_ptr<internal::Observable> handle) : handle_(std::move(handle)) {
    
    }

    Subscription Subscribe(OnNext<T> on_next, OnError on_error = nullptr, OnCompleted on_completed = nullptr) {
        auto subscription = handle_->Subscribe([on_next](const std::any& value) {
            on_next(std::any_cast<T>(value));
        }, on_error, on_completed);
        return Subscription(subscription);
    }

    Subscription Subscribe(const Observer<T>& observer) {
        return Subscription(handle_->Subscribe(observer.GetHandle()));
    }
    
    Observable<T> ObserveOn(std::shared_ptr<Scheduler> scheduler) {
        return Observable<T>(handle_->ObserveOn(scheduler));
    }

    Observable<T> Last() {
        return Observable<T>(handle_->Last());
    }

    template<typename K>
    Observable<K> Map(std::function<K(const T&)> map_operator) {
        return Observable<K>(handle_->Map([map_operator](const std::any& value) {
            return map_operator(std::any_cast<T>(value));
        }));
    }

private:
    std::shared_ptr<internal::Observable> handle_;
};


template<>
class Observable<void> {
public:
    static Observable<int> Range(int from, int to);
};

}