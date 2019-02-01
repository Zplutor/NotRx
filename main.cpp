#include <iostream>
#include <string>
#include <thread>
#include "rx/observable.h"
#include "rx/scheduler.h"

int main() {
   
    std::cout << "Main thread: " << std::this_thread::get_id() << std::endl;

    rx::Observable<void>::Range(0, 10)
    .ObserveOn(rx::Scheduler::MainThread())
    .Map<std::string>([](int value) {
        std::cout << "Map on thread: " << std::this_thread::get_id() << std::endl;
        return std::to_string(value);
    }).Subscribe([](std::string value){
        std::cout << "Subscribe on thread: " << std::this_thread::get_id() << std::endl;
        std::cout << value << std::endl;
    });

    rx::Scheduler::RunMainThread();
}
