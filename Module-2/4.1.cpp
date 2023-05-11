#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <class T> class CompareDefault {
public:
  int operator()(const T &a, const T &b) const { return a > b ? 1 : (a == b ? 0 : -1); }
};

template <class T, class Compare = CompareDefault<T>> class AvlTree {
public:
  inline AvlTree(const Compare &cmp = Compare()) : cmp_m(cmp) {}

  inline ~AvlTree() { delete root_m; }
  AvlTree(AvlTree const &other) = delete;
  auto operator=(AvlTree const &other) -> AvlTree & = delete;
  AvlTree(AvlTree &&other) noexcept = delete;
  auto operator=(AvlTree &&other) noexcept -> AvlTree & = delete;

  auto add(const T &key) -> int;
  auto remove(int index) -> void;

  auto print() -> void {
    if (root_m) {
      root_m->print("");
    }
  }

private:
  struct Node {
    inline Node(const T &key, Node *parent, Node *&owner, const Compare &cmp) : key_m(key), parent_m(parent), owner_m(owner), cmp_m(cmp) {}
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

    auto add(const T &key) -> int;
    auto remove(int index) -> void;

    auto check() -> void {
      if (!(left_m == nullptr || left_m->parent_m == this)) {
        assert(false);
      }
      if (left_m) {
        left_m->check();
      }
      if (!(right_m == nullptr || right_m->parent_m == this)) {
        assert(false);
      }
      if (right_m) {
        right_m->check();
      }
    }

    auto print(const std::string &indent) -> void {
      check();

      if (left_m) {
        left_m->print(indent + "    ");
      }

      std::cout << indent << key_m << '\n';

      if (right_m) {
        right_m->print(indent + "    ");
      }
    }

    auto balanceTree() -> void;
    auto smallRightRotate() -> void;
    auto smallLeftRotate() -> void;
    auto bigRightRotate() -> void;
    auto bigLeftRotate() -> void;

    inline auto leftHeight() const -> int { return left_m ? left_m->heigth_m : 0; }
    inline auto rightHeight() const -> int { return right_m ? right_m->heigth_m : 0; }
    inline auto leftWeight() const -> int { return left_m ? left_m->weight_m : 0; }
    inline auto rightWeight() const -> int { return right_m ? right_m->weight_m : 0; }

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

template <class T, class Compare> auto AvlTree<T, Compare>::add(const T &key) -> int {
  if (root_m == nullptr) {
    root_m = new Node(key, nullptr, root_m, cmp_m);
    return 0;
  }

  return root_m->add(key);
}

template <class T, class Compare> auto AvlTree<T, Compare>::remove(int index) -> void {
  assert(root_m && 0 <= index && index < root_m->weight_m);

  root_m->remove(index);
}

template <class T, class Compare> auto AvlTree<T, Compare>::Node::correctHeigth() -> void {
  std::cout << "correctHeigth\n";
  check();

  heigth_m = std::max(leftHeight(), rightHeight()) + 1;
}

template <class T, class Compare> auto AvlTree<T, Compare>::Node::correctWeight() -> void {
  std::cout << "correctWeight\n";
  check();

  weight_m = leftWeight() + rightWeight() + 1;
}

template <class T, class Compare> auto AvlTree<T, Compare>::Node::add(const T &key) -> int {
  std::cout << "add\n";
  check();

  assert(cmp_m(key_m, key) != 0);

  if (cmp_m(key, key_m) < 0) {
    if (left_m) {
      return left_m->add(key) + rightWeight() + 1;
    }

    left_m = new Node(key, this, owner_m, cmp_m);
    int ret = rightWeight() + 1;
    balanceTree();
    return ret;
  }

  if (right_m) {
    return right_m->add(key);
  }

  right_m = new Node(key, this, owner_m, cmp_m);
  balanceTree();

  return 0;
}

template <class T, class Compare> auto AvlTree<T, Compare>::Node::remove(int index) -> void {
  std::cout << "remove\n";
  check();

  if (index > rightWeight()) {
    return left_m->remove(index - rightWeight() - 1);
  }
  if (index < rightWeight()) {
    return right_m->remove(index);
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

  left_m->parent_m = minRight;
  minRight->left_m = left_m;

  if (minRight != right_m) {
    right_m->parent_m = minRight;
    minRight->right_m = right_m;
  }

  if (minRight->parent_m->left_m == minRight) {
    minRight->parent_m->left_m = nullptr;
  } else {
    minRight->parent_m->right_m = nullptr;
  }
  minRight->parent_m = parent_m;

  minRight->balanceTree();

  left_m = nullptr;
  right_m = nullptr;
  delete this;
}

template <class T, class Compare> auto AvlTree<T, Compare>::Node::balanceTree() -> void {
  std::cout << "balanceTree\n";
  check();

  correctHeigth();

  if (left_m && leftHeight() == rightHeight() + 2 && left_m->leftHeight() >= left_m->rightHeight()) {
    smallRightRotate();
  } else if (right_m && leftHeight() + 2 == rightHeight() && right_m->leftHeight() <= right_m->rightHeight()) {
    smallLeftRotate();
  } else if (left_m && leftHeight() == rightHeight() + 2 && left_m->leftHeight() < left_m->rightHeight()) {
    bigRightRotate();
  } else if (right_m && leftHeight() + 2 == rightHeight() && right_m->leftHeight() > right_m->rightHeight()) {
    bigLeftRotate();
  }

  correctHeigth();
  correctWeight();

  if (parent_m) {
    parent_m->balanceTree();
  }

  correctWeight();
}

template <class T, class Compare> auto AvlTree<T, Compare>::Node::smallRightRotate() -> void {
  std::cout << "smallRightRotate\n";
  check();

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

template <class T, class Compare> auto AvlTree<T, Compare>::Node::smallLeftRotate() -> void {
  std::cout << "smallLeftRotate\n";
  check();

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

template <class T, class Compare> auto AvlTree<T, Compare>::Node::bigRightRotate() -> void {
  std::cout << "bigRightRotate\n";
  check();

  left_m->smallLeftRotate();
  smallRightRotate();
}

template <class T, class Compare> auto AvlTree<T, Compare>::Node::bigLeftRotate() -> void {
  std::cout << "bigLeftRotate\n";
  check();

  right_m->smallRightRotate();
  smallLeftRotate();
}

void run(std::istream &input, std::ostream &output) {
  int n{};
  int command{};
  int argument{};

  AvlTree<int> tree{};

  input >> n;
  for (int i = 0; i < n; ++i) {
    input >> command >> argument;
    std::cout << "\n========================================================\n"
              << command << ' ' << argument << "\n========================================================\n";
    if (command == 1) {
      output << tree.add(argument) << '\n';
    } else {
      tree.remove(argument);
    }
    tree.print();
  }
}

void test() {
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
    assert(output.str() == "");
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

int main() {
  // run(std::cin, std::cout);
  test();
  return 0;
}
