#include "rx/internal/scheduler/thread_pool_scheduler.h"
#include <optional>
#include "rx/internal/thread/thread_manager.h"

namespace rx {
namespace internal {

ThreadPoolScheduler::ThreadPoolScheduler(ThreadManager& thread_manager) :
    thread_manager_(thread_manager) {

}


void ThreadPoolScheduler::Schedule(Work work) {

    auto thread_info = PickFreeThread();
    if (thread_info.first == nullptr) {
        thread_info = CreateNewThread();
    }

    Worker worker;
    worker.work = std::move(work);
    worker.thread_index = thread_info.second;
    worker.scheduler = weak_from_this();
    thread_info.first->DoWork(std::move(worker));
}


std::pair<std::shared_ptr<RunLoopThread>, std::size_t> ThreadPoolScheduler::PickFreeThread() {

    auto current_thread_id = std::this_thread::get_id();

    std::scoped_lock<std::mutex> lock(lock_);
    std::optional<std::size_t> picked_index;
    for (std::size_t index = 0; index < thread_items_.size(); ++index) {
        auto& item = thread_items_[index];

        if (item.IsBusy()) {
            //Prefer the same thread.
            if (item.thread->GetId() == current_thread_id) {
                picked_index = index;
                break;
            }
        }
        else {
            //Prefer the first free thread.
            if (!picked_index.has_value()) {
                picked_index = index;
            }
        }
    }

    if (picked_index.has_value()) {
        auto& picked_item = thread_items_[picked_index.value()];
        picked_item.work_count++;
        return std::make_pair(picked_item.thread, picked_index.value());
    }

    return {};
}


std::pair<std::shared_ptr<RunLoopThread>, std::size_t> ThreadPoolScheduler::CreateNewThread() {

    auto new_thread = thread_manager_.CreateRunLoopThread();

    ThreadItem thread_item;
    thread_item.thread = new_thread;
    thread_item.work_count = 1;

    std::scoped_lock<std::mutex> lock(lock_);
    thread_items_.push_back(thread_item);

    auto index = thread_items_.size() - 1;
    return std::make_pair(new_thread, index);
}


void ThreadPoolScheduler::ThreadWorkEnd(std::size_t index) {

    std::scoped_lock<std::mutex> lock(lock_);
    auto& item = thread_items_[index];
    item.work_count--;
}


void ThreadPoolScheduler::Worker::operator()() {

    work();

    auto scheduler_ptr = scheduler.lock();
    if (scheduler_ptr != nullptr) {
        scheduler_ptr->ThreadWorkEnd(thread_index);
    }
}

}
}