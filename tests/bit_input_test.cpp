
#include "../BitInput.h"

TEST(BitInput, Read) {
    std::string str = "ab\0";
    std::stringstream ss(str);
    BitInput reader(ss, false);
    auto first_part = reader.ReadBits(9);
    std::vector<bool> first_expected = {0, 1, 1, 0, 0, 0, 0, 1, 0};
    auto second_part = reader.ReadBits(16);
    std::vector<bool> second_expected = {1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(first_part, first_expected);
    ASSERT_EQ(second_part, second_expected);
    ASSERT_TRUE(reader.Eof());

    reader.ResetStream();

    ASSERT_FALSE(reader.Eof());
    auto resetted_first_part = reader.ReadBits(9);
    std::vector<bool> resetted_first_expected = {0, 1, 1, 0, 0, 0, 0, 1, 0};
    auto resetted_second_part = reader.ReadBits(16);
    std::vector<bool> resetted_second_expected = {1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(resetted_first_part, resetted_first_expected);
    ASSERT_EQ(resetted_second_part, resetted_second_expected);
    ASSERT_TRUE(reader.Eof());

}