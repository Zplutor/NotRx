#include "rx/creation.h"
#include "rx/internal/creator/range_creator.h"

namespace rx {

Observable<int> Range(int start, int length, int step) {
    return Observable<int>(std::make_shared<internal::RangeCreator>(start, length, step));
}

}