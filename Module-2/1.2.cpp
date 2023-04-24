#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct StringHasher {
  inline auto operator()(const std::string &str, int maxHash) const -> int {
    int hash = 0;
    int a = maxHash / 2 - 1;

    for (const auto &chr : str) {
      hash = (hash * a + chr) % maxHash;
    }

    return hash;
  }
};

template <class T, class H> class HashTable {
public:
  inline explicit HashTable(int initialSize, const H &hasher)
      : table_m(initialSize), hasher_m{hasher} {}

  ~HashTable() = default;
  HashTable(HashTable const &other) = default;
  auto operator=(HashTable const &other) -> HashTable & = default;
  HashTable(HashTable &&other) noexcept = default;
  auto operator=(HashTable &&other) noexcept -> HashTable & = default;

  auto Has(const T &key) const -> bool;
  auto Add(const T &key) -> bool;
  auto Delete(const T &key) -> bool;

private:
  void growTable();

  enum HashTableNodeStatus { Empty, Busy, Deleted };

  struct HashTableNode {
    T value{};
    HashTableNodeStatus status{Empty};
  };

  std::vector<HashTableNode> table_m;
  const H &hasher_m;
  size_t len_m{0};
};

template <class T, class H>
auto HashTable<T, H>::Has(const T &key) const -> bool {
  const int hash = hasher_m(key, table_m.size());
  int nodeIndex{};

  for (size_t i = 0; i < table_m.size(); ++i) {
    nodeIndex = (hash + i * (hash * 2 + 1)) % table_m.size();
    if (table_m[nodeIndex].status == Empty) {
      return false;
    }
    if (table_m[nodeIndex].status == Busy && table_m[nodeIndex].value == key) {
      return true;
    }
  }

  return false;
}

template <class T, class H> auto HashTable<T, H>::Add(const T &key) -> bool {
  const int hash = hasher_m(key, table_m.size());
  int corNodeIndex{};
  int nodeIndex{};

  for (size_t i = 0; i < table_m.size(); ++i) {
    nodeIndex = (hash + i * (hash * 2 + 1)) % table_m.size();
    if (table_m[nodeIndex].status != Busy) {
      corNodeIndex = nodeIndex;
    } else if (table_m[nodeIndex].value == key) {
      return false;
    }
    if (table_m[nodeIndex].status == Empty) {
      break;
    }
  }

  table_m[corNodeIndex].status = Busy;
  table_m[corNodeIndex].value = key;

  if (++len_m * 4 >= table_m.size() * 3) {
    growTable();
  }

  return true;
}

template <class T, class H> auto HashTable<T, H>::Delete(const T &key) -> bool {
  const int hash = hasher_m(key, table_m.size());
  int nodeIndex{};

  for (size_t i = 0; i < table_m.size(); ++i) {
    nodeIndex = (hash + i * (hash * 2 + 1)) % table_m.size();
    if (table_m[nodeIndex].status == Empty) {
      return false;
    }
    if (table_m[nodeIndex].status == Busy && table_m[nodeIndex].value == key) {
      break;
    }
  }

  table_m[nodeIndex].status = Deleted;
  table_m[nodeIndex].value = "";

  return true;
}

template <class T, class H> auto HashTable<T, H>::growTable() -> void {
  size_t newTableSize = table_m.size() * 2;
  std::vector<HashTableNode> newTable(newTableSize);
  int newHash{};
  int nodeIndex{};

  for (const auto &node : table_m) {
    if (node.status != Busy) {
      continue;
    }
    newHash = hasher_m(node.value, newTableSize);

    for (size_t i = 0; i < newTableSize; ++i) {
      nodeIndex = (newHash + i * (newHash * 2 + 1)) % newTableSize;
      if (newTable[nodeIndex].status != Empty) {
        continue;
      }
      break;
    }
    newTable[nodeIndex] = node;
  }

  table_m = std::move(newTable);
}

void run(std::istream &input, std::ostream &output) {
  char operation{};
  std::string value{};
  bool result{};

  StringHasher h;
  HashTable<std::string, StringHasher> table{8, h};

  while (input >> operation >> value) {
    switch (operation) {
    case '+':
      result = table.Add(value);
      break;
    case '-':
      result = table.Delete(value);
      break;
    case '?':
      result = table.Has(value);
      break;
    default:
      result = false;
      break;
    }
    output << (result ? "OK" : "FAIL") << '\n';
  }
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "- 18\n- 4\n? 0\n- 1\n+ 17\n- 11\n? 5\n? 5\n+ 11\n- 16\n? 8\n- "
             "17\n? 11\n- 9\n+ 9\n? 16\n? 7\n? 10\n+ 11\n+ 7\n+ 7\n- 9\n? "
             "11\n- 4\n? 17\n- 2\n? 3\n? 18\n- 4\n? 0\n? 6\n- 1\n+ 14\n+ 3\n+ "
             "17\n? 0\n? 1\n- 8\n+ 1\n- 17\n- 4\n? 5\n? 17\n- 7\n+ 19\n+ 18\n+ "
             "0\n+ 11\n? 16\n+ 6\n- 8\n? 10\n+ 12\n- 6\n+ 6\n- 14\n- 16\n+ "
             "12\n+ 11\n- 7\n? 5\n? 18\n- 18\n- 0\n+ 13\n+ 4\n- 8\n- 19\n- "
             "10\n- 5\n? 0\n? 11\n? 16\n+ 19\n- 1\n+ 11";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() ==
           "FAIL\nFAIL\nFAIL\nFAIL\nOK\nFAIL\nFAIL\nFAIL\nOK\nFAIL\nFAIL\nOK\nO"
           "K\nFAIL\nOK\nFAIL\nFAIL\nFAIL\nFAIL\nOK\nFAIL\nOK\nOK\nFAIL\nFAIL\n"
           "FAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nOK\nOK\nOK\nFAIL\nFAIL\nF"
           "AIL\nOK\nOK\nFAIL\nFAIL\nFAIL\nOK\nOK\nOK\nOK\nFAIL\nFAIL\nOK\nFAIL"
           "\nFAIL\nOK\nOK\nOK\nOK\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nOK\nOK\nOK\nO"
           "K\nOK\nFAIL\nOK\nFAIL\nFAIL\nFAIL\nOK\nFAIL\nOK\nOK\nFAIL\n");
  }
}

int main() {
  run(std::cin, std::cout);
  // test();
  return 0;
}