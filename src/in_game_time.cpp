#include "in_game_time.h"

#include <cassert>
#include <iostream>
#include <sstream>

#include "delta_t.h"

in_game_time::in_game_time(const double dt)
  : m_in_game_time{dt}
{

}

void test_in_game_time()
{
#ifndef NDEBUG
  // get
  {
    const double dt{0.123};
    const in_game_time t{dt};
    assert(t.get() == dt);
  }
  // operator==
  {
    const in_game_time a(0.1);
    const in_game_time b(0.1);
    const in_game_time c(0.9876);
    assert(a == b);
    assert(!(a == c));
  }
  // operator<
  {
    const in_game_time a(0.1);
    const in_game_time b(0.2);
    assert(a < b);
  }
  // in_game_time& operator+=(in_game_time& game_time, const delta_t& dt) noexcept;
  {
    in_game_time a{0.0};
    const delta_t dt{0.1};
    a += dt;
    assert(a.get() == dt.get());
  }
  // operator+
  {
    const double t1{0.1};
    const double t2{0.2};
    const in_game_time d1{t1};
    const delta_t dt{t2};
    const in_game_time d3{d1 + dt};
    assert(d3.get() == t1 + t2);
  }
  // operator-
  {
    const double t1{0.5};
    const double t2{0.3};
    const in_game_time d1{t1};
    const in_game_time d2{t2};
    const delta_t d3{d1 - d2};
    assert(d3.get() == t1 - t2);
  }
  // operator+=
  {
    const double t1{0.1};
    const double t2{0.2};
    in_game_time d1{t1};
    const delta_t dt{t2};
    d1 += dt;
    assert(d1.get() == t1 + t2);
  }
  // operator>
  {
    const in_game_time low{0.0001};
    const in_game_time high{0.11};
    assert(high > low);
  }
  // operator>=
  {
    const in_game_time low{0.0001};
    const in_game_time high{0.11};
    assert(high >= low);
  }
  // operator<<
  {
    std::stringstream s;
    s << in_game_time(0.123);
    assert(!s.str().empty());
  }
#endif // DEBUG
}

bool operator==(const in_game_time& lhs, const in_game_time& rhs) noexcept
{
  return lhs.get() == rhs.get();
}

bool operator<(const in_game_time& lhs, const in_game_time& rhs) noexcept
{
  return lhs.get() < rhs.get();
}

bool operator<=(const in_game_time& lhs, const in_game_time& rhs) noexcept
{
  return lhs.get() <= rhs.get();
}

in_game_time& operator+=(in_game_time& lhs, const delta_t& dt) noexcept
{
  lhs = lhs + dt;
  return lhs;
}

in_game_time operator+(const in_game_time& lhs, const delta_t& dt) noexcept
{
  return in_game_time(lhs.get() + dt.get());
}

in_game_time operator-(const in_game_time& lhs, const delta_t& dt) noexcept
{
  return in_game_time(lhs.get() - dt.get());
}

delta_t operator-(const in_game_time& lhs, const in_game_time& dt) noexcept
{
  return delta_t(lhs.get() - dt.get());
}

bool operator>(const in_game_time& lhs, const in_game_time& rhs) noexcept
{
  return lhs.get() > rhs.get();
}

bool operator>=(const in_game_time& lhs, const in_game_time& rhs) noexcept
{
  return lhs.get() >= rhs.get();
}

std::ostream& operator<<(std::ostream& os, const in_game_time& dt) noexcept
{
  os << dt.get();
  return os;
}
