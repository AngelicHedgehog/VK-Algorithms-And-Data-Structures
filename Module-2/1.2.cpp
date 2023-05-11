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
  inline explicit HashTable(int initialSize, const H &hasher) : table_m(initialSize), hasher_m{hasher} {}

  auto Has(const T &key) const -> bool;
  auto Add(const T &key) -> bool;
  auto Delete(const T &key) -> bool;

private:
  auto growTable() -> void;
  auto rehashTable() -> void;

  enum class HashTableNodeStatus { Empty, Busy, Deleted };

  struct HashTableNode {
    T value{};
    HashTableNodeStatus status{HashTableNodeStatus::Empty};
  };

  std::vector<HashTableNode> table_m;
  const H &hasher_m;
  size_t occupancy_m{0};
  size_t countBusyNodes_m{0};
};

template <class T, class H> auto HashTable<T, H>::Has(const T &key) const -> bool {
  const int hash{hasher_m(key, table_m.size())};
  int nodeIndex{};

  for (size_t i = 0; i < table_m.size(); ++i) {
    nodeIndex = (hash + i * (hash * 2 + 1)) % table_m.size();

    if (table_m[nodeIndex].status == HashTableNodeStatus::Empty) {
      return false;
    }

    if (table_m[nodeIndex].status == HashTableNodeStatus::Busy && table_m[nodeIndex].value == key) {
      return true;
    }
  }

  return false;
}

template <class T, class H> auto HashTable<T, H>::Add(const T &key) -> bool {
  const int hash{hasher_m(key, table_m.size())};
  int nodeIndex{};
  HashTableNodeStatus nodeOldStatus{};

  for (size_t i = 0; i < table_m.size(); ++i) {
    nodeIndex = (hash + i * (hash * 2 + 1)) % table_m.size();

    if (table_m[nodeIndex].status == HashTableNodeStatus::Empty) {
      nodeOldStatus = table_m[nodeIndex].status;

      table_m[nodeIndex].status = HashTableNodeStatus::Busy;
      table_m[nodeIndex].value = key;

      ++countBusyNodes_m;

      if (nodeOldStatus == HashTableNodeStatus::Empty && ++occupancy_m * 4 >= table_m.size() * 3) {
        growTable();
      }

      return true;
    }

    if (table_m[nodeIndex].status == HashTableNodeStatus::Busy && table_m[nodeIndex].value == key) {
      return false;
    }
  }

  return false;
}

template <class T, class H> auto HashTable<T, H>::Delete(const T &key) -> bool {
  const int hash{hasher_m(key, table_m.size())};
  int nodeIndex{};

  for (size_t i = 0; i < table_m.size(); ++i) {
    nodeIndex = (hash + i * (hash * 2 + 1)) % table_m.size();

    if (table_m[nodeIndex].status == HashTableNodeStatus::Empty) {
      return false;
    }

    if (table_m[nodeIndex].status == HashTableNodeStatus::Busy && table_m[nodeIndex].value == key) {
      table_m[nodeIndex].status = HashTableNodeStatus::Deleted;

      if (--countBusyNodes_m * 4 <= table_m.size()) {
        rehashTable();
      }

      return true;
    }
  }

  return false;
}

template <class T, class H> auto HashTable<T, H>::growTable() -> void {
  table_m.resize(table_m.size() * 2);
  rehashTable();
}

template <class T, class H> auto HashTable<T, H>::rehashTable() -> void {
  std::vector<HashTableNode> newTable{table_m.size()};
  int newOccupancy{0};
  int newHash{};
  int nodeIndex{};

  for (const auto &node : table_m) {
    if (node.status != HashTableNodeStatus::Busy) {
      continue;
    }

    newHash = hasher_m(node.value, table_m.size());
    ++newOccupancy;

    for (size_t i = 0; i < newTable.size(); ++i) {
      nodeIndex = (newHash + i * (newHash * 2 + 1)) % newTable.size();

      if (newTable[nodeIndex].status == HashTableNodeStatus::Empty) {
        newTable[nodeIndex] = node;
        break;
      }
    }
  }

  table_m = std::move(newTable);
  occupancy_m = newOccupancy;
}

void run(std::istream &input, std::ostream &output) {
  char operation{};
  std::string value{};
  bool result{};

  StringHasher h{};
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
    std::stringstream input{};
    std::stringstream output{};
    input << "+ hello\n+ bye\n? bye\n+ bye\n- bye\n? bye\n? hello";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");
  }
  {
    std::stringstream input{};
    std::stringstream output{};
    input << "- 18\n- 4\n? 0\n- 1\n+ 17\n- 11\n? 5\n? 5\n+ 11\n- 16\n? 8\n- "
             "17\n? 11\n- 9\n+ 9\n? 16\n? 7\n? 10\n+ 11\n+ 7\n+ 7\n- 9\n? "
             "11\n- 4\n? 17\n- 2\n? 3\n? 18\n- 4\n? 0\n? 6\n- 1\n+ 14\n+ 3\n+ "
             "17\n? 0\n? 1\n- 8\n+ 1\n- 17\n- 4\n? 5\n? 17\n- 7\n+ 19\n+ 18\n+ "
             "0\n+ 11\n? 16\n+ 6\n- 8\n? 10\n+ 12\n- 6\n+ 6\n- 14\n- 16\n+ "
             "12\n+ 11\n- 7\n? 5\n? 18\n- 18\n- 0\n+ 13\n+ 4\n- 8\n- 19\n- "
             "10\n- 5\n? 0\n? 11\n? 16\n+ 19\n- 1\n+ 11";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "FAIL\nFAIL\nFAIL\nFAIL\nOK\nFAIL\nFAIL\nFAIL\nOK\nFAIL\nFAIL\nOK\nO"
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