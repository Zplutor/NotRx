#pragma once

#include <functional>
#include "rx/error.h"

namespace rx {

template<typename T>
using OnNext = std::function<void(const T&)>;
using OnError = std::function<void(const Error&)>;
using OnCompleted = std::function<void()>;

}