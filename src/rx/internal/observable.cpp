#include "rx/internal/observable.h"
#include "rx/internal/operator/concat_map_operator.h"
#include "rx/internal/operator/last_operator.h"
#include "rx/internal/operator/map_operator.h"
#include "rx/internal/operator/observe_on_operator.h"

namespace rx {
namespace internal {

std::shared_ptr<Observable> Observable::ObserveOn(const std::shared_ptr<Scheduler>& scheduler) {
    return std::make_shared<ObserveOnOperator>(shared_from_this(), scheduler);
}


std::shared_ptr<Observable> Observable::Last() {
    return std::make_shared<LastOperator>(shared_from_this());
}


std::shared_ptr<Observable> Observable::Map(std::function<std::any(const std::any&)> map_operator) {
    return std::make_shared<MapOperator>(shared_from_this(), map_operator);
}


std::shared_ptr<Observable> Observable::ConcatMap(std::function<std::shared_ptr<Observable>(const std::any&)> concat_map_operator) {
    return std::make_shared<ConcatMapOperator>(shared_from_this(), concat_map_operator);
}

}
}