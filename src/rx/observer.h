#pragma once

#include <memory>
#include "rx/internal/observer.h"

namespace rx {

template<typename T>
class Observer {
public:
    Observer(std::shared_ptr<internal::Observer> handle) : handle_(std::move(handle)) { }

    void OnNext(T value) {
        handle_->OnNext(std::move(value));
    }

    void OnError(Error error) {
        handle_->OnError(std::move(error));
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