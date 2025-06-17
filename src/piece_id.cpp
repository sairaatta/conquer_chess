#include "piece_id.h"

#include <cassert>
#include <iostream>
#include <sstream>

int piece_id::sm_next_value{0};

piece_id::piece_id()
  : m_value{sm_next_value++}
{
  // I assume less then two billion IDs :-)
  assert(m_value >= 0);
}

piece_id create_new_id() noexcept
{
  return piece_id();
}

void test_id()
{
#ifndef NDEBUG
  {
    const auto a{create_new_id()};
    const auto b{create_new_id()};
    assert(a == a);
    assert(!(a == b));
    assert(a != b);
  }
  // operator<<
  {
    const piece_id i = create_new_id();
    std::stringstream s;
    s << i;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

bool operator==(const piece_id& lhs, const piece_id& rhs) noexcept
{
  return lhs.get() == rhs.get();
}

bool operator!=(const piece_id& lhs, const piece_id& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const piece_id& i) noexcept
{
  os << i.get();
  return os;
}
