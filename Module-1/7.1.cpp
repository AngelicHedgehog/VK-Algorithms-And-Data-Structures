#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

void sortMSD(std::vector<std::string>& arr, std::size_t index = 0) {
  std::vector<std::vector<std::string>> alphabet{};

  int minSym{}, maxSym{}, sym{};
  for (const auto& el : arr) {
    if (index >= el.size()) {
      continue;
    }

    sym = el[index];

    if (sym < minSym || minSym == 0) {
      minSym = sym;
    }

    if (sym > maxSym) {
      maxSym = sym;
    }
  }

  for (int i = 0; i <= maxSym - minSym; ++i) {
    alphabet.push_back({});
  }

  int finInd = -1;

  for (const auto& next : arr) {
    if (index >= next.length()) {
      arr[++finInd] = next;
    } else {
      alphabet[next[index] - minSym].push_back(next);
    }
  }

  ++index;
  for (int i = 0; i <= maxSym - minSym; ++i) {
    if (alphabet[i].size() > 1) {
      sortMSD(alphabet[i], index);
    }

    for (const auto& el : alphabet[i]) {
      arr[++finInd] = el;
    }
  }
}

void run(std::istream& input, std::ostream& output) {
  std::vector<std::string> arr{};

  input >> std::noskipws;

  char sym{0};
  std::string word{};
  while (input.eof()) {
    input >> word;
    arr.push_back(word);
    // if (sym == '\0') {
    //   arr.push_back(word);
    //   word = "";
    // } else {
    //   word.push_back(sym);
    // }
  }
  // arr.push_back(word);

  // std::sort(arr.begin(), arr.end());
  sortMSD(arr);

  for (const auto& el : arr) {
    output << el << '\n';
  }
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "\0";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "\0");
  }
}

int main() {
  run(std::cin, std::cout);
  // test();
  return 0;
};