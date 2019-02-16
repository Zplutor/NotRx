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

    rx::SubscriptionConatiner container;
    container += rx::Merge<int>({ rx::Range(0, 6) })
        .SubscribeOn(rx::Scheduler::NewThread())
        .Subscribe([](int v) {
            std::cout << v << std::endl;
        });
    
    std::string line;
    while (std::getline(std::cin, line));
}
