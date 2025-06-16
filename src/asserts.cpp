#include "asserts.h"

#include <cassert>

void test_asserts()
{
#ifndef NDEBUG
  // assert_eq: OK
  {
    const std::string s{"something irrelevant"};
    const std::string t{s};
    assert_eq(s, t);
  }
#endif // NDEBUG
}

