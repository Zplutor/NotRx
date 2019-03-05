#include "rx/internal/subject/subject.h"
#include "rx/internal/subject/subject_core.h"

namespace rx {
namespace internal {

Subject::Subject() : core_(std::make_shared<SubjectCore>()) {

}


std::shared_ptr<Observable> Subject::GetObservable() {
    return core_;
}


std::shared_ptr<Observer> Subject::GetObserver() {
    return core_;
}

}
}