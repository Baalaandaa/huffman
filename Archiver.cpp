#include "Archiver.h"
#include "BitOutput.h"
#include "BitInput.h"
#include "HuffmanTree.h"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <memory>

namespace {
    size_t BitsetToNumber(const Bitset &a) {
        size_t res = 0;
        for (size_t i = 0; i < a.size(); ++i) {
            res <<= 1;
            if (a[i]) {
                res += 1;
            }
        }
        return res;
    }
}

void Archiver::CompressFile(const std::string& filepath, BitOutput& writer, bool is_last_file) const {
    std::ifstream file_stream(filepath);
    size_t slash_index = filepath.rfind('/');
    std::string filename = filepath.substr((slash_index == std::string::npos ? 0 : slash_index + 1));
    if (!file_stream.is_open() || file_stream.bad()) {
        throw new std::runtime_error("File not found");
    }
    BitInput reader(file_stream, false);
    std::unordered_map<std::vector<bool>, size_t> count;
    size_t symbols = 0;

    std::vector<Bitset> filename_bitsets;

    for (uint8_t symbol : filename) {
        Bitset symbol_bitset(BYTE_SIZE, false);
        size_t i = 0;
        while (symbol > 0) {
            symbol_bitset[8 - i] = symbol & 1;
            symbol >>= 1;
            ++i;
        }
        filename_bitsets.push_back(symbol_bitset);
        ++count[symbol_bitset];
    }

    while (!reader.Eof()) {
        auto symbol = reader.ReadBits(8);
        ++symbols;
        if (symbols % 1'000'000 == 0) {
            std::cout << (symbols / 1'000'000) << " Mb processed" << std::endl;
        }
        if (reader.Eof()) {
            break;
        }
        symbol.insert(symbol.begin(), 0);
        ++count[symbol];
    }
    count[FILENAME_END] = 1;
    count[ONE_MORE_FILE] = 1;
    count[ARCHIVE_END] = 1;

    reader.ResetStream();
    std::unique_ptr<HuffmanTree> tree(HuffmanTree::Build(count));
    auto lengths = tree->GenerateLengths();

    std::vector<std::pair<size_t, Bitset>> ordered_symbols;
    for (const auto& [symbol, length] : lengths) {
        ordered_symbols.emplace_back(length, symbol);
    }
    std::sort(ordered_symbols.begin(), ordered_symbols.end());

    try {
        auto codes = BuildCanonical(ordered_symbols);
        writer.WriteNumber(lengths.size(), 9, true);
        std::map<size_t, size_t> size_count;
        for (const auto& [length, symbol] : ordered_symbols) {
            writer.WriteBits(symbol, true);
            ++size_count[length];
        }

        size_t max_symbol_code_size = (size_count.rbegin())->first;
        for (size_t length = 1; length <= max_symbol_code_size; ++length) {
            writer.WriteNumber(size_count[length], 9, true);
        }
        for (const auto& symbol_bitset : filename_bitsets) {
            if (codes.find(symbol_bitset) == codes.end()) {
                throw new std::runtime_error("Symbol doesn't exist in huffman tree :(");
            }
            writer.WriteBits(codes[symbol_bitset], false);
        }
        writer.WriteBits(codes[FILENAME_END], false);

        symbols = 0;
        while (!reader.Eof()) {
            ++symbols;
            if (symbols % 1'000'000 == 0) {
                std::cout << (symbols / 1'000'000) << " Mb processed" << std::endl;
            }
            auto symbol = reader.ReadBits(8);
            if (reader.Eof()) {
                break;
            }
            symbol.insert(symbol.begin(), 0);
            if (codes.find(symbol) == codes.end()) {
                throw new std::runtime_error("Symbol doesn't exist in huffman tree :(");
            }
            writer.WriteBits(codes[symbol], false);
        }
        if (is_last_file) {
            writer.WriteBits(codes[ARCHIVE_END], false);
        } else {
            writer.WriteBits(codes[ONE_MORE_FILE], false);
        }
    } catch (const std::exception& e) {
        throw e;
    }
}
std::unordered_map<Bitset, Bitset> Archiver::BuildCanonical(
    const std::vector<std::pair<size_t, Bitset>>& lengths) const {
    Bitset code(lengths[0].first, false);
    std::unordered_map<Bitset, Bitset> result;
    for (size_t i = 0; i < lengths.size(); ++i) {
        result[lengths[i].second] = code;
        if (i + 1 < lengths.size()) {
            size_t j = code.size();
            while (j > 0 && code[j - 1]) {
                code[j - 1] = false;
                --j;
            }
            if (j > 0) {
                code[j - 1] = true;
            } else {
                throw new std::runtime_error("Canonical code cannot be built");
            }
            for (size_t k = 0; k < lengths[i + 1].first - lengths[i].first; ++k) {
                code.push_back(0);
            }
        }
    }
    return result;
}

void Archiver::DecompressArchive(const std::string& filename) const {
    std::ifstream archive_istream(filename);
    if (!archive_istream.is_open() || archive_istream.bad()) {
        throw new std::runtime_error("File not found");
    }
    BitInput reader(archive_istream, true);
    bool archive_end = false;
    while (!archive_end) {
        size_t symbol_count = BitsetToNumber(reader.ReadBits(9));
        std::vector<Bitset> symbols;
        for (size_t i = 0; i < symbol_count; ++i) {
            symbols.push_back(reader.ReadBits(9));
        }

        std::vector<std::pair<size_t, Bitset>> lengths;
        size_t symbol_index = 0;
        size_t length = 0;
        size_t current_count = 0;

        while(symbol_index < symbol_count) {
            while(current_count == 0) {
                current_count = BitsetToNumber(reader.ReadBits(9));
                ++length;
            }
            lengths.emplace_back(length, symbols[symbol_index]);
            ++symbol_index;
            --current_count;
        }

        auto code_by_symbol = BuildCanonical(lengths);
        std::unordered_map<Bitset, Bitset> codes;
        for (const auto&[symbol, code]: code_by_symbol) {
            codes[code] = symbol;
        }
        Bitset current(0);
        std::string output_filename;
        while (codes.find(current) == codes.end() || codes[current] != FILENAME_END) {
            while(codes.find(current) == codes.end()) {
                bool bit = reader.ReadBits(1)[0];
                current.push_back(bit);
            }
            if (codes[current] != FILENAME_END) {
               output_filename.push_back(static_cast<char>(BitsetToNumber(codes[current])));
               current = Bitset(0);
            }
        }
        std::ofstream file_output(output_filename);
        if (!file_output.is_open() || file_output.bad()) {
            throw new std::runtime_error("Cannot open file to write");
        }
        BitOutput writer(file_output);
        current = Bitset(0);
        size_t symbols_processed = 0;
        while (codes.find(current) == codes.end() || (codes[current] != ARCHIVE_END && codes[current] != ONE_MORE_FILE)) {
            while(codes.find(current) == codes.end()) {
                bool bit = reader.ReadBits(1)[0];
                current.push_back(bit);
            }
            if (codes[current] != ARCHIVE_END && codes[current] != ONE_MORE_FILE) {
                ++symbols_processed;
                auto code = codes[current];
                code.erase(code.begin());
                writer.WriteBits(code, false);
                current = Bitset(0);
            }
            if (symbols_processed % 1'000'000 == 0) {
                std::cout << (symbols_processed / 1'000'000) << " Mb processed" << std::endl;
            }
        }
        writer.Close();
        if (codes[current] == ARCHIVE_END) {
            archive_end = true;
        }
    }
}
