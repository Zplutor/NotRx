#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include "rx/observable.h"
#include "rx/scheduler.h"

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
    
    rx::Observable<void>::Range(0, 10)
    .ConcatMap<int>([](int v) {
        return rx::Observable<void>::Range(v, v + 2);
    })
    .Subscribe([](int v) {
        std::cout << v << std::endl;
    });

    std::string line;
    while (std::getline(std::cin, line));
}
