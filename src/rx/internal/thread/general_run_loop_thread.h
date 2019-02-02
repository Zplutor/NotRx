#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include "rx/internal/thread/thread.h"
#include "rx/run_loop_thread.h"

namespace rx {
namespace internal {

class GeneralRunLoopThread : public RunLoopThread {
public:
    GeneralRunLoopThread(std::function<std::shared_ptr<Thread>(Work)> create);
    ~GeneralRunLoopThread();

    std::thread::id GetId() override;
    void DoWork(Work work) override;

private:
    class State {
    public:
        std::atomic<bool> is_stopped{};
        std::deque<Work> works;
        std::mutex works_lock;
        std::condition_variable work_event;
    };

private:
    static void Run(std::shared_ptr<State>);

private:
    std::shared_ptr<Thread> thread_;
    std::shared_ptr<State> state_;
};

}
}