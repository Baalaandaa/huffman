#include <gtest/gtest.h>
#include "bit_input_test.cpp"
#include "bit_output_test.cpp"
#include "priority_queue_test.cpp"
#include "huffman_tree_test.cpp"
#include "integration_test.cpp"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}