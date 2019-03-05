#include "rx/internal/subject/replay_subject.h"
#include "rx/internal/subject/replay_subject_core.h"

namespace rx {
namespace internal {

ReplaySubject::ReplaySubject() : core_(std::make_shared<ReplaySubjectCore>()) {

}


std::shared_ptr<Observable> ReplaySubject::GetObservable() {
    return core_;
}


std::shared_ptr<Observer> ReplaySubject::GetObserver() {
    return core_;
}

}
}