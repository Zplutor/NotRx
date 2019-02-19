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
    explicit Observable(std::shared_ptr<internal::Observable> handle) : handle_(std::move(handle)) { }

    Subscription Subscribe(OnNext<T> on_next) {
        return Subscribe(std::move(on_next), nullptr, nullptr);
    }

    Subscription Subscribe(OnNext<T> on_next, OnError on_error) {
        return Subscribe(std::move(on_next), std::move(on_error), nullptr);
    }

    Subscription Subscribe(OnNext<T> on_next, OnCompleted on_completed) {
        return Subscribe(std::move(on_next), nullptr, std::move(on_completed));
    }

    Subscription Subscribe(OnNext<T> on_next, OnError on_error, OnCompleted on_completed) {
        return Subscribe(Observer<T>::Create(std::move(on_next), std::move(on_error), std::move(on_completed)));
    }

    Subscription Subscribe(const Observer<T>& observer) {
        return Subscription(handle_->Subscribe(observer.GetHandle()));
    }
    
    Observable SubscribeOn(const std::shared_ptr<Scheduler>& scheduler) {
        return Observable(handle_->SubscribeOn(scheduler));
    }

    Observable ObserveOn(const std::shared_ptr<Scheduler>& scheduler) {
        return Observable(handle_->ObserveOn(scheduler));
    }

    Observable First() {
        return Observable(handle_->First());
    }

    Observable Last() {
        return Observable(handle_->Last());
    }

    template<typename K>
    Observable<K> Map(std::function<K(const T&)> map_operator) {
        return Observable<K>(handle_->Map([map_operator](const std::any& value) {
            return map_operator(std::any_cast<T>(value));
        }));
    }

    template<typename K>
    Observable<K> ConcatMap(std::function<Observable<K>(const T&)> concat_map_operator) {
        return Observable<K>(handle_->ConcatMap([concat_map_operator](const std::any& value) {
            return concat_map_operator(std::any_cast<T>(value)).GetHandle();
        }));
    }

    const std::shared_ptr<internal::Observable>& GetHandle() const {
        return handle_;
    }

private:
    std::shared_ptr<internal::Observable> handle_;
};

}