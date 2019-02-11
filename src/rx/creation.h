#pragma once

#include "rx/observable.h"
#include "rx/observer.h"
#include "rx/internal/creator/customized_creator.h"
#include "rx/internal/creator/just_creator.h"

namespace rx {

Observable<int> Range(int start, int length, int step = 1);


template<typename T>
Observable<T> Create(std::function<Subscription(Observer<T> observer)> creator) {
    auto observable_handle = std::make_shared<internal::CustomizedCreator>([creator](const std::shared_ptr<internal::Observer>& observer) {
        return creator(Observer<T>(observer)).GetHandle();
    });
    return Observable<T>(observable_handle);
}


template<typename T>
Observable<T> Just(const T& value) {
    return Observable<T>(std::make_shared<internal::JustCreator>(value));
}

}