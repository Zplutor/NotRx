#pragma once

#include <memory>
#include "rx/internal/observer.h"

namespace rx {

template<typename T>
class Observer {
public:
    static Observer Create(OnNext<T> on_next, OnCompleted on_completed = nullptr) {
        return Create(std::move(on_next), nullptr, std::move(on_completed));
    }

    static Observer Create(OnNext<T> on_next, OnError on_error = nullptr, OnCompleted on_completed = nullptr) {
        auto bridged_on_next = [on_next](const std::any& value) {
            on_next(std::any_cast<T>(value));
        };
        return Observer(internal::Observer::Create(std::move(bridged_on_next), std::move(on_error), std::move(on_completed)));
    }

public:
    explicit Observer(std::shared_ptr<internal::Observer> handle) : handle_(std::move(handle)) { }

    void OnNext(const T& value) {
        handle_->OnNext(value);
    }

    void OnError(const Error& error) {
        handle_->OnError(error);
    }

    void OnCompleted() {
        handle_->OnCompleted();
    }

    const std::shared_ptr<internal::Observer>& GetHandle() const {
        return handle_;
    }

private:
    std::shared_ptr<internal::Observer> handle_;
};

}