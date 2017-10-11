#include <gtest/gtest.h>

int main (int c, char ** argv) {
    ::testing::InitGoogleTest(&c, argv);
    return RUN_ALL_TESTS();
}
