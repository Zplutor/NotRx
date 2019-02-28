#include "rx/internal/scheduler/timer_manager.h"
#include <algorithm>
#include <optional>

namespace rx {
namespace internal {

TimerManager::TimerManager(ThreadManager& thread_manager) {

    state_ = std::make_shared<ThreadSharedState>();
    timer_thread_ = thread_manager.CreateThread(std::bind(&TimerManager::TimerThreadWork, state_));
}


TimerManager::~TimerManager() {

    state_->is_stopped.store(true);
    state_->event.notify_one();
}


int TimerManager::SetTimer(std::chrono::steady_clock::time_point time_point, std::shared_ptr<Scheduler> scheduler, TimerWork timer_work) {

    auto timer_id = ++timer_id_seed_;

    Timer timer;
    timer.id = timer_id;
    timer.time_point = time_point;
    timer.scheduler = std::move(scheduler);
    timer.work = std::move(timer_work);

    std::scoped_lock<std::mutex> lock(state_->lock);

    bool need_notify{};
    if (state_->timers.empty()) {
        need_notify = true;
    }
    else {
        if (timer.time_point < state_->timers.front().time_point) {
            need_notify = true;
        }
    }

    state_->timers.push_back(std::move(timer));
    std::sort(state_->timers.begin(), state_->timers.end(), [](const Timer& timer1, const Timer& timer2) {
        return timer1.time_point < timer2.time_point;
    });

    if (need_notify) {
        state_->event.notify_one();
    }

    return timer_id;
}


void TimerManager::ResetTimer(int id) {

    std::scoped_lock<std::mutex> lock(state_->lock);
    if (state_->timers.empty()) {
        return;
    }

    bool need_notify{};
    if (state_->timers.front().id == id) {
        need_notify = true;
    }

    auto end = std::remove_if(state_->timers.begin(), state_->timers.end(), [id](const Timer& timer) {
        return timer.id == id;
    });
    state_->timers.erase(end, state_->timers.end());
    
    if (need_notify) {
        state_->event.notify_one();
    }
}


void TimerManager::TimerThreadWork(const std::shared_ptr<ThreadSharedState>& state) {

    std::unique_lock<std::mutex> lock(state->lock);
    while (true) {

        if (state->is_stopped.load()) {
            break;
        }

        if (state->timers.empty()) {
            state->event.wait(lock);
            continue;
        }
        
        auto now = std::chrono::steady_clock::now();
        auto& first_timer = state->timers.front();
        if (now < first_timer.time_point) {
            state->event.wait_until(lock, first_timer.time_point);
            continue;
        }
        
        auto id = first_timer.id;
        auto scheduler = std::move(first_timer.scheduler);
        auto work = std::move(first_timer.work);
        state->timers.pop_front();

        lock.unlock();
        scheduler->Schedule(std::bind(work, id));
        lock.lock();
    }
}

}
}