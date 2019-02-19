#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include "rx/creation.h"
#include "rx/observable.h"
#include "rx/scheduler.h"
#include "rx/subscription_container.h"

class Logger {
public:
    Logger() : stream_(std::make_unique<std::ostringstream>()) { }
    Logger(Logger&& other) : stream_(std::move(other.stream_)) { }

    template<typename T>
    Logger operator<<(const T& t) {
        *stream_ << t;
        return std::move(*this);
    }

    ~Logger() {
        if (stream_ != nullptr) {
            *stream_ << std::endl;
            std::cout << stream_->str();
        }
    }

private:
    std::unique_ptr<std::ostringstream> stream_;
};


int main() {
    
    rx::Create<int>([](rx::Observer<int> observer) {
    
        auto is_cancel = std::make_shared<bool>(false);

        for (int i = 0; i < 100; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (*is_cancel) {
                break;
            }
            observer.OnNext(i);
        }

        if (!*is_cancel) {
            observer.OnCompleted();
        }

        return rx::Subscription::Create([is_cancel]() {
            *is_cancel = true;
        });
    })
    .SubscribeOn(rx::Scheduler::ThreadPool())
    .Subscribe([](int v) {
        std::cout << "OnNext: " << v << std::endl;
    }, 
    [](const rx::Error& error) {
    
    },
    []() {
        std::cout << "OnCompleted" << std::endl;
    });

    std::string line;
    while (std::getline(std::cin, line));
}
