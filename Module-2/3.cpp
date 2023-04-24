#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void run(std::istream &input, std::ostream &output) {}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "");
  }
}

int main() {
  //   run(std::cin, std::cout);
  test();
  return 0;
}