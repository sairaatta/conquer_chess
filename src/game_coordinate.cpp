#include "game_coordinate.h"

#include "helper.h"
#include "square.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

game_coordinate::game_coordinate(
  const double x,
  const double y
) : m_x{x}, m_y{y}
{

}


double calc_distance(const game_coordinate& lhs, const game_coordinate& rhs) noexcept
{
  const double dx{lhs.get_x() - rhs.get_x()};
  const double dy{lhs.get_y() - rhs.get_y()};
  return calc_distance(dx, dy);
}

double calc_length(const game_coordinate& coordinat) noexcept
{
  return calc_distance(
    coordinat,
    game_coordinate()
  );
}

game_coordinate center_on_center(const game_coordinate& coordinat)
{
  return game_coordinate(
    std::trunc(coordinat.get_x()) + 0.5,
    std::trunc(coordinat.get_y()) + 0.5
  );
}

game_coordinate create_random_game_coordinate(
  std::default_random_engine& rng_engine
)
{
  return to_coordinat(create_random_square(rng_engine));
}

game_coordinate get_above(const game_coordinate& coordinat) noexcept
{
  game_coordinate pos{coordinat + game_coordinate(0.0, -1.0)};
  if (pos.get_y() < 0.0)
  {
    pos += game_coordinate(0.0, 8.0);
  }
  return pos;
}

game_coordinate get_below(const game_coordinate& coordinat) noexcept
{
  game_coordinate pos{coordinat + game_coordinate(0.0, 1.0)};
  if (pos.get_y() > 8.0)
  {
    pos += game_coordinate(0.0, -8.0);
  }
  return pos;
}

game_coordinate get_left(const game_coordinate& coordinat) noexcept
{
  game_coordinate pos{coordinat + game_coordinate(-1.0, 0.0)};
  if (pos.get_x() < 0.0)
  {
    pos += game_coordinate(8.0, 0.0);
  }
  return pos;
}

game_coordinate get_right(const game_coordinate& coordinat) noexcept
{
  game_coordinate pos{coordinat + game_coordinate(1.0, 0.0)};
  if (pos.get_x() > 8.0)
  {
    pos += game_coordinate(-8.0, 0.0);
  }
  return pos;
}

game_coordinate get_rotated_coordinat(const game_coordinate& coordinat) noexcept
{
  return game_coordinate(
    8.0 - coordinat.get_x(),
    8.0 - coordinat.get_y()
  );
}

bool is_coordinat_on_board(const game_coordinate& c) noexcept
{
  return
       c.get_x() >= 0.0
    && c.get_x() <  8.0
    && c.get_y() >= 0.0
    && c.get_y() <  8.0
  ;
}

bool is_forward(
  const chess_color color,
  const square& from,
  const square& to
)
{
  const bool is_right{to.get_x() > from.get_x()};
  return (color == chess_color::white && is_right)
    || (color == chess_color::black && !is_right)
  ;
}

void test_game_coordinate()
{
  #ifndef NDEBUG
  // calc_length
  {
    const game_coordinate c(3.0, 4.0);
    assert(is_close(calc_length(c), 5.0, 0.01));
  }
  // center_on_center
  {
    const game_coordinate c(3.2, 4.7);
    const game_coordinate expected(3.5, 4.5);
    const game_coordinate created(center_on_center(c));
    assert(expected == created);
  }
  // create_random_game_coordinat
  {
    const int seed{314};
    std::default_random_engine rng_engine(seed);
    create_random_game_coordinate(rng_engine);
  }
  // get
  {
    const double x{12.34};
    const double y{23.45};
    const game_coordinate c(x, y);
    assert(c.get_x() == x);
    assert(c.get_y() == y);
  }
  // get_above
  {
    const game_coordinate c(3.5, 3.5);
    const auto above{get_above(c)};
    assert(above.get_y() < c.get_y());
  }
  // get_above loops
  {
    const game_coordinate c(3.5, 0.5);
    const auto above{get_above(c)};
    assert(above.get_y() > c.get_y());
  }
  // get_below
  {
    const game_coordinate c(3.5, 3.5);
    const auto below{get_below(c)};
    assert(below.get_y() > c.get_y());
  }
  // get_below loops
  {
    const game_coordinate c(3.5, 7.5);
    const auto below{get_below(c)};
    assert(below.get_y() < c.get_y());
  }
  // get_left
  {
    const game_coordinate c(3.5, 3.5);
    const auto left{get_left(c)};
    assert(left.get_x() < c.get_x());
  }
  // get_left loops
  {
    const game_coordinate c(0.5, 3.5);
    const auto left{get_left(c)};
    assert(left.get_x() > c.get_x());
  }
  // get_right
  {
    const game_coordinate c(3.5, 3.5);
    const auto right{get_right(c)};
    assert(right.get_x() > c.get_x());
  }
  // get_right loops
  {
    const game_coordinate c(7.5, 3.5);
    const auto right{get_right(c)};
    assert(right.get_x() < c.get_x());
  }
  // get_rotated_coordinat
  {
    const game_coordinate c(1.5, 2.5);
    const auto rotated{get_rotated_coordinat(c)};
    assert(is_close(rotated.get_x(), 6.5, 0.1));
    assert(is_close(rotated.get_y(), 5.5, 0.1));
  }
  // is_coordinat_on_board
  {
    assert(is_coordinat_on_board(game_coordinate()));
    assert(is_coordinat_on_board(game_coordinate(1.2, 3.4)));
    assert(is_coordinat_on_board(game_coordinate(4.0, 4.0)));
    assert(!is_coordinat_on_board(game_coordinate(-4.0, 4.0)));
    assert(!is_coordinat_on_board(game_coordinate(4.0, -4.0)));
    assert(!is_coordinat_on_board(game_coordinate(-4.0, -4.0)));
    assert(!is_coordinat_on_board(game_coordinate(400.0, 4.0)));
    assert(!is_coordinat_on_board(game_coordinate(4.0, 400.0)));
  }
  // to_notation
  {
    assert(to_notation(game_coordinate(0.5, 0.5)) == "a1");
    assert(to_notation(game_coordinate(9.5, 9.5)) == "--");
  }
  // operator ==
  {
    const game_coordinate a(1.2345, 6.7890);
    const game_coordinate b(1.2345, 6.7890);
    const game_coordinate c(0.0, 0.0);
    assert(a == b);
    assert(!(a == c));
  }
  // operator+
  {
    const game_coordinate a(1.1, 2.2);
    const game_coordinate b(3.3, 4.4);
    const game_coordinate c{a + b};
    assert(c.get_x() == a.get_x() + b.get_x());
    assert(c.get_y() == a.get_y() + b.get_y());
  }
  // operator-
  {
    const game_coordinate a(1.23, 4.56);
    const game_coordinate b(7.89, 0.12);
    const game_coordinate c{a - b};
    assert(c.get_x() == a.get_x() - b.get_x());
    assert(c.get_y() == a.get_y() - b.get_y());
  }
  // operator !=
  {
    const game_coordinate a(1.2345, 6.7890);
    const game_coordinate b(1.2345, 6.7890);
    const game_coordinate c(0.0, 0.0);
    assert(!(a != b));
    assert(a != c);
  }
  // operator*
  {
    const game_coordinate a(1.2345, 6.7890);
    const double factor{1.23};
    const auto b{a * factor};
    assert(b.get_x() == a.get_x() * factor);
    assert(b.get_y() == a.get_y() * factor);
  }
  // operator/
  {
    const game_coordinate a(1.2345, 6.7890);
    const double factor{2.34};
    const auto b{a / factor};
    assert(b.get_x() == a.get_x() / factor);
    assert(b.get_y() == a.get_y() / factor);
  }
  // operator<<
  {
    std::stringstream s;
    const game_coordinate c(1.2345, 6.7890);
    s << c;
    const std::string expected{"(1.2, 6.8)"};
    const std::string created{s.str()};
    assert(expected == created);
  }
  #endif // NDEBUG
}

std::string to_notation(const game_coordinate& g)
{
  if (g.get_x() >= 0.0
    && g.get_x() < 8.0
    && g.get_y() >= 0.0
    && g.get_y() < 8.0
  )
  {
    return to_str(square(g));
  }
  return "--";
}

bool operator==(const game_coordinate& lhs, const game_coordinate& rhs) noexcept
{
  return lhs.get_x() == rhs.get_x()
    && lhs.get_y() == rhs.get_y()
  ;
}

bool operator!=(const game_coordinate& lhs, const game_coordinate& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const game_coordinate& coordinat)
{
  const double x{std::round(coordinat.get_x() * 10.0) / 10.0};
  const double y{std::round(coordinat.get_y() * 10.0) / 10.0};
  os << "(" << x << ", "  << y << ")";
  return os;
}

game_coordinate operator-(const game_coordinate& lhs, const game_coordinate& rhs) noexcept
{
  return game_coordinate(
    lhs.get_x() - rhs.get_x(),
    lhs.get_y() - rhs.get_y()
  );
}

game_coordinate operator+(const game_coordinate& lhs, const game_coordinate& rhs) noexcept
{
  return game_coordinate(
    lhs.get_x() + rhs.get_x(),
    lhs.get_y() + rhs.get_y()
  );
}

game_coordinate& operator+=(game_coordinate& lhs, const game_coordinate& rhs) noexcept
{
  lhs = game_coordinate(lhs + rhs);
  return lhs;
}

game_coordinate operator/(const game_coordinate& coordinat, const double factor) noexcept
{
  return game_coordinate(
    coordinat.get_x() / factor,
    coordinat.get_y() / factor
  );

}

game_coordinate operator*(const game_coordinate& c, const double factor) noexcept
{
  return game_coordinate(
    c.get_x() * factor,
    c.get_y() * factor
  );
}
