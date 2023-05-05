#include <cassert>
#include <iostream>
#include <queue>
#include <sstream>

template <class T> class BinaryTree {
public:
  BinaryTree() = default;

  ~BinaryTree();
  BinaryTree(BinaryTree const &other) = delete;
  auto operator=(BinaryTree const &other) -> BinaryTree & = delete;
  BinaryTree(BinaryTree &&other) noexcept = delete;
  auto operator=(BinaryTree &&other) noexcept -> BinaryTree & = delete;

  auto add(const T &key) -> void;
  auto checkForEqual() -> bool;

private:
  struct Node {
    inline Node(const T &key) : key_m(key) {}

    auto add(const T &key) -> void;

    T key_m;
    Node *left_m{};
    Node *right_m{};
  };

  Node *root_m{};
};

template <class T> BinaryTree<T>::~BinaryTree() {
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

template <class T> auto BinaryTree<T>::add(const T &key) -> void {
  if (root_m == nullptr) {
    root_m = new Node(key);
    return;
  }

  root_m->add(key);
}

template <class T> auto BinaryTree<T>::checkForEqual() -> bool {
  if (root_m == nullptr) {
    return true;
  }

  T rootValue = root_m->key_m;
  std::queue<Node *> nodes{{root_m}};
  Node *next{};

  while (!nodes.empty()) {
    next = nodes.front();
    nodes.pop();
    if (next != nullptr) {
      if (rootValue != next->key_m) {
        return false;
      }
      nodes.push(next->left_m);
      nodes.push(next->right_m);
    }
  }

  return true;
}

template <class T> auto BinaryTree<T>::Node::add(const T &key) -> void {
  if (key < key_m) {
    if (left_m == nullptr) {
      left_m = new Node(key);
    } else {
      left_m->add(key);
    }
    return;
  }

  if (right_m == nullptr) {
    right_m = new Node(key);
  } else {
    right_m->add(key);
  }
}

void run(std::istream &input, std::ostream &output) {
  int value{};
  BinaryTree<int> tree{};

  while (input >> value) {
    tree.add(value);
  }

  output << (tree.checkForEqual() ? 1 : 0);
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "2 3 1";
    run(input, output);
    std::cout << ">>> " << output.str() << '\n';
    assert(output.str() == "0");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "1 1 1";
    run(input, output);
    std::cout << ">>> " << output.str() << '\n';
    assert(output.str() == "1");
  }
}

int main() {
  run(std::cin, std::cout);
  //   test();
  return 0;
}
