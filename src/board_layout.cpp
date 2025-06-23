#include "board_layout.h"

#include "screen_coordinate.h"

#include <cassert>

board_layout::board_layout(const screen_rect& r)
  : m_background(r)
{
  assert(get_width(r) >= 8);
  assert(get_height(r) >= 8);


  // Create the x coordinates of the squares
  std::vector<double> xs;
  {
    double x = r.get_tl().get_x();
    const double dx = get_width(r) / 8.0;
    xs.push_back(x);
    for (int i = 0; i != 8; ++i)
    {
      x += dx;
      xs.push_back(x);
    }
  }
  assert(xs.size() == 8 + 1);

  // Create the y coordinates of the squares
  std::vector<double> ys;
  {
    double y = r.get_tl().get_y();
    const double dy = get_height(r) / 8.0;
    ys.push_back(y);
    for (int i = 0; i != 8; ++i)
    {
      y += dy;
      ys.push_back(y);
    }
  }

  m_rects.resize(8);
  for (int y = 0; y != 8; ++y)
  {
    m_rects[y].resize(8);
    for (int x = 0; x != 8; ++x)
    {
      const screen_rect square_rect{
        screen_coordinate(xs[x], ys[y]),
        screen_coordinate(xs[x + 1], ys[y + 1])
      };
      m_rects[y][x] = square_rect;
    }
  }
}

const screen_rect& board_layout::get_background() const noexcept
{
  return m_background;
}

int get_height(const board_layout& b)
{
  return get_height(b.get_background());
}

int get_width(const board_layout& b)
{
  return get_width(b.get_background());

}

const screen_rect& board_layout::get_square(const int x, const int y) const
{
  assert(x >= 0);
  assert(x < 8);
  assert(y >= 0);
  assert(y < 8);
  assert(m_rects.size() == 8);
  assert(m_rects[y].size() == 8);
  return m_rects[y][x];
}


void test_board_layout()
{
#ifndef NDEBUG
  // Constructor
  {
    const board_layout layout(
      screen_rect(
        screen_coordinate(0, 0),
        screen_coordinate(64, 64)
      )
    );
    const auto& a1{layout.get_square(0, 0)};
    assert(a1.get_tl().get_x() == 0);
    assert(a1.get_tl().get_y() == 0);
    assert(a1.get_br().get_x() == 8);
    assert(a1.get_br().get_y() == 8);

    const auto& a8{layout.get_square(7, 0)};
    assert(a8.get_tl().get_x() == 64 - 8);
    assert(a8.get_tl().get_y() == 0);
    assert(a8.get_br().get_x() == 64);
    assert(a8.get_br().get_y() == 8);
  }

#endif // NDEBUG
}
