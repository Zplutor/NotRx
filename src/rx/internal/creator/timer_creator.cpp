#include "rx/internal/creator/timer_creator.h"
#include "rx/internal/scheduler/scheduler_manager.h"
#include "rx/internal/subscription_core.h"
#include "rx/internal/subscription_creation.h"

namespace rx {
namespace internal {
namespace {

class TimerSubscriptionCore : public SubscriptionCore {
public:
    TimerSubscriptionCore(
        std::optional<std::chrono::steady_clock::duration> interval,
        std::shared_ptr<Scheduler> scheduler,
        std::shared_ptr<Observer> observer)
        :
        interval_(interval),
        scheduler_(std::move(scheduler)),
        observer_(std::move(observer)) {

    }

    void Subscribe(std::chrono::steady_clock::duration delay) {

        next_time_point_ = std::chrono::steady_clock::now() + delay;

        auto& timer_manager = SchedulerManager::Instance().GetTimerManager();
        timer_id_ = timer_manager.SetTimer(next_time_point_, scheduler_, std::bind(&TimerSubscriptionCore::OnTimer, this));
    }

    void OnUnsubscribe() override {
        SchedulerManager::Instance().GetTimerManager().ResetTimer(timer_id_);
    }

private:
    void OnTimer() {

        observer_->OnNext(value_);
        value_++;

        if (interval_) {

            next_time_point_ += *interval_;

            auto& timer_manager = SchedulerManager::Instance().GetTimerManager();
            timer_id_ = timer_manager.SetTimer(next_time_point_, scheduler_, std::bind(&TimerSubscriptionCore::OnTimer, this));
        }
        else {
            observer_->OnCompleted();
            FinishSubscription();
        }
    }

private:
    std::optional<std::chrono::steady_clock::duration> interval_;
    std::shared_ptr<Scheduler> scheduler_;
    std::shared_ptr<Observer> observer_;
    int value_{};
    int timer_id_{};
    std::chrono::steady_clock::time_point next_time_point_;
};

}

TimerCreator::TimerCreator(
    std::chrono::steady_clock::duration delay,
    std::optional<std::chrono::steady_clock::duration> interval,
    std::shared_ptr<Scheduler> scheduler) 
    :
    delay_(delay),
    interval_(interval),
    scheduler_(std::move(scheduler)) {

}


std::shared_ptr<Subscription> TimerCreator::Subscribe(const std::shared_ptr<Observer>& observer) {

    auto subscription_core = std::make_shared<TimerSubscriptionCore>(interval_, scheduler_, observer);
    subscription_core->Subscribe(delay_);
    return MakeSubscription(subscription_core);
}

}
}