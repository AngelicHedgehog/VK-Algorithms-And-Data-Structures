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

  auto add(const T &key) -> int;
  auto remove(int index) -> void;

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

    auto add(const T &key) -> int;
    auto remove(int index) -> void;

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

    T key_m;
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
auto AvlTree<T, Compare>::add(const T &key) -> int {
  if (root_m == nullptr) {
    root_m = new Node(key, nullptr, root_m, cmp_m);
    return 0;
  }

  int res = root_m->add(key);
  // root_m->print();
  return res;
}

template <class T, class Compare>
auto AvlTree<T, Compare>::remove(int index) -> void {
  assert(root_m && 0 <= index && index < root_m->weight_m);

  root_m->remove(index);
  // root_m->print();
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
auto AvlTree<T, Compare>::Node::add(const T &key) -> int {
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

template <class T, class Compare>
auto AvlTree<T, Compare>::Node::remove(int index) -> void {
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
  int n{};
  int command{};
  int argument{};

  AvlTree<int> tree{};

  input >> n;
  for (int i = 0; i < n; ++i) {
    input >> command >> argument;
    if (command == 1) {
      output << tree.add(argument) << '\n';
    } else {
      tree.remove(argument);
    }
  }
}

void test() {
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
    input
        << "10\n1 239\n1 675\n1 559\n2 1\n1 4\n1 968\n1 593\n2 4\n2 1\n1 405\n";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "0\n0\n1\n2\n0\n2\n2\n");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input
        << "10\n1 451\n1 962\n2 0\n2 0\n1 15\n1 507\n2 0\n1 915\n2 1\n1 893\n";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "0\n0\n0\n0\n0\n1\n");
  }
}

int main() {
  run(std::cin, std::cout);
  // test();
  return 0;
}

// def gen(n):
//     inp = str(n) + '\n'
//     out = ''
//     seq = []
//     for _ in range(n):
//         if len(seq):
//             cmd = random.choice([1, 2])
//         else:
//             cmd = 1
//         inp += str(cmd) + ' '
//         if cmd == 1:
//             arg = random.choice(list(set(range(1000)) - set(seq)))
//             seq.append(arg)
//             seq.sort(key=lambda x: -x)
//             out += str(seq.index(arg)) + '\n'
//         else:
//             arg = random.choice(range(len(seq)))
//             seq.pop(arg)
//         inp += str(arg) + '\n'
//     return inp, out