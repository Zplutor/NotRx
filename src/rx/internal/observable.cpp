#include "rx/internal/observable.h"
#include "rx/internal/general_observer.h"
#include "rx/internal/last_operator.h"
#include "rx/internal/map_operator.h"
#include "rx/internal/operator/observe_on_operator.h"

namespace rx {
namespace internal {

std::shared_ptr<Subscription> Observable::Subscribe(OnNext<std::any> on_next, OnError on_error, OnCompleted on_completed) {
    return Subscribe(std::make_shared<GeneralObserver>(on_next, on_error, on_completed));
}


std::shared_ptr<Observable> Observable::ObserveOn(const std::shared_ptr<Scheduler>& scheduler) {
    return std::make_shared<ObserveOnOperator>(shared_from_this(), scheduler);
}


std::shared_ptr<Observable> Observable::Last() {
    return std::make_shared<LastOperator>(shared_from_this());
}


std::shared_ptr<Observable> Observable::Map(std::function<std::any(const std::any&)> map_operator) {
    return std::make_shared<MapOperator>(shared_from_this(), map_operator);
}

}
}