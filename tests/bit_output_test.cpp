
#include "../BitOutput.h"

TEST(BitOutput, Write) {
    std::stringstream ss;
    BitOutput writer(ss);
    writer.WriteBits({0, 0, 0, 0, 1, 0, 1, 0}, true);
    ASSERT_EQ(ss.str(), "P");
    writer.WriteBits({0, 1, 0, 1, 0, 0, 0, 0}, false);
    ASSERT_EQ(ss.str(), "PP");
    writer.WriteNumber('q', 8, false);
    ASSERT_EQ(ss.str(), "PPq");
}