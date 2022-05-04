#include "../HuffmanTree.h"
#include "../Archiver.h"

TEST(HuffmanTree, build) {
    std::unordered_map<std::vector<bool>, size_t> cnt;
    cnt[{1, 0}] = 1;
    cnt[{0, 1}] = 1;
    HuffmanTree* tree;
    ASSERT_NO_THROW(tree = HuffmanTree::Build(cnt));
}

TEST(HuffmanTree, lengths) {
    std::unordered_map<std::vector<bool>, size_t> cnt;
    cnt[{1, 0}] = 1;
    cnt[{0, 1}] = 1;
    HuffmanTree* tree = HuffmanTree::Build(cnt);
    auto codes = tree->GenerateLengths();
    ASSERT_EQ(codes.size(), 2);
    auto code = codes[{1, 0}];
    ASSERT_EQ(code, 1);
}

TEST(HuffmanTree, canonical) {
    std::unordered_map<std::vector<bool>, size_t> cnt;
    cnt[{1, 0}] = 1;
    cnt[{0, 1}] = 1;
    HuffmanTree* tree = HuffmanTree::Build(cnt);
    auto lengths = tree->GenerateLengths();
    std::vector<std::pair<size_t, Bitset>> ordered_symbols;
    for (const auto& [symbol, length] : lengths) {
        ordered_symbols.emplace_back(length, symbol);
    }
    std::sort(ordered_symbols.begin(), ordered_symbols.end());
    const Archiver archiver;
    auto codes = archiver.BuildCanonical(ordered_symbols);
    auto first_code = codes[ordered_symbols[0].second];
    Bitset first_expected_code = {0};
    auto second_code = codes[ordered_symbols[1].second];
    Bitset second_expected_code = {1};
    ASSERT_EQ(first_code, first_expected_code);
    ASSERT_EQ(second_code, second_expected_code);
}