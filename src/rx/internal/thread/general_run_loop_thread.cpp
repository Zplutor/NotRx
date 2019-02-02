#include "rx/internal/thread/general_run_loop_thread.h"

namespace rx {
namespace internal {

GeneralRunLoopThread::GeneralRunLoopThread(std::function<std::shared_ptr<Thread>(Work)> create) {

    state_ = std::make_shared<State>();
    thread_ = create(std::bind(Run, state_));
}


GeneralRunLoopThread::~GeneralRunLoopThread() {

    state_->is_stopped.store(true);
    state_->work_event.notify_one();
}


std::thread::id GeneralRunLoopThread::GetId() {
    return thread_->GetId();
}


void GeneralRunLoopThread::DoWork(Work work) {

    std::scoped_lock<std::mutex> lock(state_->works_lock);
    state_->works.push_back(std::move(work));
    state_->work_event.notify_one();
}


void GeneralRunLoopThread::Run(std::shared_ptr<State> state) {

    while (true) {

        std::unique_lock<std::mutex> lock(state->works_lock);

        if (state->works.empty()) {
            state->work_event.wait(lock, [&state]() {
                return state->is_stopped.load() || !state->works.empty();
            });
        }

        if (state->is_stopped.load()) {
            break;
        }

        auto work = std::move(state->works.front());
        state->works.pop_front();

        lock.unlock();
        work();
    }
}

}
}