#pragma once

#include <mutex>
#include "rx/internal/observable.h"
#include "rx/internal/subject/replay_subject.h"

namespace rx {
namespace internal {

class ReplayOperator : public Observable {
public:
    ReplayOperator(std::shared_ptr<Observable> source) : 
        source_(std::move(source)), replay_subject_(std::make_shared<ReplaySubject>()) { }

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override {

        std::call_once(source_subscribe_once_flag_, [this]() {
            source_->Subscribe(replay_subject_->GetObserver());
        });

        return replay_subject_->GetObservable()->Subscribe(observer);
    }

private:
    std::shared_ptr<Observable> source_;
    std::shared_ptr<ReplaySubject> replay_subject_;
    std::once_flag source_subscribe_once_flag_;
};

}
}