#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <class T> class CompareDefault {
public:
  int operator()(const T &a, const T &b) const {
    return a > b ? 1 : (a == b ? 0 : -1);
  }
};

template <class T, class Compare = CompareDefault<T>> class AvlTree {
public:
  inline AvlTree(const Compare &cmp = Compare()) : cmp_m(cmp) {}

  inline ~AvlTree() { delete root_m; }
  AvlTree(AvlTree const &other) = delete;
  auto operator=(AvlTree const &other) -> AvlTree & = delete;
  AvlTree(AvlTree &&other) noexcept = delete;
  auto operator=(AvlTree &&other) noexcept -> AvlTree & = delete;

  auto add(const T &key) -> void;
  auto remove(const T &key) -> void;
  auto exists(const T &key) -> bool;
  auto next(const T &key) -> const T *;
  auto prev(const T &key) -> const T *;

private:
  struct Node {
    inline Node(const T &key, Node *parent, Node *&owner, const Compare &cmp)
        : key_m(key), parent_m(parent), owner_m(owner), cmp_m(cmp) {}
    inline ~Node() {
      delete left_m;
      delete right_m;
    }
    Node(Node const &other) = delete;
    auto operator=(Node const &other) -> Node & = delete;
    Node(Node &&other) noexcept = delete;
    auto operator=(Node &&other) noexcept -> Node & = delete;

    auto correctHeigth() -> void;
    auto correctWeight() -> void;

    auto add(const T &key) -> void;
    auto remove(const T &key) -> void;
    auto exists(const T &key) -> bool;
    auto next(const T &key) -> const T *;
    auto prev(const T &key) -> const T *;

    auto balanceTree() -> void;
    auto smallRightRotate() -> void;
    auto smallLeftRotate() -> void;
    auto bigRightRotate() -> void;
    auto bigLeftRotate() -> void;

    inline auto leftHeight() const -> int {
      return left_m ? left_m->heigth_m : 0;
    }
    inline auto rightHeight() const -> int {
      return right_m ? right_m->heigth_m : 0;
    }
    inline auto leftWeight() const -> int {
      return left_m ? left_m->weight_m : 0;
    }
    inline auto rightWeight() const -> int {
      return right_m ? right_m->weight_m : 0;
    }

    const T key_m;
    Node *parent_m;
    Node *left_m{};
    Node *right_m{};
    int heigth_m{1};
    int weight_m{1};
    Node *&owner_m;
    const Compare &cmp_m;
  };

  Node *root_m{};
  const Compare &cmp_m;
};

template <class T, class Compare>
auto AvlTree<T, Compare>::add(const T &key) -> void {
  if (root_m == nullptr) {
    root_m = new Node(key, nullptr, root_m, cmp_m);
    return;
  }

  root_m->add(key);
}

template <class T, class Compare>
auto AvlTree<T, Compare>::remove(const T &key) -> void {
  if (root_m) {
    root_m->remove(key);
  }
}

template <class T, class Compare>
auto AvlTree<T, Compare>::exists(const T &key) -> bool {
  if (root_m == nullptr) {
    return false;
  }

  return root_m->exists(key);
}

template <class T, class Compare>
auto AvlTree<T, Compare>::next(const T &key) -> const T * {
  if (root_m == nullptr) {
    return nullptr;
  }

  return root_m->next(key);
}
template <class T, class Compare>
auto AvlTree<T, Compare>::prev(const T &key) -> const T * {
  if (root_m == nullptr) {
    return nullptr;
  }

  return root_m->prev(key);
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::correctHeigth() -> void {
  heigth_m = std::max(leftHeight(), rightHeight()) + 1;
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::correctWeight() -> void {
  weight_m = leftWeight() + rightWeight() + 1;
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::add(const T &key) -> void {
  if (cmp_m(key_m, key) == 0) {
    return;
  }

  if (cmp_m(key, key_m) < 0) {
    if (left_m) {
      left_m->add(key);
      return;
    }

    left_m = new Node(key, this, owner_m, cmp_m);
    balanceTree();
    return;
  }

  if (right_m) {
    right_m->add(key);
    return;
  }

  right_m = new Node(key, this, owner_m, cmp_m);
  balanceTree();
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::remove(const T &key) -> void {
  if (cmp_m(key_m, key) > 0) {
    if (left_m) {
      left_m->remove(key);
    }
    return;
  }
  if (cmp_m(key_m, key) < 0) {
    if (right_m) {
      right_m->remove(key);
    }
    return;
  }

  if (left_m == nullptr) {
    if (parent_m == nullptr) {
      owner_m = right_m;
    } else if (parent_m->left_m == this) {
      parent_m->left_m = right_m;
    } else {
      parent_m->right_m = right_m;
    }

    if (right_m) {
      right_m->parent_m = parent_m;

      right_m->balanceTree();
    } else if (parent_m) {
      parent_m->balanceTree();
    }

    right_m = nullptr;
    delete this;
    return;
  }

  if (right_m == nullptr) {
    if (parent_m == nullptr) {
      owner_m = left_m;
    } else if (parent_m->left_m == this) {
      parent_m->left_m = left_m;
    } else {
      parent_m->right_m = left_m;
    }

    left_m->parent_m = parent_m;

    left_m->balanceTree();

    left_m = nullptr;
    delete this;
    return;
  }

  Node *minRight = right_m;
  while (minRight->left_m) {
    minRight = minRight->left_m;
  }

  if (parent_m == nullptr) {
    owner_m = minRight;
  } else if (parent_m->left_m == this) {
    parent_m->left_m = minRight;
  } else {
    parent_m->right_m = minRight;
  }
  minRight->parent_m = parent_m;

  left_m->parent_m = minRight;
  minRight->left_m = left_m;

  if (minRight != right_m) {
    right_m->parent_m = minRight;
    minRight->right_m = right_m;
  }

  minRight->balanceTree();

  left_m = nullptr;
  right_m = nullptr;
  delete this;
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::exists(const T &key) -> bool {
  if (cmp_m(key_m, key) == 0) {
    return true;
  }

  if (cmp_m(key, key_m) < 0) {
    return left_m && left_m->exists(key);
  }

  return right_m && right_m->exists(key);
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::next(const T &key) -> const T * {
  const T *nextResult{};
  if (cmp_m(key_m, key) > 0) {
    if (left_m && (nextResult = left_m->next(key)) && *nextResult > key_m) {
      return nextResult;
    }

    return &key_m;
  }

  return (right_m ? right_m->next(key) : nullptr);
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::prev(const T &key) -> const T * {
  const T *nextResult{};
  if (cmp_m(key_m, key) < 0) {
    if (right_m && (nextResult = right_m->next(key)) && *nextResult < key_m) {
      return nextResult;
    }

    return &key_m;
  }

  return (left_m ? left_m->next(key) : nullptr);
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::balanceTree() -> void {
  correctHeigth();

  if (left_m && leftHeight() == rightHeight() + 2 &&
      left_m->leftHeight() >= left_m->rightHeight()) {
    smallRightRotate();
  } else if (right_m && leftHeight() + 2 == rightHeight() &&
             right_m->leftHeight() <= right_m->rightHeight()) {
    smallLeftRotate();
  } else if (left_m && leftHeight() == rightHeight() + 2 &&
             left_m->leftHeight() < left_m->rightHeight()) {
    bigRightRotate();
  } else if (right_m && leftHeight() + 2 == rightHeight() &&
             right_m->leftHeight() > right_m->rightHeight()) {
    bigLeftRotate();
  }

  correctHeigth();
  correctWeight();

  if (parent_m) {
    parent_m->balanceTree();
  }

  correctWeight();
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::smallRightRotate() -> void {
  if (parent_m == nullptr) {
    owner_m = left_m;
  } else if (parent_m->left_m == this) {
    parent_m->left_m = left_m;
  } else {
    parent_m->right_m = left_m;
  }
  left_m->parent_m = parent_m;
  parent_m = left_m;
  left_m = parent_m->right_m;
  if (left_m) {
    left_m->parent_m = this;
  }
  parent_m->right_m = this;
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::smallLeftRotate() -> void {
  if (parent_m == nullptr) {
    owner_m = right_m;
  } else if (parent_m->left_m == this) {
    parent_m->left_m = right_m;
  } else {
    parent_m->right_m = right_m;
  }
  right_m->parent_m = parent_m;
  parent_m = right_m;
  right_m = parent_m->left_m;
  if (right_m) {
    right_m->parent_m = this;
  }
  parent_m->left_m = this;
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::bigRightRotate() -> void {
  left_m->smallLeftRotate();
  smallRightRotate();
}

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::bigLeftRotate() -> void {
  right_m->smallRightRotate();
  smallLeftRotate();
}

void run(std::istream &input, std::ostream &output) {
  std::string command{};
  int argument{};
  const int *result{};

  AvlTree<int> tree{};

  while (input >> command >> argument) {
    if (command == "insert") {
      tree.add(argument);
    } else if (command == "delete") {
      tree.remove(argument);
    } else if (command == "exists") {
      output << (tree.exists(argument) ? "true" : "false") << '\n';
    } else if (command == "next") {
      result = tree.next(argument);
      if (result) {
        output << *result << '\n';
      } else {
        output << "none\n";
      }
    } else if (command == "prev") {
      result = tree.prev(argument);
      if (result) {
        output << *result << '\n';
      } else {
        output << "none\n";
      }
    }
  }
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "insert 2\ninsert 5\ninsert 3\nexists 2\nexists 4\nnext 4\nprev "
             "4\ndelete 5\nnext 4\nprev 4";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "true\nfalse\n5\n3\nnone\n3\n");
  }
}

int main() {
  run(std::cin, std::cout);
  //   test();
  return 0;
}
