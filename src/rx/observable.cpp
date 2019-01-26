#include "rx/observable.h"
#include "rx/internal/range_source.h"

namespace rx {

Observable<int> Observable<void>::Range(int from, int to) {

    auto impl = std::make_shared<internal::RangeSource>(from, to);
    return Observable<int>(impl);
}

}