#pragma once

#include <thread>
#include <functional>
#include "rx/work.h"

namespace rx {

class RunLoopThread {
public:
    static std::shared_ptr<RunLoopThread> Main();
    static std::shared_ptr<RunLoopThread> Create();

public:
    RunLoopThread() = default;
    virtual ~RunLoopThread() = default;

    RunLoopThread(const RunLoopThread&) = delete;
    RunLoopThread& operator=(const RunLoopThread&) = delete;

    virtual std::thread::id GetId() = 0;
    virtual void DoWork(Work work) = 0;
};

}