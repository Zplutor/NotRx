#include <iostream>
#include "rx/observable.h"
#include "rx/scheduler.h"
#include <thread>

int main() {
    
    rx::Observable<void>::Range(0, 10).Map<std::string>([](int value) {
        return std::to_string(value);
    }).Subscribe([](std::string value){
        std::cout << value << std::endl;
    });

    std::cout << "MainThread: " << std::this_thread::get_id() << std::endl;
    rx::Scheduler::NewThread()->Schedule([]() {
        std::cout << "ThisThread: " << std::this_thread::get_id() << std::endl;
    });

    int x;
    std::cin >> x;
}
