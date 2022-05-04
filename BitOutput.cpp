//
// Created by Kirill Balandin on 02.10.2021.
//

#include <iostream>
#include "BitOutput.h"

BitOutput::BitOutput(std::ostream& stream)
    : buffer_(0), bits_(0), stream_(stream){
}

void BitOutput::WriteBits(std::vector<bool> data, bool is_little_endian = false) {
    if (is_little_endian) {
        std::reverse(data.begin(), data.end());
    }
    for (const bool& bit : data) {
        if (bit) {
            buffer_ = buffer_ | (1 << (7 - bits_));
        }
        bits_++;
        if (bits_ == 8) {
            stream_.put(buffer_);
            buffer_ = 0;
            bits_ = 0;
        }
    }
    stream_.flush();

}
void BitOutput::Close() {
    if (bits_ > 0) {
       stream_.put(buffer_);
    }
    stream_.flush();
}

void BitOutput::WriteNumber(size_t data, size_t bits, bool is_little_endian) {
    std::vector<bool> bit(bits, false);
    size_t i = 0;
    while (data > 0) {
        bit[bits - i - 1] = (data & 1);
        data >>= 1;
        ++i;
    }
    WriteBits(bit, is_little_endian);
}
