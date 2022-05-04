#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "BitOutput.h"

using Bitset = std::vector<bool>;
const Bitset FILENAME_END = {1, 0, 0, 0, 0, 0, 0, 0, 0};
const Bitset ONE_MORE_FILE = {1, 0, 0, 0, 0, 0, 0, 0, 1};
const Bitset ARCHIVE_END = {1, 0, 0, 0, 0, 0, 0, 1, 0};
const uint8_t BYTE_SIZE = 9;

class Archiver {

public:
    void CompressFile(const std::string &filename, BitOutput& writer, bool is_last_file) const;
    std::unordered_map<Bitset, Bitset> BuildCanonical(const std::vector<std::pair<size_t, Bitset>> &lengths) const;
    void DecompressArchive(const std::string &filename) const;
};