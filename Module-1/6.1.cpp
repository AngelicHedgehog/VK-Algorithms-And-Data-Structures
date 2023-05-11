#include <cassert>
#include <iostream>
#include <sstream>

template <class T> class CompareDefault {
public:
  int operator()(const T &a, const T &b) const { return a > b ? 1 : (a == b ? 0 : -1); }
};

template <class T, class Compare = CompareDefault<T>> int partition(T *a, int n, Compare cmp = Compare()) {
  int cmp_first_last = cmp(a[0], a[n - 1]);
  int cmp_first_mid = cmp(a[0], a[n / 2]);
  int cmp_mid_last = cmp(a[n / 2], a[n - 1]);

  if (cmp_first_last * cmp_first_mid <= 0)
    std::swap(a[0], a[n - 1]);
  else if (cmp_first_mid * cmp_mid_last >= 0)
    std::swap(a[n / 2], a[n - 1]);

  T part = a[n - 1];
  int i = 0;

  for (int j = 0; j < n; ++j)
    if (cmp(a[j], part) <= 0)
      std::swap(a[i++], a[j]);

  return i - 1;
}

template <class T, class Compare = CompareDefault<T>> T findKStat(T *a, int n, int k, Compare cmp = Compare()) {
  int pivot_pos = partition(a, n, cmp);

  for (; pivot_pos != k; pivot_pos = partition(a, n, cmp))
    if (pivot_pos < k) {
      a += pivot_pos + 1;
      n -= pivot_pos + 1;
      k -= pivot_pos + 1;
    } else
      n = pivot_pos;

  return a[pivot_pos];
}

void run(std::istream &input, std::ostream &output) {
  int n;
  input >> n;

  int *arr = new int[n];
  for (int i = 0; i < n; ++i)
    input >> arr[i];

  output << findKStat(arr, n, n * .1) << std::endl;
  output << findKStat(arr, n, n * .5) << std::endl;
  output << findKStat(arr, n, n * .9) << std::endl;

  delete[] arr;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "10\n1 2 3 4 5 6 7 8 9 10";
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == "2\n6\n10\n");
  }
}

int main() {
  run(std::cin, std::cout);
  //   test();
  return 0;
};