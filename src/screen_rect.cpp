#include "screen_rect.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>

screen_rect::screen_rect(
  const screen_coordinate& top_left,
  const screen_coordinate& bottom_right
) : m_top_left{top_left},
    m_bottom_right{bottom_right}
{
  assert(top_left != bottom_right);
  assert(top_left.get_x() < bottom_right.get_x());
  assert(top_left.get_y() < bottom_right.get_y());
}

screen_rect create_partial_rect_from_lhs(const screen_rect& r, const double f)
{
  assert(f >= 0.0);
  assert(f <= 1.0);
  const int width(get_width(r));
  const double dx{f * static_cast<double>(width)};
  const int n_pixels{static_cast<int>(std::round(dx))};
  assert(n_pixels > 0);
  const int x1{r.get_tl().get_x()};
  const int y1{r.get_tl().get_y()};
  const int x2{r.get_tl().get_x() + n_pixels};
  const int y2{r.get_br().get_y()};
  const screen_coordinate tl(x1, y1);
  const screen_coordinate br(x2, y2);
  return screen_rect(tl, br);
}


screen_rect create_rect_inside(const screen_rect& r) noexcept
{
  const auto tl{r.get_tl() + screen_coordinate(1, 1)};
  const auto br{r.get_br() - screen_coordinate(1, 1)};
  return screen_rect(tl, br);
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


screen_rect create_centered_rect(const screen_coordinate c, const int w, const int h) noexcept
{
  assert(w > 0);
  assert(h > 0);
  const int x1{c.get_x() - (w / 2)};
  const int y1{c.get_y() - (h / 2)};
  const screen_rect r(
    screen_coordinate(x1, y1),
    screen_coordinate(x1 + w, y1 + h)
  );
  assert(get_height(r) == h);
  assert(get_width(r) == w);
  return r;
}

screen_coordinate get_center(const screen_rect& r) noexcept
{
  return screen_coordinate(
    (r.get_tl().get_x() + r.get_br().get_x()) / 2,
    (r.get_tl().get_y() + r.get_br().get_y()) / 2
  );
}

screen_rect get_default_screen_rect() noexcept
{
  const screen_coordinate tl(0, 0);
  const screen_coordinate br(1080, 1920);
  return screen_rect(tl, br);
}

int get_height(const screen_rect& r) noexcept
{
  return r.get_br().get_y() - r.get_tl().get_y();
}

screen_rect get_lhs_half(const screen_rect& r) noexcept
{
  const screen_coordinate top_left(
    r.get_tl().get_x(),
    r.get_tl().get_y()
  );
  const screen_coordinate bottom_right(
    (r.get_tl().get_x() + r.get_br().get_x()) / 2,
    r.get_br().get_y()
  );
  const screen_rect half(top_left, bottom_right);
  return half;
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

screen_rect get_rhs_half(const screen_rect& r) noexcept
{
  const screen_coordinate top_left(
    (r.get_tl().get_x() + r.get_br().get_x()) / 2,
    r.get_tl().get_y()
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
  // screen_rect::get_br and screen_rect::get_tl
  {
    const screen_coordinate tl(1, 2);
    const screen_coordinate br(3, 4);
    const screen_rect r(tl, br);
    assert(r.get_br() == br);
    assert(r.get_tl() == tl);
  }
  // create_centered_rect
  {
    // Easy
    {
      const int width{64};
      const int height{128};
      const auto r{create_centered_rect(screen_coordinate(100, 200), width, height)};
      assert(get_width(r) == width);
      assert(get_height(r) == height);
      assert(r.get_tl().get_x() == 100 - 32);
      assert(r.get_tl().get_y() == 200 - 64);
      assert(r.get_br().get_x() == 100 + 32);
      assert(r.get_br().get_y() == 200 + 64);
    }
    // Harder
    {
      const int width{65};
      const int height{129};
      const auto r{create_centered_rect(screen_coordinate(101, 201), width, height)};
      assert(get_width(r) == width);
      assert(get_height(r) == height);
    }
  }
  // create_partial_rect_from_lhs
  {
    const screen_rect r(screen_coordinate(100, 200), screen_coordinate(300, 400));
    const screen_rect created{create_partial_rect_from_lhs(r, 0.25)};
    const screen_rect expected(screen_coordinate(100, 200), screen_coordinate(150, 400));
    assert(created == expected);
  }

  // create_rect_inside
  {
    const screen_rect r(screen_coordinate(1, 2), screen_coordinate(11, 12));
    const screen_rect created{create_rect_inside(r)};
    assert(r != created);
    const screen_rect expected(
      screen_coordinate(2, 3),
      screen_coordinate(10, 11)
    );
    assert(created == expected);
  }

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
  // get_center
  {
    const screen_rect r(
      screen_coordinate(100, 200),
      screen_coordinate(300, 400)
    );
    const screen_coordinate expected(
      200, 300
    );
    const auto result{get_center(r)};
    assert(result == expected);
  }
  // get_lhs_half
  {
    const screen_rect r(screen_coordinate(0, 0), screen_coordinate(2, 2));
    const screen_rect created{get_lhs_half(r)};
    assert(r != created);
    const screen_rect expected(
      screen_coordinate(0, 0),
      screen_coordinate(1, 2)
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
  // get_rhs_half
  {
    const screen_rect r(screen_coordinate(0, 0), screen_coordinate(2, 2));
    const screen_rect created{get_rhs_half(r)};
    assert(r != created);
    const screen_rect expected(
      screen_coordinate(1, 0),
      screen_coordinate(2, 2)
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
    const auto r{get_default_screen_rect()};
    assert(r.get_tl().get_x() == 0);
    assert(r.get_tl().get_y() == 0);
    assert(r.get_br().get_x() > 320);
    assert(r.get_br().get_y() > 200);
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

/*
screen_rect& operator+=(screen_rect& rect, const screen_coordinate& delta) noexcept
{
  rect = screen_rect(
    rect.get_tl() + delta,
    rect.get_br() + delta
  );
  return rect;
}
*/

std::ostream& operator<<(std::ostream& os, const screen_rect& r) noexcept
{
  os << "(" << r.get_tl() << "-" << r.get_br() << ")";
  return os;
}
