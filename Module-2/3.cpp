#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
// #include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

template <class T> class CompareDefault {
public:
  int operator()(const T &a, const T &b) const {
    return a > b ? 1 : (a == b ? 0 : -1);
  }
};

template <class T, class Compare = CompareDefault<T>> class BTree {
public:
  inline BTree(const size_t minSize, const Compare &cmp = Compare())
      : minSize_m(minSize), cmp_m(cmp) {}

  inline ~BTree() { delete root_m; }
  BTree(BTree const &other) = delete;
  auto operator=(BTree const &other) -> BTree & = delete;
  BTree(BTree &&other) noexcept = delete;
  auto operator=(BTree &&other) noexcept -> BTree & = delete;

  auto add(const T &key) -> void;

  auto sliceView() const -> std::vector<std::vector<T>>;

  inline auto print() -> void {
    if (root_m != nullptr) {
      root_m->print("");
    }
  }

private:
  struct Node {
    inline Node(const std::vector<T> &keys, Node *parent, Node *&owner,
                const size_t &minSize, const Compare &cmp)
        : keys_m(keys), parent_m(parent), owner_m(owner), minSize_m(minSize),
          cmp_m(cmp) {}
    inline ~Node() {
      for (const auto &child : children_m) {
        delete child;
      }
    }

    Node(Node const &other) = delete;
    auto operator=(Node const &other) -> Node & = delete;
    Node(Node &&other) noexcept = delete;
    auto operator=(Node &&other) noexcept -> Node & = delete;

    auto add(const T &key) -> void;

    auto checkSplit() -> void;

    inline auto print(std::string step) -> void {
      if (!children_m.empty()) {
        assert(children_m[0]->parent_m == this);
        children_m[0]->print(step + "  ");
      }
      for (size_t i = 0; i < keys_m.size(); ++i) {
        std::cout << step << keys_m[i] << '\n';
        if (!children_m.empty()) {
          assert(children_m[i + 1]->parent_m == this);
          children_m[i + 1]->print(step + "  ");
        }
      }
    }

    std::vector<T> keys_m;
    std::vector<Node *> children_m{};
    Node *parent_m;
    Node *&owner_m;
    const size_t minSize_m;
    const Compare &cmp_m;
  };

  const size_t minSize_m;
  Node *root_m{};
  const Compare &cmp_m;
};

template <class T, class Compare>
auto BTree<T, Compare>::add(const T &key) -> void {
  if (root_m == nullptr) {
    root_m = new Node({}, nullptr, root_m, minSize_m, cmp_m);
  }

  root_m->add(key);
}

template <class T, class Compare>
auto BTree<T, Compare>::sliceView() const -> std::vector<std::vector<T>> {
  if (root_m == nullptr) {
    return {};
  }

  std::vector<std::vector<T>> result{};

  std::queue<Node *const *> currSlice{{&root_m}};
  std::queue<Node *const *> nextSlice{};
  std::vector<T> nextResult{};
  Node *const *nextNode{};

  while (!currSlice.empty()) {
    while (!currSlice.empty()) {
      nextNode = currSlice.front();
      currSlice.pop();

      nextResult.insert(nextResult.end(), (*nextNode)->keys_m.begin(),
                        (*nextNode)->keys_m.end());

      for (auto &child : (*nextNode)->children_m) {
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
auto BTree<T, Compare>::Node::add(const T &key) -> void {
  size_t index = 0;
  while (index != keys_m.size() && cmp_m(key, keys_m[index]) > 0) {
    ++index;
  }
  assert(index == keys_m.size() || cmp_m(key, keys_m[index]) != 0);

  if (children_m.empty()) {
    keys_m.insert(std::next(keys_m.begin(), index), key);

    if (keys_m.size() == minSize_m * 2 - 1) {
      checkSplit();
    }

    return;
  }

  children_m[index]->add(key);
}

template <class T, class Compare>
auto BTree<T, Compare>::Node::checkSplit() -> void {
  if (keys_m.size() != minSize_m * 2 - 1) {
    return;
  }

  if (parent_m == nullptr) {
    parent_m = new Node({}, nullptr, owner_m, minSize_m, cmp_m);
    parent_m->children_m.push_back(this);
    owner_m = parent_m;
  }

  int parentIndex = std::distance(parent_m->children_m.begin(),
                                  std::find(parent_m->children_m.begin(),
                                            parent_m->children_m.end(), this));

  Node *newRightNode = new Node(
      std::vector<T>(std::next(keys_m.begin(), minSize_m), keys_m.end()),
      parent_m, owner_m, minSize_m, cmp_m);

  parent_m->keys_m.insert(std::next(parent_m->keys_m.begin(), parentIndex),
                          keys_m[minSize_m - 1]);
  keys_m.resize(minSize_m - 1);

  parent_m->children_m.insert(
      std::next(parent_m->children_m.begin(), parentIndex + 1), newRightNode);

  if (!children_m.empty()) {
    for (auto it = std::next(children_m.begin(), minSize_m);
         it != children_m.end(); ++it) {
      newRightNode->children_m.push_back(*it);
      (*it)->parent_m = newRightNode;
    }

    children_m.resize(minSize_m);
  }

  parent_m->checkSplit();
}

void run(std::istream &input, std::ostream &output) {
  int t{};
  int element{};

  input >> t;
  BTree<int> tree(t);

  while (input >> element) {
    tree.add(element);
  }
  // tree.print();

  for (const auto &slice : tree.sliceView()) {
    std::copy(slice.begin(), slice.end(),
              std::ostream_iterator<int>(output, " "));
    output << '\n';
  }
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "2\n 0 1 0 1 0 1 0 1 0 1";
    run(input, output);
    // std::cout << ">>> " << output.str() << std::endl;
    // assert(output.str() == "3 \n1 5 7 \n0 2 4 6 8 9 \n");
  }
  // {
  //   std::stringstream input;
  //   std::stringstream output;
  //   input << "4\n0 1 2 3 4 5 6 7 8 9";
  //   run(input, output);
  //   std::cout << ">>> " << output.str() << std::endl;
  //   assert(output.str() == "3 \n0 1 2 4 5 6 7 8 9 \n");
  // }
}

int main() {
  run(std::cin, std::cout);
  // test();
  return 0;
}
