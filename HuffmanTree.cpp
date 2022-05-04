#include "HuffmanTree.h"
#include "PriorityQueue.h"
#include <stdexcept>
#include <queue>

Node::Node() {
    l_ = nullptr;
    r_ = nullptr;
    data_ = std::vector<bool> (0);
    entry_count_ = 0;
}

std::unordered_map<std::vector<bool>, size_t> Node::GenerateLengths() const {
    std::queue<std::pair<const Node*, size_t> > queue;
    std::unordered_map<std::vector<bool>, size_t> result;
    queue.push({this, 0});
    while (!queue.empty()) {
        const Node* vertex;
        size_t length;
        std::tie(vertex, length) = queue.front();
        queue.pop();
        if (vertex->l_ == nullptr && vertex->r_ == nullptr) {
            result[vertex->data_] = length;
        } else {
            queue.push({vertex->l_, length + 1});
            queue.push({vertex->r_, length + 1});
        }
    }
    return result;
}

bool Node::operator<(const Node& oth) const {
    return entry_count_ < oth.entry_count_;
}

Node::Node(std::vector<bool> data, size_t entry_count) {
    data_ = data;
    entry_count_ = entry_count;
    l_ = nullptr;
    r_ = nullptr;
}

Node* Node::Merge(const Node* oth) const {
    Node *res = new Node();
    res->entry_count_ = entry_count_ + oth->entry_count_;
    res->l_ = this;
    res->r_ = oth;
    return res;
}

Node* Node::Build(const std::unordered_map<std::vector<bool>, size_t>& count) {
    PriorityQueue<Node*, NodeComparator> queue;
    for(const auto& [byte, cnt]: count) {
        queue.Push(new Node(byte, cnt));
    }
    while(queue.Size() > 1) {
        Node* lhs = queue.Top();
        queue.Pop();
        Node* rhs = queue.Top();
        queue.Pop();
        queue.Push(lhs->Merge(rhs));
    }
    return queue.Top();
}

bool NodeComparator::operator()(const Node* lhs, const Node* rhs) const {
    return !(*lhs < *rhs);
}
