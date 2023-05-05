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
  auto findMinHeidth() -> int;

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

template <class T> auto BinaryTree<T>::findMinHeidth() -> int {
  if (root_m == nullptr) {
    return 0;
  }

  int heidth{1};
  std::queue<Node *> sliceNodes{{root_m}};
  Node *nextNode{};

  while (!sliceNodes.empty()) {
    for (int c = sliceNodes.size(); c > 0; --c) {
      nextNode = sliceNodes.front();
      sliceNodes.pop();

      if (nextNode->left_m) {
        sliceNodes.push(nextNode->left_m);
      } else {
        return heidth;
      }

      if (nextNode->right_m) {
        sliceNodes.push(nextNode->right_m);
      } else {
        return heidth;
      }
    }

    ++heidth;
  }

  return heidth;
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

  output << tree.findMinHeidth();
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "2 1 3";
    run(input, output);
    std::cout << ">>> " << output.str() << '\n';
    assert(output.str() == "2");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "3 2 1 4";
    run(input, output);
    std::cout << ">>> " << output.str() << '\n';
    assert(output.str() == "2");
  }
}

int main() {
  //   run(std::cin, std::cout);
  test();
  return 0;
}
