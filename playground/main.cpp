#include <thread>
#include <iostream>
#include <set>
#include "rx/creation.h"
#include "rx/subject.h"
#include "rx/run_loop_thread.h"
#include "rx/scheduler.h"
#include "rx/subscription_container.h"

class Task {
public:
    virtual ~Task() = default;

    virtual void Execute() = 0;
};


template<typename T>
class TemplateTask : public Task {
public:
    using ValueType = T;

public:
    rx::Observable<T> GetObservable() {
        return subject_.GetObservable();
    }

protected:
    void OnNext(const T& value) {
        subject_.GetObserver().OnNext(value);
    }

    void OnError(const rx::Error& error) {
        subject_.GetObserver().OnError(error);
    }

    void OnCompleted() {
        subject_.GetObserver().OnCompleted();
    }

private:
    rx::Subject<T> subject_;
};


class StringTask : public TemplateTask<std::string> {
public:
    void Execute() override {

        std::cout << "Execute" << std::endl;
        OnNext("1");
        OnNext("2");
        OnNext("3");
        OnCompleted();
    }
};


class TaskDispatcher {
public:
    TaskDispatcher() {
        dispatch_thread_ = rx::RunLoopThread::Create();
    }

    void DispatchTask(const std::shared_ptr<Task>& task) {
        dispatch_thread_->DoWork(std::bind(&TaskDispatcher::DispatchTaskInThread, this, task));
    }

private:
    void DispatchTaskInThread(const std::shared_ptr<Task>& task) {
        task->Execute();
    }

private:
    std::shared_ptr<rx::RunLoopThread> dispatch_thread_;
};


TaskDispatcher g_task_dispatcher;


template<typename TaskType>
rx::Observable<typename TaskType::ValueType> DispatchTask(const std::shared_ptr<TaskType>& task) {
    g_task_dispatcher.DispatchTask(task);
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return task->GetObservable();
}


int main() {

    rx::SubscriptionConatiner subscriptions;

    auto pre_time = std::chrono::steady_clock::now();

    subscriptions += rx::Interval(std::chrono::milliseconds(100)).Subscribe([&pre_time](int v) {
        auto current = std::chrono::steady_clock::now();
        std::cout << "Timer " << v << " after " << std::chrono::duration_cast<std::chrono::milliseconds>(current - pre_time).count() << "ms" << std::endl;
        pre_time = current;
    });

    auto task = std::make_shared<StringTask>();
    subscriptions += DispatchTask(task)
        .ObserveOn(rx::Scheduler::MainThread())
        .Subscribe([](const std::string& value) {
            std::cout << value << std::endl;
        }, 
        []() {
            std::cout << "Completed" << std::endl;
        });

    std::string line;
    while (std::getline(std::cin, line)) {
        rx::Scheduler::RunMainThread();
    }
}