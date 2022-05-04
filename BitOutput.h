#pragma once
#include <cstdint>
#include <istream>
#include <vector>

class BitOutput {
public:
    BitOutput(std::ostream& stream);
    void WriteBits(std::vector<bool> data, bool is_little_endian);
    void WriteNumber(size_t data, size_t bits, bool is_little_endian);
    void Close();

private:
    uint8_t buffer_;
    uint8_t bits_;
    std::ostream& stream_;
};

