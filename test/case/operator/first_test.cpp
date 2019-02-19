#include <gtest/gtest.h>
#include "rx/observable.h"
#include "rx/creation.h"

TEST(First, First) {

    int sum{};
    int completed_count{};

    rx::Range(100, 10).First().Subscribe([&sum](int v) {
        sum += v;
    },
    [&completed_count]() {
        completed_count++;
    });

    ASSERT_EQ(sum, 100);
    ASSERT_EQ(completed_count, 1);
}