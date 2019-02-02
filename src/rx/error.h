#pragma once

#include <string>

namespace rx {

class Error {
public:
    std::string domain;
    std::string message;
    int code{};
};

}