#include <cassert>
#include <iostream>
#include <sstream>

void run(std::istream &input, std::ostream &output) {}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "5");
  }
}

int main() {
  //   run(std::cin, std::cout);
  test();
  return 0;
};