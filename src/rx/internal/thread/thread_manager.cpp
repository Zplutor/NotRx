#include "rx/internal/thread/thread_manager.h"
#include "rx/internal/thread/general_run_loop_thread.h"
#include "rx/internal/thread/main_thread.h"

namespace rx {
namespace internal {

ThreadManager::~ThreadManager() {

    std::vector<std::shared_ptr<Thread>> threads;
    {
        std::scoped_lock<std::mutex> lock(threads_lock_);
        threads = threads_;
        is_joining_threads_ = true;
    }

    for (const auto& each_thread : threads) {
        each_thread->Join();
    }
}


std::shared_ptr<MainThread> ThreadManager::GetMainThread() {

    std::call_once(main_thread_once_flag_, [this]() {
        main_thread_ = std::make_shared<MainThread>();
    });

    return main_thread_;
}


std::shared_ptr<RunLoopThread> ThreadManager::CreateRunLoopThread() {
    return std::make_shared<GeneralRunLoopThread>([this](Work work) {
        return CreateThread(std::move(work));
    });
}


std::shared_ptr<Thread> ThreadManager::CreateThread(Work work) {

    std::scoped_lock<std::mutex> lock(threads_lock_);
    auto thread = std::make_shared<Thread>(std::move(work), std::bind(&ThreadManager::ThreadEnd, this, std::placeholders::_1));
    threads_.push_back(thread);
    return thread;
}


void ThreadManager::ThreadEnd(Thread* thread) {

    std::scoped_lock<std::mutex> lock(threads_lock_);
    if (is_joining_threads_) {
        return;
    }

    auto iterator = threads_.begin();
    while (iterator != threads_.end()) {

        if (iterator->get() == thread) {
            (*iterator)->Detach();
            iterator = threads_.erase(iterator);
            break;
        }
        ++iterator;
    }
}


}
}