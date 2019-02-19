#include <gtest/gtest.h>
#include "rx/observable.h"
#include "rx/creation.h"

TEST(Map, Map) {

    std::string concat;

    rx::Range(0, 10).Map<std::string>([](int v) {
        return std::to_string(v);
    })
    .Subscribe([&concat](const std::string& v) {
        concat += v;
    });

    ASSERT_EQ(concat, "0123456789");
}


TEST(Map, Error) {

    std::string concat;
    rx::Error error;
    int completed_count{};

    rx::Range(0, 10).Map<std::string>([](int v) {
        if (v == 4) {
            throw rx::Error("Map", 1);
        }
        return std::to_string(v);
    })
    .Subscribe([&concat](const std::string& v) {
        concat += v;
    }, 
    [&error](const rx::Error& e) {
        error = e;
    }, 
    [&completed_count]() {
        completed_count++;
    });

    ASSERT_EQ(concat, "0123");
    ASSERT_EQ(error.domain, "Map");
    ASSERT_EQ(error.code, 1);
    ASSERT_EQ(completed_count, 0);
}