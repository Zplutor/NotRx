#include "rx/creation.h"
#include "rx/internal/creator/range_creator.h"
#include "rx/internal/creator/timer_creator.h"
#include "rx/internal/scheduler/scheduler_manager.h"

namespace rx {

Observable<int> Range(int start, int length, int step) {
    return Observable<int>(std::make_shared<internal::RangeCreator>(start, length, step));
}


Observable<int> Timer(std::chrono::steady_clock::duration delay) {
    return Timer(delay, internal::SchedulerManager::Instance().GetTimerScheduler());
}

Observable<int> Timer(std::chrono::steady_clock::duration delay, std::shared_ptr<Scheduler> scheduler) {
    return Observable<int>(std::make_shared<internal::TimerCreator>(delay, std::optional<std::chrono::steady_clock::duration>{}, scheduler));
}


Observable<int> Timer(std::chrono::steady_clock::duration delay, std::chrono::steady_clock::duration interval) {
    return Timer(delay, interval, internal::SchedulerManager::Instance().GetTimerScheduler());
}

Observable<int> Timer(std::chrono::steady_clock::duration delay, std::chrono::steady_clock::duration interval, std::shared_ptr<Scheduler> scheduler) {
    return Observable<int>(std::make_shared<internal::TimerCreator>(delay, interval, scheduler));
}


Observable<int> Interval(std::chrono::steady_clock::duration interval) {
    return Interval(interval, internal::SchedulerManager::Instance().GetTimerScheduler());
}

Observable<int> Interval(std::chrono::steady_clock::duration interval, std::shared_ptr<Scheduler> scheduler) {
    return Observable<int>(std::make_shared<internal::TimerCreator>(interval, interval, scheduler));
}

}