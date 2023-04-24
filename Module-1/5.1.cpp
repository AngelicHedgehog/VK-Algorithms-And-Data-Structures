#include <cassert>
#include <iostream>
#include <sstream>

template <class T> class CompareDefault {
public:
  int operator()(const T &a, const T &b) const {
    return a > b ? 1 : (a == b ? 0 : -1);
  }
};

template <class T, class Compare = CompareDefault<T>>
void merge_sort(T *arr, int n, Compare cmp = Compare()) {
  for (int size = 1; size < n; size *= 2) {
    T *buf = new T[size * 2];

    for (int step = 0; step < n; step += size * 2) {
      int left = std::max(0, std::min(size, n - step));
      int right = std::max(0, std::min(size, n - step - size));
      int buf_i = left + right;

      while (left || right)
        if (left && right)
          buf[--buf_i] =
              cmp(arr[step + left - 1], arr[step + right + size - 1]) <= 0
                  ? arr[step + --right + size]
                  : arr[step + --left];
        else
          buf[--buf_i] = left ? arr[step + --left] : arr[step + --right + size];

      for (int i = 0; i < size * 2 && i < n - step; ++i)
        arr[step + i] = buf[i];
    }

    delete[] buf;
  }
}

struct TimeSeg {
  int t1;
  int t2;
};

void run(std::istream &input, std::ostream &output) {
  int n;
  input >> n;

  TimeSeg *time_segs = new TimeSeg[n];
  for (int i = 0; i < n; ++i)
    input >> time_segs[i].t1 >> time_segs[i].t2;

  merge_sort(time_segs, n, [](const TimeSeg &a, const TimeSeg &b) {
    return a.t2 > b.t2
               ? 1
               : (a.t2 == b.t2 ? (a.t1 < b.t1 ? 1 : (a.t1 == b.t1 ? 0 : -1))
                               : -1); // сортировка в первую очередь по
                                      // возрастанию времени ухода и во вторую
                                      // по убыванию времени призода
  });

  if (n == 0) {
    output << "0";
    return;
  }

  int res = 2;
  int t1 = time_segs[0].t2 - 1;
  int t2 = time_segs[0].t2;
  for (int i = 1; i < n; ++i)
    if (t2 < time_segs[i].t2) {
      if (t2 < time_segs[i].t1) {
        res += 2;
        t1 = time_segs[i].t2 - 1;
        t2 = time_segs[i].t2;
      } else if (t1 < time_segs[i].t1) {
        res += 1;
        t1 = t2;
        t2 = time_segs[i].t2;
      }
    }

  output << res;

  delete[] time_segs;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "5\n1 10\n10 12\n1 10\n1 10\n23 24";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "5");
  }
}

int main() {
  run(std::cin, std::cout);
  //   test();
  return 0;
};