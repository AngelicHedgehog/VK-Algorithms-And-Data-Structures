#include <cassert>
#include <iostream>
#include <sstream>


void find_valid_range(int &begin, int &end, int size_arr, int *&arr, int val) {
  if (end > size_arr) {
    end = size_arr - 1;
    return;
  }

  while (arr[end] <= val) {
    begin <<= 1;
    end <<= 1;
    if (end >= size_arr) {
      end = size_arr - 1;
      return;
    }
  }
}

bool is_in_range(int begin, int end, const int *arr, int val) {
  int mid = 0;
  while (end - begin != 1) {
    mid = (begin + end) / 2;
    if (arr[mid] <= val)
      begin = mid;
    else
      end = mid;
  }

  return arr[begin] == val || arr[end] == val;
}

void find_intersect(int size_a, int size_b, int *&arr_a, int *&arr_b,
                    bool *&result) {
  int start = 0;
  int stop = 1;
  for (int i = 0; i < size_b; ++i) {
    find_valid_range(start, stop, size_a, arr_a, arr_b[i]);
    result[i] = is_in_range(start, stop, arr_a, arr_b[i]);
  }
}

void run(std::istream &input, std::ostream &output) {
  int n = 0;
  int k = 0;
  input >> n >> k;

  int *arr_a = new int[n];
  for (int i = 0; i < n; ++i) {
    input >> arr_a[i];
  }
  int *arr_b = new int[k];
  for (int i = 0; i < k; ++i) {
    input >> arr_b[i];
  }
  bool *is_b_in_a = new bool[k];
  for (int i = 0; i < k; ++i) {
    is_b_in_a[i] = false;
  }

  find_intersect(n, k, arr_a, arr_b, is_b_in_a);

  for (int i = 0, first = 1; i < k; ++i) {
    if (is_b_in_a[i]) {
      if (first)
        first = 0;
      else
        output << ' ';
      output << arr_b[i];
    }
  }
  delete[] arr_a;
  delete[] arr_b;
  delete[] is_b_in_a;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "2 1 10 20 20";
    run(input, output);
    std::cout << output.str() << '\n';
    assert(output.str() == "20");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "100 5\
        0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19\
        20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39\
        40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59\
        60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79\
        80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99\
        0 20 40 80 99";
    run(input, output);
    std::cout << output.str() << '\n';
    assert(output.str() == "0 20 40 80 99");
  }
}

int main() {
  run(std::cin, std::cout);
  // test();
  return 0;
}