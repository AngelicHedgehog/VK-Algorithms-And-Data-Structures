#include <cassert>
#include <iostream>
#include <sstream>

template <typename T> class CycleQueue {
public:
  CycleQueue() : buf_(nullptr), size_(0), len_(0), begin_(0), end_(0){};
  ~CycleQueue() { delete[] buf_; };
  CycleQueue(const CycleQueue &ref_queue) = delete;
  T pop();
  void push(const T &value);
  bool isEmpty() const { return len_ == 0; };

private:
  bool isFull() const { return len_ == size_; };
  void grow();
  T *buf_;
  int size_, len_, begin_, end_;
};

template <typename T> T CycleQueue<T>::pop() {
  assert(!isEmpty());

  --len_;
  if (++begin_ == size_) {
    begin_ = 0;
    return buf_[size_ - 1];
  }

  return buf_[begin_ - 1];
}

template <typename T> void CycleQueue<T>::push(const T &value) {
  if (isFull())
    grow();

  ++len_;
  buf_[end_] = value;
  if (++end_ == size_)
    end_ = 0;
}

template <typename T> void CycleQueue<T>::grow() {
  int newSize = std::max(size_ * 2, 4);
  T *newBuf = new T[newSize];
  for (int i = begin_ - size_; i < 0; ++i) {
    newBuf[newSize + i] = buf_[size_ + i];
  }
  for (int i = 0; i < end_; ++i) {
    newBuf[i] = buf_[i];
  }

  if (size_)
    begin_ += newSize - size_;

  delete[] buf_;
  buf_ = newBuf;
  size_ = newSize;
}

template <typename T> int exec(CycleQueue<T> &queue, int cmd, int arg) {
  switch (cmd) {
  case 2:
    return (!queue.isEmpty() && queue.pop() == arg) ||
                   (queue.isEmpty() && arg == -1)
               ? 0
               : 1;
  case 3:
  default:
    queue.push(arg);
    return 0;
  }
}

void run(std::istream &input, std::ostream &output) {
  int n = 0;
  input >> n;

  int a = 0;
  int b = 0;
  CycleQueue<int> queue;
  for (int i = 0; i < n; ++i) {
    input >> a >> b;
    if (exec(queue, a, b)) {
      output << "NO";
      return;
    }
  }

  output << "YES";
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "3 3 44 3 50 2 44";
    run(input, output);
    std::cout << output.str() << '\n';
    assert(output.str() == "YES");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "2 2 -1 3 10";
    run(input, output);
    std::cout << output.str() << '\n';
    assert(output.str() == "YES");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "2 3 44 2 66";
    run(input, output);
    std::cout << output.str() << '\n';
    assert(output.str() == "NO");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "40\
		2 -1   3 66   3 881   2 66   3 832   3 727   3 920   3 469   3 698   2 881\
        3 188   2 832   3 892   2 727   2 920   2 469   3 891   2 698   2 188   2 892\
		2 891   3 224   2 224   3 431   2 431   2 -1   2 -1   2 -1   2 -1   2 -1\
        3 697   3 433   3 598   3 651   3 710   3 764   3 498   3 989   3 841   2 697";
    run(input, output);
    std::cout << output.str() << '\n';
    assert(output.str() == "YES");
  }
}

int main() {
  run(std::cin, std::cout);
  // test();
  return 0;
}