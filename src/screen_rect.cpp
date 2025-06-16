#include "screen_rect.h"

#include <cassert>
#include <iostream>
#include <sstream>

screen_rect::screen_rect(
  const screen_coordinate& top_left,
  const screen_coordinate& bottom_right
) : m_top_left{top_left},
    m_bottom_right{bottom_right}
{

}

screen_coordinate get_center(const screen_rect& r) noexcept
{
  return screen_coordinate(
    (r.get_tl().get_x() + r.get_br().get_x()) / 2,
    (r.get_tl().get_y() + r.get_br().get_y()) / 2
  );
}

screen_rect get_bottom_left_corner(const screen_rect& r) noexcept
{
  const screen_coordinate top_left(
    r.get_tl().get_x(),
    (r.get_tl().get_y() + r.get_br().get_y()) / 2
  );
  const screen_coordinate bottom_right(
    (r.get_tl().get_x() + r.get_br().get_x()) / 2,
    r.get_br().get_y()
  );
  const screen_rect corner(top_left, bottom_right);
  return corner;
}

screen_rect get_bottom_right_corner(const screen_rect& r) noexcept
{
  const screen_coordinate top_left(
    (r.get_tl().get_x() + r.get_br().get_x()) / 2,
    (r.get_tl().get_y() + r.get_br().get_y()) / 2
  );
  const screen_coordinate bottom_right(
    r.get_br().get_x(),
    r.get_br().get_y()
  );
  const screen_rect corner(top_left, bottom_right);
  return corner;
}

int get_height(const screen_rect& r) noexcept
{
  return r.get_br().get_y() - r.get_tl().get_y();
}

screen_rect get_lower_eighth(const screen_rect& r) noexcept
{
  return get_lower_half(get_lower_fourth(r));
}

screen_rect get_lower_fourth(const screen_rect& r) noexcept
{
  return get_lower_half(get_lower_half(r));
}

screen_rect get_lower_half(const screen_rect& r) noexcept
{
  const screen_coordinate top_left(
    r.get_tl().get_x(),
    (r.get_tl().get_y() + r.get_br().get_y()) / 2
  );
  const screen_coordinate bottom_right(
    r.get_br().get_x(),
    r.get_br().get_y()
  );
  const screen_rect half(top_left, bottom_right);
  return half;
}

screen_rect get_top_left_corner(const screen_rect& r) noexcept
{
  const screen_coordinate top_left(
    r.get_tl().get_x(),
    r.get_tl().get_y()
  );
  const screen_coordinate bottom_right(
    (r.get_tl().get_x() + r.get_br().get_x()) / 2,
    (r.get_tl().get_y() + r.get_br().get_y()) / 2
  );
  const screen_rect corner(top_left, bottom_right);
  return corner;
}

screen_rect get_top_right_corner(const screen_rect& r) noexcept
{
  const screen_coordinate top_left(
    (r.get_tl().get_x() + r.get_br().get_x()) / 2,
    r.get_tl().get_y()
  );
  const screen_coordinate bottom_right(
    r.get_br().get_x(),
    (r.get_tl().get_y() + r.get_br().get_y()) / 2
  );
  const screen_rect corner(top_left, bottom_right);
  return corner;
}

screen_rect get_upper_half(const screen_rect& r) noexcept
{
  const screen_coordinate top_left(
    r.get_tl().get_x(),
    r.get_tl().get_y()
  );
  const screen_coordinate bottom_right(
    r.get_br().get_x(),
    (r.get_tl().get_y() + r.get_br().get_y()) / 2
  );
  const screen_rect half(top_left, bottom_right);
  return half;
}

int get_width(const screen_rect& r) noexcept
{
  return r.get_br().get_x() - r.get_tl().get_x();
}

bool is_in(const screen_coordinate& pos, const screen_rect& r) noexcept
{
  return pos.get_x() >= r.get_tl().get_x()
    && pos.get_x() <= r.get_br().get_x()
    && pos.get_y() >= r.get_tl().get_y()
    && pos.get_y() <= r.get_br().get_y()
  ;
}

void test_screen_rect()
{
#ifndef NDEBUG
  // get_bottom_left_corner
  {
    const screen_rect r(screen_coordinate(0, 0), screen_coordinate(2, 2));
    const screen_rect created{get_bottom_left_corner(r)};
    assert(r != created);
    const screen_rect expected(
      screen_coordinate(0, 1),
      screen_coordinate(1, 2)
    );
    assert(created == expected);
  }
  // get_bottom_right_corner
  {
    const screen_rect r(screen_coordinate(0, 0), screen_coordinate(2, 2));
    const screen_rect created{get_bottom_right_corner(r)};
    assert(r != created);
    const screen_rect expected(
      screen_coordinate(1, 1),
      screen_coordinate(2, 2)
    );
    assert(created == expected);
  }
  // get_lower_half
  {
    const screen_rect r(screen_coordinate(0, 0), screen_coordinate(2, 2));
    const screen_rect created{get_lower_half(r)};
    assert(r != created);
    const screen_rect expected(
      screen_coordinate(0, 1),
      screen_coordinate(2, 2)
    );
    assert(created == expected);
  }
  // get_lower_fourth
  {
    const screen_rect r(screen_coordinate(0, 0), screen_coordinate(2, 4));
    const screen_rect created{get_lower_fourth(r)};
    assert(r != created);
    const screen_rect expected(
      screen_coordinate(0, 3),
      screen_coordinate(2, 4)
    );
    assert(created == expected);
  }
  // get_lower_eighth
  {
    const screen_rect r(screen_coordinate(0, 0), screen_coordinate(2, 8));
    const screen_rect created{get_lower_eighth(r)};
    assert(r != created);
    const screen_rect expected(
      screen_coordinate(0, 7),
      screen_coordinate(2, 8)
    );
    assert(created == expected);
  }
  // get_top_left_corner
  {
    const screen_rect r(screen_coordinate(0, 0), screen_coordinate(2, 2));
    const screen_rect created{get_top_left_corner(r)};
    assert(r != created);
    const screen_rect expected(
      screen_coordinate(0, 0),
      screen_coordinate(1, 1)
    );
    assert(created == expected);
  }
  // get_top_right_corner
  {
    const screen_rect r(screen_coordinate(0, 0), screen_coordinate(2, 2));
    const screen_rect created{get_top_right_corner(r)};
    assert(r != created);
    const screen_rect expected(
      screen_coordinate(1, 0),
      screen_coordinate(2, 1)
    );
    assert(created == expected);
  }
  // get_upper_half
  {
    const screen_rect r(screen_coordinate(0, 0), screen_coordinate(2, 2));
    const screen_rect created{get_upper_half(r)};
    assert(r != created);
    const screen_rect expected(
      screen_coordinate(0, 0),
      screen_coordinate(2, 1)
    );
    assert(created == expected);
  }
  // get_width and get_height
  {
    //   012345678901
    // 0 ............
    // 1 ..+---|---+.
    // 2 ..|   |   |.
    // 3 ..----X----.
    // 4 ..|   |   |.
    // 5 ..+---|---+.
    // 6 ............

    const screen_rect r(screen_coordinate(2, 1), screen_coordinate(10, 5));
    assert(get_width(r) == 8); // The bottom-left is excluded?
    assert(get_height(r) == 4); // The bottom-left is excluded?
  }
  // get_center
  {
    //   012345678901
    // 0 ............
    // 1 ..+---|---+.
    // 2 ..|   |   |.
    // 3 ..----X----.
    // 4 ..|   |   |.
    // 5 ..+---|---+.
    // 6 ............

    const screen_rect r(screen_coordinate(2, 1), screen_coordinate(10, 5));
    const auto c{get_center(r)};
    assert(c.get_x() == 6);
    assert(c.get_y() == 3);
  }
  // get_default_controls_screen_size
  {
    const auto r{get_default_controls_screen_size()};
    assert(r.get_x() > 0);
  }
  // is_in
  {
    const screen_rect r(
      screen_coordinate(10, 20),
      screen_coordinate(100, 200)
    );
    assert(is_in(screen_coordinate(50, 60), r));
    assert(!is_in(screen_coordinate(0, 60), r));
    assert(!is_in(screen_coordinate(1000, 60), r));
    assert(!is_in(screen_coordinate(50, 0), r));
    assert(!is_in(screen_coordinate(60, 600), r));
  }
  // operator==
  {
    const screen_rect a;
    const screen_rect b;
    const screen_rect c(screen_coordinate(1, 2), screen_coordinate(3, 4));
    assert(a == b);
    assert(!(a == c));
  }
  // operator!=
  {
    const screen_rect a;
    const screen_rect b;
    const screen_rect c(screen_coordinate(1, 2), screen_coordinate(3, 4));
    assert(!(a != b));
    assert(a != c);
  }
  // operator<<
  {
    std::stringstream s;
    s << screen_rect();
    assert(!s.str().empty());
  }
#endif // DEBUG
}

bool operator==(const screen_rect& lhs, const screen_rect& rhs) noexcept
{
  return lhs.get_tl() == rhs.get_tl()
    && lhs.get_br() == rhs.get_br()
  ;
}

bool operator!=(const screen_rect& lhs, const screen_rect& rhs) noexcept
{
  return !(lhs == rhs);
}

screen_rect& operator+=(screen_rect& rect, const screen_coordinate& delta) noexcept
{
  rect = screen_rect(
    rect.get_tl() + delta,
    rect.get_br() + delta
  );
  return rect;
}

std::ostream& operator<<(std::ostream& os, const screen_rect& r) noexcept
{
  os << "(" << r.get_tl() << "-" << r.get_br() << ")";
  return os;
}
