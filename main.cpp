#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
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
     
    Logger() << "Main thread: " << std::this_thread::get_id();
    
    rx::Observable<void>::Range(0, 10)
    .ObserveOn(rx::Scheduler::ThreadPool())
    .Map<std::string>([](int value) {
        Logger() << "Map on thread: " << std::this_thread::get_id();
        return std::to_string(value);
    }).Subscribe([](std::string value){
        Logger() << "Subscribe on thread: " << std::this_thread::get_id() << "\n" << value;
    });

    std::string line;
    while (std::getline(std::cin, line));
}
