#include <gtest/gtest.h>
#include "rx/observable.h"
#include "rx/creation.h"

TEST(Last, Last) {

    int sum{};
    int completed_count{};

    rx::Range(0, 10).Last().Subscribe([&sum](int v) {
        sum += v;
    }, 
    [&completed_count]() {
        completed_count++;
    });

    ASSERT_EQ(sum, 9);
    ASSERT_EQ(completed_count, 1);
}