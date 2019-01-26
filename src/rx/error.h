#pragma once

#include <string>

namespace rx {

class Error {
public:
    std::string domain;
    int code;
    std::string message;
};

}