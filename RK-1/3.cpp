#include <cassert>
#include <iostream>
#include <sstream>

template <class T> class CompareDefault {
public:
  int operator()(const T &a, const T &b) const { return a > b ? 1 : (a == b ? 0 : -1); }
};

template <class T, class Compare = CompareDefault<T>> void merge_sort(T *arr, int n, Compare cmp = Compare()) {
  for (int size = 1; size < n; size *= 2) {
    T *buf = new T[size * 2];

    for (int step = 0; step < n; step += size * 2) {
      int left = std::max(0, std::min(size, n - step));
      int right = std::max(0, std::min(size, n - step - size));
      int buf_i = left + right;

      while (left || right)
        if (left && right)
          buf[--buf_i] = cmp(arr[step + left - 1], arr[step + right + size - 1]) <= 0 ? arr[step + --right + size] : arr[step + --left];
        else
          buf[--buf_i] = left ? arr[step + --left] : arr[step + --right + size];

      for (int i = 0; i < size * 2 && i < n - step; ++i)
        arr[step + i] = buf[i];
    }

    delete[] buf;
  }
}

struct Box {
  Box(){};
  ~Box() { delete[] cords; };
  int *cords;
  int i;
};

void run(std::istream &input, std::ostream &output) {
  int n;
  input >> n;

  Box *boxs = new Box[n];
  int x, y, z;
  for (int i = 0; i < n; ++i) {
    input >> x >> y >> z;
    boxs[i].i = i;
    boxs[i].cords = new int[3]{x, y, z};
    merge_sort(boxs[i].cords, 3);
  }

  merge_sort(boxs, n, [](const Box &a, const Box &b) {
    return a.cords[0] >= b.cords[0] && a.cords[1] >= b.cords[1] && a.cords[2] >= b.cords[2] ? 1 : -1;
  });

  for (int i = 0; i < n; ++i)
    output << boxs[i].i << ' ';

  delete[] boxs;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "3\n2 3 5\n1 1 1\n10 4 10";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "1 0 2 ");
  }
}

int main() {
  //   run(std::cin, std::cout);
  test();
  return 0;
};