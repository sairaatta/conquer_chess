#include "screen_coordinate.h"

#include "helper.h"
#include <cassert>
#include <iostream>

screen_coordinate::screen_coordinate(const int x, const int y)
  : m_x{x}, m_y{y}
{

}

double calc_angle_degrees(const screen_coordinate& from, const screen_coordinate& to)
{
  return calc_angle_degrees(to - from);
}

double calc_angle_degrees(const screen_coordinate& delta)
{
  return calc_angle_degrees(
    delta.get_x(),
    delta.get_y()
  );
}


double calc_distance(const screen_coordinate& a, const screen_coordinate& b) noexcept
{
  const auto delta{b - a};
  return calc_distance(delta.get_x(), delta.get_y());
}

void test_screen_coordinate()
{
  #ifndef NDEBUG
  // calc_angle_degrees, one screen_coordinat
  {
    const screen_coordinate c(1, 0);
    assert(calc_angle_degrees(c) == 0.0);
  }
  // calc_angle_degrees, two screen_coordinat
  {
    const screen_coordinate c(0, 0);
    const screen_coordinate d(1, 0);
    assert(calc_angle_degrees(c, d) == 0.0);
  }
  // calc_distance
  {
    const screen_coordinate c(0, 0);
    const screen_coordinate d(3, 4);
    assert(calc_distance(c, d) == 5.0);
  }
  // operator+
  {
    const screen_coordinate c_1(1, 2);
    const screen_coordinate c_2(3, 4);
    const screen_coordinate c_3{c_1 + c_2};
    assert(c_1.get_x() + c_2.get_x() == c_3.get_x());
    assert(c_1.get_y() + c_2.get_y() == c_3.get_y());
  }
  // operator-
  {
    const screen_coordinate c_1(1, 2);
    const screen_coordinate c_2(3, 4);
    const screen_coordinate c_3{c_1 - c_2};
    assert(c_1.get_x() - c_2.get_x() == c_3.get_x());
    assert(c_1.get_y() - c_2.get_y() == c_3.get_y());
  }
  // operator*
  {
    const screen_coordinate c_1(100, 200);
    const double f{1.2};
    const screen_coordinate c_2{c_1 * f};
    assert(c_1.get_x() * f == c_2.get_x());
    assert(c_1.get_y() * f == c_2.get_y());
  }
  // operator/
  {
    const screen_coordinate c_1(100, 200);
    const double f{1.2};
    const screen_coordinate c_2{c_1 / f};
    const int expected_x{static_cast<int>(static_cast<double>(c_1.get_x()) / f)};
    const int expected_y{static_cast<int>(static_cast<double>(c_1.get_y()) / f)};
    assert(expected_x == c_2.get_x());
    assert(expected_y == c_2.get_y());
  }
  // operator+=
  {
    const screen_coordinate c_1(1, 2);
    const screen_coordinate c_2(3, 4);
    screen_coordinate c_3{c_1};
    c_3 += c_2;
    assert(c_1.get_x() + c_2.get_x() == c_3.get_x());
    assert(c_1.get_y() + c_2.get_y() == c_3.get_y());
  }
  // operator==
  {
    const screen_coordinate a(1, 2);
    const screen_coordinate b(1, 2);
    const screen_coordinate c(3, 4);
    const screen_coordinate d(1, 3);
    const screen_coordinate e(4, 2);
    assert(a == b);
    assert(!(a == c));
    assert(!(a == d));
    assert(!(a == e));
  }
  // operator!=
  {
    const screen_coordinate a(1, 2);
    const screen_coordinate b(1, 2);
    const screen_coordinate c(3, 4);
    const screen_coordinate d(1, 3);
    const screen_coordinate e(4, 2);
    assert(!(a != b));
    assert(a != c);
    assert(a != d);
    assert(a != e);
  }
  #endif
}

std::string to_str(const screen_coordinate& c) noexcept
{
  return "("
    + std::to_string(c.get_x())
    + ","
    + std::to_string(c.get_y())
    + ")"
  ;
}

std::ostream& operator<<(std::ostream& os, const screen_coordinate& c)
{
  os << to_str(c);
  return os;
}

screen_coordinate operator+(const screen_coordinate& lhs, const screen_coordinate& rhs) noexcept
{
  return screen_coordinate(
    lhs.get_x() + rhs.get_x(),
    lhs.get_y() + rhs.get_y()
  );
}

screen_coordinate operator-(const screen_coordinate& lhs, const screen_coordinate& rhs) noexcept
{
  return screen_coordinate(
    lhs.get_x() - rhs.get_x(),
    lhs.get_y() - rhs.get_y()
  );
}

screen_coordinate operator*(const screen_coordinate& lhs, const double& factor) noexcept
{
  return screen_coordinate(
    static_cast<double>(lhs.get_x()) * factor,
    static_cast<double>(lhs.get_y()) * factor
  );
}

screen_coordinate operator/(const screen_coordinate& lhs, const double& factor)
{
  assert(factor != 0.0);
  return screen_coordinate(
    static_cast<double>(lhs.get_x()) / factor,
    static_cast<double>(lhs.get_y()) / factor
  );
}

screen_coordinate& operator+=(screen_coordinate& lhs, const screen_coordinate& rhs) noexcept
{
  lhs = lhs + rhs;
  return lhs;
}

bool operator==(const screen_coordinate& lhs, const screen_coordinate& rhs) noexcept
{
  return lhs.get_x() == rhs.get_x()
    && lhs.get_y() == rhs.get_y()
  ;
}

bool operator!=(const screen_coordinate& lhs, const screen_coordinate& rhs) noexcept
{
  return !(lhs == rhs);
}
