#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

template <class T>
class CompareDefault {
 public:
    [[nodiscard]] inline auto operator()(const T& a, const T& b) const noexcept -> int { return a > b ? 1 : (a == b ? 0 : -1); }
};

template <class T, class Compare = CompareDefault<T>>
class AvlTree {
 public:
    inline AvlTree(const Compare& cmp = Compare()) noexcept : m_root(new Node(cmp)), m_cmp(cmp) {}

    inline ~AvlTree() noexcept { delete m_root; }
    AvlTree(AvlTree const& other) = delete;
    auto operator=(AvlTree const& other) -> AvlTree& = delete;
    AvlTree(AvlTree&& other) noexcept = delete;
    auto operator=(AvlTree&& other) noexcept -> AvlTree& = delete;

    [[nodiscard]] auto add(const T& key) -> int;
    auto remove(int index) -> void;

    inline auto print() const noexcept -> void { m_root->print(""); }

 private:
    struct Node {
        inline Node(const Compare& cmp) : isLeaf(true), heigth(0), weight(0), cmp(cmp) {}
        inline Node(const T& key, const Compare& cmp) : key(key), left(new Node(cmp)), right(new Node(cmp)), cmp(cmp) {}
        inline ~Node() {
            delete left;
            delete right;
        }
        Node(Node const& other) = delete;
        auto operator=(Node const& other) -> Node& = delete;
        Node(Node&& other) noexcept = delete;
        auto operator=(Node&& other) noexcept -> Node& = delete;

        auto correctHeigth() noexcept -> void;
        auto correctWeight() noexcept -> void;

        [[nodiscard]] auto find(const T& key) const -> int;
        [[nodiscard]] auto add(const T& key) -> int;
        auto remove(int index) -> void;

        inline auto print(const std::string& indent) noexcept -> void {
            if (left) {
                left->print(indent + "    ");
            }

            std::cout << indent << key << '\n';

            if (right) {
                right->print(indent + "    ");
            }
        }

        auto balanceTree() noexcept -> void;
        auto smallLeftRotate() noexcept -> void;
        auto smallRightRotate() noexcept -> void;
        auto bigLeftRotate() noexcept -> void;
        auto bigRightRotate() noexcept -> void;

        [[nodiscard]] inline auto leftHeight() const noexcept -> int { return !left->isLeaf ? left->heigth : 0; }
        [[nodiscard]] inline auto rightHeight() const noexcept -> int { return !right->isLeaf ? right->heigth : 0; }
        [[nodiscard]] inline auto balanceFactor() const noexcept -> int { return leftHeight() - rightHeight(); }

        [[nodiscard]] inline auto leftWeight() const noexcept -> int { return !left->isLeaf ? left->weight : 0; }
        [[nodiscard]] inline auto rightWeight() const noexcept -> int { return !right->isLeaf ? right->weight : 0; }

        inline auto swap(Node* firstNode, Node* secondNode) -> void {
            std::swap(firstNode->isLeaf, secondNode->isLeaf);
            std::swap(firstNode->key, secondNode->key);
            std::swap(firstNode->left, secondNode->left);
            std::swap(firstNode->right, secondNode->right);
            std::swap(firstNode->heigth, secondNode->heigth);
            std::swap(firstNode->weight, secondNode->weight);
        }

        bool isLeaf{false};
        T key{};
        Node* left{nullptr};
        Node* right{nullptr};
        int heigth{1};
        int weight{1};
        const Compare& cmp;
    };

    Node* m_root{nullptr};
    const Compare& m_cmp;
};

template <class T, class Compare>
auto AvlTree<T, Compare>::add(const T& key) -> int {
    if (m_root->isLeaf) {
        delete m_root;
        m_root = new Node(key, m_cmp);
        return 0;
    }

    return m_root->add(key);
}

template <class T, class Compare>
auto AvlTree<T, Compare>::remove(int index) -> void {
    m_root->remove(index);
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::correctHeigth() noexcept -> void {
    if (isLeaf) {
        return;
    }

    heigth = std::max(leftHeight(), rightHeight()) + 1;
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::correctWeight() noexcept -> void {
    if (isLeaf) {
        return;
    }

    weight = leftWeight() + rightWeight() + 1;
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::find(const T& findKey) const -> int {
    assert(!isLeaf);

    if (cmp(findKey, key) < 0) {
        return left->find(findKey) + rightWeight() + 1;
    }
    if (cmp(findKey, key) > 0) {
        return right->find(findKey);
    }

    return rightWeight();
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::add(const T& addKey) -> int {
    assert(cmp(addKey, key) != 0);

    if (isLeaf) {
        auto* newNode = new Node(addKey, cmp);
        swap(this, newNode);
        delete newNode;
        return 0;
    }

    int resInd{};

    if (cmp(addKey, key) < 0) {
        resInd = left->add(addKey) + rightWeight() + 1;
    } else {
        resInd = right->add(addKey);
    }
    balanceTree();
    return resInd;
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::remove(int index) -> void {
    assert(!isLeaf);

    if (index > rightWeight()) {
        left->remove(index - rightWeight() - 1);
        return balanceTree();
    }
    if (index < rightWeight()) {
        right->remove(index);
        return balanceTree();
    }

    Node* oldLeft = left;
    Node* oldRight = right;

    if (oldLeft->isLeaf) {
        swap(this, oldRight);
        oldRight->right = nullptr;
        delete oldRight;
        balanceTree();
        return;
    }

    if (oldRight->isLeaf) {
        swap(this, oldLeft);
        oldLeft->left = nullptr;
        delete oldLeft;
        balanceTree();
        return;
    }

    std::stack<Node*> mustBalansedNodes{{this}};
    Node* minRight = right;
    while (!minRight->left->isLeaf) {
        mustBalansedNodes.push(minRight);
        minRight = minRight->left;
    }

    Node* minRightRight = minRight->right;

    key = minRight->key;
    swap(minRight, minRightRight);
    minRightRight->right = new Node(cmp);
    delete minRightRight;

    while (!mustBalansedNodes.empty()) {
        mustBalansedNodes.top()->balanceTree();
        mustBalansedNodes.pop();
    }
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::balanceTree() noexcept -> void {
    if (isLeaf) {
        return;
    }

    correctHeigth();
    correctWeight();

    if (!right->isLeaf && balanceFactor() < -1) {
        if (right->balanceFactor() < 0) {
            smallLeftRotate();
        } else {
            bigLeftRotate();
        }
    } else if (!left->isLeaf && balanceFactor() > 1) {
        if (left->balanceFactor() > 0) {
            smallRightRotate();
        } else {
            bigRightRotate();
        }
    }
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::smallLeftRotate() noexcept -> void {
    Node* oldRight = right;
    swap(this, oldRight);
    std::swap(left, oldRight->right);
    left = oldRight;

    left->correctHeigth();
    left->correctWeight();
    correctHeigth();
    correctWeight();
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::smallRightRotate() noexcept -> void {
    Node* oldLeft = left;
    swap(this, oldLeft);
    std::swap(right, oldLeft->left);
    right = oldLeft;

    right->correctHeigth();
    right->correctWeight();
    correctHeigth();
    correctWeight();
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::bigLeftRotate() noexcept -> void {
    right->smallRightRotate();
    smallLeftRotate();
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::bigRightRotate() noexcept -> void {
    left->smallLeftRotate();
    smallRightRotate();
}

auto run(std::istream& input, std::ostream& output) -> void {
    int n{};
    int command{};
    int argument{};

    AvlTree<int> tree{};

    input >> n;
    for (int i = 0; i < n; ++i) {
        input >> command >> argument;
        // std::cout << "\n========================================================\n"
        //           << command << ' ' << argument << "\n========================================================\n";
        if (command == 1) {
            output << tree.add(argument) << '\n';
        } else {
            tree.remove(argument);
        }
        // tree.print();
    }
}

auto test() -> void {
    {
        std::ifstream input;
        input.open("4.1-in.a");
        std::stringstream output;
        std::ifstream outputExp;
        outputExp.open("4.1-out.a");
        run(input, output);
        int a{};
        int b{};
        while ((output >> a) && (outputExp >> b)) {
            assert(a == b);
        }
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5\n1 100\n1 200\n1 50\n2 1\n1 150\n";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "0\n0\n2\n1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "75\n1 41\n1 18467\n2 0\n1 26500\n1 19169\n2 1\n1 11478\n1 29358\n2 "
                 "2\n1 24464\n1 5705\n2 0\n1 23281\n1 16827\n2 1\n1 491\n1 2995\n2 "
                 "0\n1 4827\n1 5436\n2 7\n1 14604\n1 3902\n2 0\n1 292\n1 12382\n2 "
                 "1\n1 18716\n1 19718\n2 7\n1 5447\n1 21726\n2 11\n1 11538\n1 "
                 "1869\n2 9\n1 25667\n1 26299\n2 11\n1 9894\n1 28703\n2 6\n1 "
                 "31322\n1 30333\n2 9\n1 4664\n1 15141\n2 10\n1 28253\n1 6868\n2 "
                 "5\n1 27644\n1 32662\n2 1\n1 20037\n1 12859\n2 3\n1 9741\n1 "
                 "27529\n2 1\n1 12316\n1 3035\n2 14\n1 1842\n1 288\n2 22\n1 9040\n1 "
                 "8942\n2 16\n1 22648\n1 27446\n2 5\n1 15890\n1 6729\n2 8";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() ==
               "0\n0\n0\n1\n1\n0\n2\n3\n2\n3\n4\n4\n3\n3\n2\n6\n8\n2\n0\n0\n5\n0\n5\n11\n0\n0\n8\n0\n0\n1\n13\n8\n3\n11\n4\n0\n"
               "7\n11\n12\n4\n12\n19\n20\n22\n14\n15\n5\n4\n10\n18\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10\n1 62\n2 0\n1 292\n1 397\n1 127\n2 2\n1 731\n2 1\n1 653\n1 "
                 "379\n";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "0\n0\n0\n2\n0\n1\n2\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10\n1 574\n2 0\n1 691\n1 655\n1 674\n1 398\n2 2\n1 916\n2 0\n1 "
                 "183\n";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "0\n0\n1\n1\n3\n0\n3\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10\n1 239\n1 675\n1 559\n2 1\n1 4\n1 968\n1 593\n2 4\n2 1\n1 405\n";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "0\n0\n1\n2\n0\n2\n2\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10\n1 451\n1 962\n2 0\n2 0\n1 15\n1 507\n2 0\n1 915\n2 1\n1 893\n";
        run(input, output);
        std::cout << ">>> " << output.str() << std::endl;
        assert(output.str() == "0\n0\n0\n0\n0\n1\n");
    }
}

auto main() -> int {
    run(std::cin, std::cout);
    // test();
    return 0;
}
