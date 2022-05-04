#pragma once
#include <vector>
#include <unordered_map>

class Node {
    size_t entry_count_;
    std::vector<bool> data_;
    const Node *l_;
    const Node *r_;
public:

    Node();

    Node(std::vector<bool> data, size_t entry_count);

    static Node* Build(const std::unordered_map<std::vector<bool>, size_t> &count);
    Node* Merge(const Node* oth) const;
    std::unordered_map<std::vector<bool>, size_t> GenerateLengths() const;
    bool operator<(const Node &oth) const;
};

struct NodeComparator {

    bool operator()(const Node* lhs, const Node* rhs) const;

};

using HuffmanTree = Node;