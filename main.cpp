#include <iostream>
#include "rx/observable.h"

int main() {
    
    rx::Observable<void>::Range(0, 10).Map<std::string>([](int value) {
        return std::to_string(value);
    }).Subscribe([](std::string value){
        std::cout << value << std::endl;
    });
}
