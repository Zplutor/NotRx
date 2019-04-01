#include <gtest/gtest.h>
#include "rx/observable.h"
#include "rx/creation.h"

TEST(Replay, Replay) {

    auto observable = rx::Range(0, 5).Replay();

    std::string string;
    auto on_next = [&string](int v) {
        string += std::to_string(v);
    };

    int complete_count{};
    auto on_completed = [&complete_count]() {
        complete_count++;
    };

    observable.Subscribe(on_next, on_completed);
    observable.Subscribe(on_next, on_completed);
    observable.Subscribe(on_next, on_completed);

    ASSERT_EQ(string, "012340123401234");
    ASSERT_EQ(complete_count, 3);
}