#pragma once

#include <chrono>
#include "rx/observable.h"
#include "rx/observer.h"
#include "rx/internal/creator/customized_creator.h"
#include "rx/internal/creator/just_creator.h"
#include "rx/internal/creator/merge_creator.h"

namespace rx {

Observable<int> Range(int start, int length, int step = 1);

Observable<int> Timer(std::chrono::steady_clock::duration delay);
Observable<int> Timer(std::chrono::steady_clock::duration delay, std::shared_ptr<Scheduler> scheduler);

Observable<int> Timer(std::chrono::steady_clock::duration delay, std::chrono::steady_clock::duration interval);
Observable<int> Timer(std::chrono::steady_clock::duration delay, std::chrono::steady_clock::duration interval, std::shared_ptr<Scheduler> scheduler);

Observable<int> Interval(std::chrono::steady_clock::duration interval);
Observable<int> Interval(std::chrono::steady_clock::duration interval, std::shared_ptr<Scheduler> scheduler);


template<typename T>
Observable<T> Create(std::function<Subscription(Observer<T>)> creator) {
    auto observable_handle = std::make_shared<internal::CustomizedCreator>([creator](const std::shared_ptr<internal::Observer>& observer) {
        return creator(Observer<T>(observer)).GetHandle();
    });
    return Observable<T>(observable_handle);
}


template<typename T>
Observable<T> Just(const T& value) {
    return Observable<T>(std::make_shared<internal::JustCreator>(value));
}


template<typename T>
Observable<T> Merge(const std::vector<Observable<T>>& observables) {
    std::vector<std::shared_ptr<internal::Observable>> handles;
    for (const auto& each_observable : observables) {
        handles.push_back(each_observable.GetHandle());
    }
    return Observable<T>(std::make_shared<internal::MergeCreator>(handles));
}

}