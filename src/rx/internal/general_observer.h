#pragma once

#include <functional>
#include "rx/internal/observer.h"
#include "rx/observer_functions.h"

namespace rx {
namespace internal {

class GeneralObserver : public Observer {
public:
    GeneralObserver(rx::OnNext<std::any> on_next, rx::OnError on_error, rx::OnCompleted on_completed) :
        on_next_(on_next),
        on_error_(on_error),
        on_completed_(on_completed) {

    }

    void OnNext(const std::any& value) override {
        if (on_next_ != nullptr) {
            on_next_(value);
        }
    }

    void OnError(const Error& error) override {
        if (on_error_ != nullptr) {
            on_error_(error);
        }
    }

    void OnCompleted() override {
        if (on_completed_ != nullptr) {
            on_completed_();
        }
    }

private:
    rx::OnNext<std::any> on_next_;
    rx::OnError on_error_;
    rx::OnCompleted on_completed_;
};

}
}