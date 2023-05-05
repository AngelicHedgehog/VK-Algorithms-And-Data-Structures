#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_set>

struct Node {
  std::string product_name;
  int color;
  int size;

  bool operator==(const Node &other) const {
    return color == other.color && size == other.size &&
           product_name == other.product_name;
  }
};

namespace std {

template <> struct hash<Node> {
  std::size_t operator()(const Node &node) const {

    return ((std::hash<std::string>()(node.product_name) ^
             (std::hash<int>()(node.color) << 1)) >>
            1) ^
           (hash<int>()(node.size) << 1);
  }
};

} // namespace std

void run(std::istream &input, std::ostream &output) {
  char operation{};
  Node nextNode{};

  std::unordered_set<Node> modeSet{};

  while (input >> operation >> nextNode.product_name >> nextNode.color >>
         nextNode.size) {
    switch (operation) {
    case '+':
      output << (modeSet.insert(nextNode).second ? "OK\n" : "FAIL\n");
      break;

    case '-':
      output << (modeSet.erase(nextNode) != 0 ? "OK\n" : "FAIL\n");
      break;

    case '?':
      output << (modeSet.find(nextNode) != modeSet.end() ? "OK\n" : "FAIL\n");
      break;
    }
  }
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "+ a 1 2\n+ a 1 1\n+ a 2 1\n? a 1 1\n? b 2 1\n? a 1 3\n? b 1 1";
    run(input, output);
    std::cout << ">>> " << output.str() << '\n';
    assert(output.str() == "OK\nOK\nOK\nOK\nFAIL\nFAIL\nFAIL\n");
  }
}

int main() {
  run(std::cin, std::cout);
  //   test();
  return 0;
}
