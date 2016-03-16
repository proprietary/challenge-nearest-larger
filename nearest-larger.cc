// $ clang++ nearest-larger.cc -std=c++11 -o nearest-larger -Wall -g
// $ ./nearest-larger

#include <iostream>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>

//////////IGNORE THIS---IRRELEVANT AUTISM
template<typename T>
T abs(const T n) {
  T m = n >> (sizeof(T)*CHAR_BIT - 1);
  T ret = (n^m) - m;
  return ret;
}

template<typename T>
int cmp(const T x, const T y) {
  if(x > y) {
    return 1;
  } else if(y > x) {
    return -1;
  } else {
    return 0;
  }
}

template <typename T>
struct Error {
  bool state;
  T expected;
  T got;
  const char* fnName;

  // builders
  static Error bad(T expected, T got, const char* fnName) {
    Error r;
    r.state = true;
    r.expected = expected;
    r.got = got;
    r.fnName = fnName;
    return r;
  }
  static Error good(const char* fnName) {
    Error r;
    r.state = false;
    r.fnName = fnName;
    return r;
  }
};

Error<int64_t> testAbs() {
  int64_t x = -25L;
  uint64_t y = abs<int64_t>(x);
  uint64_t z = 25L;
  if(y != z) {
    return Error<int64_t>::bad(z, y, "testAbs");
  } else {
    return Error<int64_t>::good("testAbs");
  }
}

Error<int> testCmp1() {
  uint32_t x1 = 42;
  uint32_t y1 = 8;
  int g1 = cmp<uint32_t>(x1, y1);
  int e1 = 1;
  if(g1 != e1) {
    return Error<int>::bad(e1, g1, "testCmp1");
  } else {
    return Error<int>::good("testCmp1");
  }
}

Error<int> testCmp2() {
  uint32_t x1 = 8;
  uint32_t y1 = 42;
  int g1 = cmp<uint32_t>(x1, y1);
  int e1 = -1;
  if(g1 != e1) {
    return Error<int>::bad(e1, g1, "testCmp2");
  } else {
    return Error<int>::good("testCmp2");
  }
}

Error<int> testCmp3() {
  uint32_t x1 = 8;
  uint32_t y1 = 8;
  int g1 = cmp<uint32_t>(x1, y1);
  int e1 = 0;
  if(g1 != e1) {
    return Error<int>::bad(e1, g1, "testCmp3");
  } else {
    return Error<int>::good("testCmp3");
  }
}

template<typename T>
void runTest(Error<T> (*fn)()) {
  Error<T> e = (*fn)();
  if(e.state == false) {
    std::cout << "Test passed! ("
      << e.fnName << ")\n";
  } else {
    std::ostringstream msg;
    msg << "Test failed! (" << e.fnName << ")\n"
      << "Expected\t" << e.expected
      << "\nGot\t\t" << e.got
      << "\n---------------------\n";
    std::cout << msg.str();
  }
}

///////////STOP IGNORING
///////////READ THIS
int nearestLarger(int* const arr, int n, int i) {
  // `n` is the length of `arr`
  // 1. `arr[i]` < `arr[j]`
  // 2. there is no `j2` closer to `i` than `j`
  //    where `arr[i]` < `arr[j2]`
  for(int b = i > 0 ? (i-1) : 0; b >= 0; b--) {
    for(int f = i; f < n; f++) {
      if(arr[f] > arr[i] || arr[b] > arr[i]) {
        int c = cmp<int>(arr[b], arr[f]);
        switch(c) {
          case -1:
            return f;
          case 1:
            return b;
          case 0:
            return b;
        }
      }
    }
  }

  return -1; // error! faulty algorithm!
}

Error<int> testNearestLarger() {
  int arrs[][4] = {
    { 2,3,4,8 },
    { 2,8,4,3 },
    { 2,6,4,8 },
    { 2,6,4,6 },
    { 8,2,4,3 },
    { 2,4,3,8 },
    { 2,6,4,8 },
    { 2,6,9,4 }
  };
  int expecteds[8] = { 3,1,1,1,0,3,3,3 };
  int* arr = NULL; int expected = 0;

  for(int i = 0; i < sizeof(arrs[0])/sizeof(arrs); i++) {
    arr = arrs[i];
    expected = expecteds[i];
    int j = nearestLarger(&arr[0], 4, expected);
    if(j == -1) {
      return Error<int>::bad(expected, j, "nearestLarger; faulty algorithm!");
    } else if(j == expected) {
      continue;
      // return Error<int>::good("nearestLarger");
    } else {
      return Error<int>::bad(expected, j, "nearestLarger");
    }
  }
  return Error<int>::good("nearestLarger");
}

int main(int argc, char** argv) {
  runTest(&testAbs);
  runTest(&testCmp1);
  runTest(&testCmp2);
  runTest(&testCmp3);
  runTest(&testNearestLarger);
  return 0;
}
