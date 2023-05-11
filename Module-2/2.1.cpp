#include <cassert>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

template <class T> class CompareDefault {
public:
  int operator()(const T &a, const T &b) const { return a > b ? 1 : (a == b ? 0 : -1); }
};

template <class T, class Compare = CompareDefault<T>> class BinaryTree {
public:
  inline BinaryTree(const Compare &cmp) : cmp_m(cmp) {}

  ~BinaryTree();
  BinaryTree(BinaryTree const &other) = delete;
  auto operator=(BinaryTree const &other) -> BinaryTree & = delete;
  BinaryTree(BinaryTree &&other) noexcept = delete;
  auto operator=(BinaryTree &&other) noexcept -> BinaryTree & = delete;

  auto add(const T &key) -> void;
  auto getKeysInOrder() const -> std::vector<T>;

private:
  struct Node {
    inline Node(const T &key, Node *&parent) : key_m(key), parent_m(parent) {}
    Node(Node const &other) = delete;
    auto operator=(Node const &other) -> Node & = delete;
    Node(Node &&other) noexcept = delete;
    auto operator=(Node &&other) noexcept -> Node & = delete;

    T key_m;
    Node *&parent_m;
    Node *left_m{};
    Node *right_m{};
  };

  Node *root_m{};
  Compare cmp_m{};
};

template <class T, class Compare> BinaryTree<T, Compare>::~BinaryTree() {
  std::queue<Node *> nodes{{root_m}};
  Node *next{};

  while (!nodes.empty()) {
    next = nodes.front();
    nodes.pop();
    if (next != nullptr) {
      nodes.push(next->left_m);
      nodes.push(next->right_m);
      delete next;
    }
  }
}

template <class T, class Compare> auto BinaryTree<T, Compare>::add(const T &key) -> void {
  Node **curNode = &root_m;
  Node **parentNode = curNode;

  while (*curNode != nullptr) {
    parentNode = curNode;
    if (cmp_m((*curNode)->key_m, key) <= 0) {
      curNode = &(*curNode)->right_m;
    } else {
      curNode = &(*curNode)->left_m;
    }
  }

  *curNode = new Node(key, *parentNode);
}

template <class T, class Compare> auto BinaryTree<T, Compare>::getKeysInOrder() const -> std::vector<T> {
  std::vector<T> keys{};

  Node *lastNode{};
  Node *curNode = root_m;
  Node *nextNode{};

  while (curNode != lastNode) {
    if (lastNode == curNode->left_m) {
      keys.push_back(curNode->key_m);
      if (curNode->right_m == nullptr) {
        nextNode = curNode->parent_m;
      } else {
        nextNode = curNode->right_m;
      }
    } else if (lastNode == curNode->right_m && lastNode != nullptr) {
      nextNode = curNode->parent_m;
    } else if (curNode->left_m != nullptr) {
      nextNode = curNode->left_m;
    } else if (curNode->right_m != nullptr) {
      keys.push_back(curNode->key_m);
      nextNode = curNode->right_m;
    } else {
      keys.push_back(curNode->key_m);
      nextNode = curNode->parent_m;
    }

    lastNode = curNode;
    curNode = nextNode;
  }

  return keys;
}

void run(std::istream &input, std::ostream &output) {
  CompareDefault<int> cmp{};
  BinaryTree<int, CompareDefault<int>> tree{cmp};

  int n{};
  int key{};

  input >> n;
  for (int i = 0; i < n; ++i) {
    input >> key;
    tree.add(key);
  }

  for (const auto &key : tree.getKeysInOrder()) {
    output << key << ' ';
  }
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "10 9 10 4 3 2 7 8 5 1 6";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "1 2 3 4 5 6 7 8 9 10 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "10 4 7 5 10 3 2 8 1 6 9";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "1 2 3 4 5 6 7 8 9 10 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "10 2 3 6 1 10 8 4 9 7 5";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "1 2 3 4 5 6 7 8 9 10 ");
  }
}

int main() {
  run(std::cin, std::cout);
  // test();
  return 0;
}