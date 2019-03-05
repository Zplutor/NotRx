#include "rx/internal/subject/replay_subject_core.h"

namespace rx {
namespace internal {

Work ReplaySubjectCore::OnSubscribeExtension(const std::shared_ptr<Observer>& observer) {

    auto values = values_;
    auto error = error_;
    auto is_completed = is_completed_;

    return [values, error, is_completed, observer]() {
    
        for (const auto& each_value : values) {
            observer->OnNext(each_value);
        }

        if (error) {
            observer->OnError(*error);
        }
        else {
            if (is_completed) {
                observer->OnCompleted();
            }
        }
    };
}


void ReplaySubjectCore::OnNextExtension(const std::any& value) {
    values_.push_back(value);
}


void ReplaySubjectCore::OnErrorExtension(const Error& error) {
    error_ = error;
}


void ReplaySubjectCore::OnCompletedExtension() {
    is_completed_ = true;
}

}
}