#include "rx/internal/thread/thread.h"

namespace rx {
namespace internal {

Thread::~Thread() {

}


Thread::Thread(Work work, std::function<void(Thread*)> end) :
    end_(std::move(end)),
    thread_(std::bind(&Thread::Run, this, std::move(work))) {

}


void Thread::Run(Work work) {
    work();
    end_(this);
}

}
}