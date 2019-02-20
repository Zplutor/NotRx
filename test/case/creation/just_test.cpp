#include <gtest/gtest.h>
#include <rx/creation.h>

TEST(Just, Just) {
    
    std::string original_value("just");
    std::string subscribed_value;
    int completed_count{};

    rx::Just(original_value).Subscribe([&subscribed_value](const std::string& v) {
        subscribed_value = v;
    },
    [&completed_count]() {
        completed_count++;
    });

    ASSERT_EQ(subscribed_value, original_value);
    ASSERT_EQ(completed_count, 1);
}