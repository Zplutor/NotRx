#pragma once

#include <functional>
#include <thread>
#include "rx/work.h"

namespace rx {
namespace internal {

class Thread {
public:
    Thread(Work work, std::function<void(Thread*)> end);

    std::thread::id GetId() {
        return thread_.get_id();
    }

    bool IsJoinable() {
        return thread_.joinable();
    }

    void Join() {
        thread_.join();
    }

    void Detach() {
        thread_.detach();
    }

private:
    void Run(Work work);

private:
    std::thread thread_;
    std::function<void(Thread*)> end_;
};

}
}