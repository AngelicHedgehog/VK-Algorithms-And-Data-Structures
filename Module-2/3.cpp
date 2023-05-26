#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

template <class T>
class CompareDefault {
 public:
    [[nodiscard]] inline auto operator()(const T& a, const T& b) const noexcept -> int { return a > b ? 1 : (a == b ? 0 : -1); }
};

template <class T, class Compare = CompareDefault<T>>
class BTree {
 public:
    inline BTree(const size_t minSize, const Compare& cmp = Compare()) : minSize(minSize), m_cmp(cmp) {}

    inline ~BTree() noexcept { delete m_root; }
    BTree(BTree const& other) = delete;
    auto operator=(BTree const& other) -> BTree& = delete;
    BTree(BTree&& other) noexcept = delete;
    auto operator=(BTree&& other) noexcept -> BTree& = delete;

    auto add(const T& key) -> void;

    [[nodiscard]] auto sliceView() const noexcept -> std::vector<std::vector<T>>;

    inline auto print() const noexcept -> void {
        if (m_root != nullptr) {
            m_root->print("");
        }
    }

 private:
    struct Node {
        inline Node(const std::vector<T>& keys, Node* parent, Node*& owner, const size_t& minSize, const Compare& cmp)
            : m_keys(keys), m_parent(parent), m_owner(owner), minSize(minSize), m_cmp(cmp) {}
        inline ~Node() {
            for (const auto& child : m_children) {
                delete child;
            }
        }

        Node(Node const& other) = delete;
        auto operator=(Node const& other) -> Node& = delete;
        Node(Node&& other) noexcept = delete;
        auto operator=(Node&& other) noexcept -> Node& = delete;

        auto add(const T& key) -> void;

        auto checkSplit() noexcept -> void;

        inline auto print(const std::string& step) const noexcept -> void {
            if (!m_children.empty()) {
                assert(m_children[0]->m_parent == this);
                m_children[0]->print(step + "  ");
            }
            for (size_t i = 0; i < m_keys.size(); ++i) {
                std::cout << step << m_keys[i] << '\n';
                if (!m_children.empty()) {
                    assert(m_children[i + 1]->m_parent == this);
                    m_children[i + 1]->print(step + "  ");
                }
            }
        }

        std::vector<T> m_keys;
        std::vector<Node*> m_children{};
        Node* m_parent;
        Node*& m_owner;
        const size_t minSize;
        const Compare& m_cmp;
    };

    const size_t minSize;
    Node* m_root{};
    const Compare& m_cmp;
};

template <class T, class Compare>
auto BTree<T, Compare>::add(const T& key) -> void {
    if (m_root == nullptr) {
        m_root = new Node({}, nullptr, m_root, minSize, m_cmp);
    }

    m_root->add(key);
}

template <class T, class Compare>
auto BTree<T, Compare>::sliceView() const noexcept -> std::vector<std::vector<T>> {
    if (m_root == nullptr) {
        return {};
    }

    std::vector<std::vector<T>> result{};

    std::queue<Node* const*> currSlice{{&m_root}};
    std::queue<Node* const*> nextSlice{};
    std::vector<T> nextResult{};
    Node* const* nextNode{};

    while (!currSlice.empty()) {
        while (!currSlice.empty()) {
            nextNode = currSlice.front();
            currSlice.pop();

            nextResult.insert(nextResult.end(), (*nextNode)->m_keys.begin(), (*nextNode)->m_keys.end());

            for (auto& child : (*nextNode)->m_children) {
                nextSlice.push(&child);
            }
        }

        std::swap(currSlice, nextSlice);

        result.push_back(nextResult);
        nextResult.clear();
    }

    return result;
}

template <class T, class Compare>
auto BTree<T, Compare>::Node::add(const T& key) -> void {
    size_t index{0};
    while (index != m_keys.size() && m_cmp(key, m_keys[index]) > 0) {
        ++index;
    }
    assert(index == m_keys.size() || m_cmp(key, m_keys[index]) != 0);

    if (m_children.empty()) {
        m_keys.insert(std::next(m_keys.begin(), index), key);

        checkSplit();
        return;
    }

    m_children[index]->add(key);
}

template <class T, class Compare>
auto BTree<T, Compare>::Node::checkSplit() noexcept -> void {
    if (m_keys.size() != minSize * 2 - 1) {
        return;
    }

    if (m_parent == nullptr) {
        m_parent = new Node({}, nullptr, m_owner, minSize, m_cmp);
        m_parent->m_children.push_back(this);
        m_owner = m_parent;
    }

    int parentIndex =
        std::distance(m_parent->m_children.begin(), std::find(m_parent->m_children.begin(), m_parent->m_children.end(), this));

    Node* newRightNode = new Node({std::next(m_keys.begin(), minSize), m_keys.end()}, m_parent, m_owner, minSize, m_cmp);

    m_parent->m_keys.insert(std::next(m_parent->m_keys.begin(), parentIndex), m_keys[minSize - 1]);
    m_keys.resize(minSize - 1);

    m_parent->m_children.insert(std::next(m_parent->m_children.begin(), parentIndex + 1), newRightNode);

    if (!m_children.empty()) {
        for (auto it = std::next(m_children.begin(), minSize); it != m_children.end(); ++it) {
            newRightNode->m_children.push_back(*it);
            (*it)->m_parent = newRightNode;
        }

        m_children.resize(minSize);
    }

    m_parent->checkSplit();
}

auto run(std::istream& input, std::ostream& output) -> void {
    int t{};
    int element{};

    input >> t;
    BTree<int> tree(t);

    while (input >> element) {
        tree.add(element);
    }
    // tree.print();

    for (const auto& slice : tree.sliceView()) {
        std::cout << slice.size() << '\n';
        std::copy(slice.begin(), slice.end(), std::ostream_iterator<int>(output, " "));
        output << '\n';
    }
}

auto test() -> void {
    {
        std::ifstream input;
        input.open("3-in.a");
        std::stringstream output;
        std::ifstream outputExp;
        outputExp.open("3-out.a");
        run(input, output);
        // std::cout << ">>> " << output.str() << std::endl;
        int a{};
        int b{};
        while ((output >> a) && (outputExp >> b)) {
            assert(a == b);
        }
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2\n 0 1 2 3 4 5 6 7 8 9";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "3 \n1 5 7 \n0 2 4 6 8 9 \n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n0 1 2 3 4 5 6 7 8 9";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "3 \n0 1 2 4 5 6 7 8 9 \n");
    }
}

auto main() -> int {
    // run(std::cin, std::cout);
    test();
    return 0;
}
