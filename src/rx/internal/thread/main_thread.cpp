#include "rx/internal/thread/main_thread.h"

namespace rx {
namespace internal {

void MainThread::Run() {

    while (true) {

        Work work;
        {
            std::scoped_lock<std::mutex> lock(lock_);
            if (work_queue_.empty()) {
                break;
            }

            work = std::move(work_queue_.front());
            work_queue_.pop_front();
        }

        work();
    }
}


std::thread::id MainThread::GetId() {
    return {};
}


void MainThread::DoWork(std::function<void()> work) {

    std::scoped_lock<std::mutex> lock(lock_);
    work_queue_.push_back(std::move(work));
}

}
}