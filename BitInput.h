#pragma once
#include <cstdint>
#include <istream>
#include <vector>

class BitInput {
public:
    BitInput(std::istream& stream, bool is_little_endian);
    std::vector<bool> ReadBits(size_t count);
    void ResetStream();
    bool Eof();

private:
    uint8_t buffer_;
    uint8_t bits_remain_;
    std::istream& stream_;
    bool eof_;
    bool is_little_endian_;
};

