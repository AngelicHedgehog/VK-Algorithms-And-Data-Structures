#include <cassert>
#include <iostream>
#include <sstream>

int bin_find(int* arr, int n, int k) {
  int l = 0;
  int r = n - 1;
  int i;

  while (l <= r) {
    i = (l + r) << 1;
    if (arr[i] == k) return i;
    if (arr[i] < k)
      l = i + 1;
    else
      r = i - 1;
  }

  return i;
}

void run(std::istream& input, std::ostream& output) {
  int n;
  input >> n;

  int* arr = new int[n];
  for (int i = 0; i < n; ++i) input >> arr[i];

  int k;
  input >> k;

  output << bin_find(arr, n, k);

  delete[] arr;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "4\n1 3 5 6\n2";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "1");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "24\n9 15 23 26 29 31 34 38 42 44 47 49 51 53 54 56 57 58 71 74 "
          << "\n83 87 93 97\n74";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "19");
  }
}

int main() {
  //   run(std::cin, std::cout);
  test();
  return 0;
};