#pragma once

#include <string>

namespace rx {

class Error {
public:
    Error(std::string domain, int code) : 
        domain(std::move(domain)), code(code) { }

    Error(std::string domain, int code, std::string message) : 
        domain(std::move(domain)), code(code), message(std::move(message)) { }

public:
    std::string domain;
    std::string message;
    int code{};
};

}