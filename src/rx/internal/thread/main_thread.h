#pragma once

#include <deque>
#include <mutex>
#include "rx/run_loop_thread.h"

namespace rx {
namespace internal {

class MainThread : public RunLoopThread {
public:
    void Run();

    void DoWork(std::function<void()> work) override;

private:
    std::deque<Work> work_queue_;
    std::mutex lock_;
};

}
}