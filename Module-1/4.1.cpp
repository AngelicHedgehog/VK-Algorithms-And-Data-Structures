#include <cassert>
#include <iostream>
#include <sstream>

template <typename T> class Array {
public:
  Array() : buf_(nullptr), size_(0), len_(0){};
  Array &operator=(Array &other) = delete;
  ~Array() { delete[] buf_; };
  int size() const { return len_; }
  bool isEmpty() const { return len_ == 0; };
  T &operator[](int index) const;
  const T &last() const;
  void deleteLast();
  void append(const T &element);

private:
  T *buf_;
  int size_;
  int len_;

  bool isFull() const { return size_ == len_; }
  void grow();
};

template <typename T> T &Array<T>::operator[](int index) const {
  assert(index < len_ && index >= 0);
  return buf_[index];
}

template <typename T> const T &Array<T>::last() const {
  assert(!isEmpty());
  return buf_[len_ - 1];
}

template <typename T> void Array<T>::deleteLast() {
  assert(!isEmpty());
  --len_;
}

template <typename T> void Array<T>::append(const T &element) {
  if (isFull())
    grow();
  buf_[len_++] = element;
}

template <typename T> void Array<T>::grow() {
  int new_size = std::max(size_ * 2, 4);
  T *new_buf = new T[new_size];

  for (int i = 0; i < size_; ++i)
    new_buf[i] = buf_[i];

  delete[] buf_;
  buf_ = new_buf;
  size_ = new_size;
}

template <typename T, typename Compare> class Heap {
public:
  explicit Heap(const Compare &cmp) : arr(), cmp_(cmp){};
  Heap &operator=(Heap &other) = delete;
  ~Heap(){};
  void insert(const T &element);
  T extractMax();

private:
  Array<T> arr;
  Compare cmp_;

  void siftDown(int i);
  void siftUp(int i);
};

template <typename T, typename Compare> void Heap<T, Compare>::insert(const T &element) {
  arr.append(element);
  siftUp(arr.size() - 1);
  // std::cout << "after insert: ";
  // for (int i = 0; i < arr.size(); ++i) std::cout << arr[i].value << ' ';
  // std::cout << std::endl;
}

template <typename T, typename Compare> T Heap<T, Compare>::extractMax() {
  assert(!arr.isEmpty());

  T result = arr[0];
  arr[0] = arr.last();
  arr.deleteLast();

  if (!arr.isEmpty())
    siftDown(0);
  // std::cout << "after extractMax: ";
  // for (int i = 0; i < arr.size(); ++i) std::cout << arr[i].value << ' ';
  // std::cout << std::endl;
  return result;
}

template <typename T, typename Compare> void Heap<T, Compare>::siftDown(int i) {
  int left = i * 2 + 1;
  int right = i * 2 + 2;
  int largest = i;

  if (left < arr.size() && cmp_(arr[left], arr[largest]) > 0)
    largest = left;
  if (right < arr.size() && cmp_(arr[right], arr[largest]) > 0)
    largest = right;

  if (largest != i) {
    std::swap(arr[i], arr[largest]);
    siftDown(largest);
  }
}

template <typename T, typename Compare> void Heap<T, Compare>::siftUp(int i) {
  while (i) {
    int parent = (i - 1) / 2;
    if (cmp_(arr[i], arr[parent]) <= 0)
      return;
    std::swap(arr[i], arr[parent]);
    i = parent;
  }
}

struct Element {
  Element() : value(NULL), values_left(-1), array_index(-1){};
  Element(int v, int v_l, int a_i) : value(v), values_left(v_l), array_index(a_i){};
  int value;
  int values_left;
  int array_index;
};

int compare(Element elem1, Element elem2) {
  // std::cout << "compare: " << elem1.value << " ? " << elem2.value <<
  // std::endl;
  return elem1.value > elem2.value ? 1 : (elem1.value == elem2.value ? 0 : -1);
}

void run(std::istream &input, std::ostream &output) {
  int k = 0;
  input >> k;

  int n = 0;
  int res_len = 0;
  int **arrays = new int *[k];
  Heap<Element, int (&)(Element, Element)> my_heap(compare);

  for (int i = 0; i < k; ++i) {
    input >> n;
    res_len += n;
    arrays[i] = new int[n];

    for (int j = 0; j < n; ++j)
      input >> arrays[i][j];

    if (n)
      my_heap.insert({arrays[i][n - 1], n - 1, i});
  }

  int *res_arr = new int[res_len];
  Element elem;

  for (int i = res_len - 1; i >= 0; --i) {
    elem = my_heap.extractMax();
    res_arr[i] = elem.value;

    if (elem.values_left--) {
      elem.value = arrays[elem.array_index][elem.values_left];
      my_heap.insert(elem);
    }
  }

  for (int i = 0; i < res_len; ++i)
    output << res_arr[i] << ' ';

  for (int i = 0; i < k; ++i)
    delete[] arrays[i];
  delete[] arrays;
  delete[] res_arr;
}

struct Test {
  std::string input;
  std::string output;
};

void test() {
  int c_tests = 3;
  Test *tests = new Test[c_tests]{{"3\t1  6\t2  50 90\t3  1 10 70\t", "1 6 10 50 70 90 "},
                                  {"4\t0\t0\t2  63 143\t3  53 57 102\t", "53 57 63 102 143 "},
                                  {"4\t4  54 141 152 194\t2  74 168\t2  45 126\t2  93 108\t", "45 54 74 93 108 126 141 152 168 194 "}};
  for (int i = 0; i < c_tests; ++i) {
    std::stringstream input;
    std::stringstream output;
    input << tests[i].input;
    run(input, output);
    std::cout << ">>> " << output.str() << std::endl;
    assert(output.str() == tests[i].output);
  }
}

int main() {
  run(std::cin, std::cout);
  // test();
  return 0;
}