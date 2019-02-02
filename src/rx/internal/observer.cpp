#include "rx/internal/observer.h"
#include "rx/internal/general_observer.h"

namespace rx {
namespace internal {

std::shared_ptr<Observer> Observer::Create(rx::OnNext<std::any> on_next, rx::OnError on_error, rx::OnCompleted on_completed) {
    return std::make_shared<GeneralObserver>(std::move(on_next), std::move(on_error), std::move(on_completed));
}

}
}