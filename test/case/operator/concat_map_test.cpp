#include <gtest/gtest.h>
#include "rx/observable.h"
#include "rx/creation.h"

TEST(ConcatMap, ConcatMap) {

    std::string concat_values;
    int completed_count{};

    rx::Range(0, 5).ConcatMap<int>([](int v) {
        return rx::Range(v, 2);
    })
    .Subscribe([&concat_values](int v) {
        concat_values += std::to_string(v);
    }, 
    [&completed_count]() {
        completed_count++;
    });

    ASSERT_EQ(concat_values, "0112233445");
    ASSERT_EQ(completed_count, 1);
}


TEST(ConcatMap, Error) {

    std::string concat_values;
    rx::Error error;
    int completed_count{};

    rx::Range(0, 5).ConcatMap<int>([](int v) {
        if (v == 3) {
            throw rx::Error("ConcatMap", 2);
        }
        return rx::Range(v, 2);
    })
    .Subscribe([&concat_values](int v) {
        concat_values += std::to_string(v);
    },
    [&error](const rx::Error& e) {
        error = e;
    },
    [&completed_count]() {
        completed_count++;
    });

    ASSERT_EQ(concat_values, "011223");
    ASSERT_EQ(error.domain, "ConcatMap");
    ASSERT_EQ(error.code, 2);
    ASSERT_EQ(completed_count, 0);
}