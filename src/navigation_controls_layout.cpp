#include "navigation_controls_layout.h"

#include <cassert>

navigation_controls_layout::navigation_controls_layout(const screen_rect& r)
  : m_background{r}
{
  const int symbol_width{64};
  const int symbol_height{64};
  assert(get_height(r) == 2 * symbol_height);
  assert(get_width(r) == 3 * symbol_width);

  const int x1{r.get_tl().get_x()};
  const int x2{x1 + symbol_width};
  const int x3{x2 + symbol_width};
  const int x4{x3 + symbol_width};
  const int y1{r.get_tl().get_y()};
  const int y2{y1 + symbol_height};
  const int y3{y2 + symbol_height};
  m_up = screen_rect(screen_coordinate(x2, y1), screen_coordinate(x3, y2));
  m_left = screen_rect(screen_coordinate(x1, y2), screen_coordinate(x2, y3));
  m_down = screen_rect(screen_coordinate(x2, y2), screen_coordinate(x3, y3));
  m_right = screen_rect(screen_coordinate(x3, y2), screen_coordinate(x4, y3));


  m_mouse = create_centered_rect(get_center(m_background), 64, 64);
}

void test_navigation_controls_layout()
{
#ifndef NDEBUG
  // Constructor
  {
    const screen_rect r(
      screen_coordinate(0, 0),
      screen_coordinate(3 * 64, 2 * 64)
    );
    navigation_controls_layout layout(r);
    assert(r == layout.get_background());
  }
  // operator==
  {
    const screen_rect r(
      screen_coordinate(0, 0),
      screen_coordinate(3 * 64, 2 * 64)
    );
    navigation_controls_layout layout_1(r);
    navigation_controls_layout layout_2(r);
    assert(layout_1 == layout_2);
  }
  // operator!=
  {
    const screen_rect r1(
      screen_coordinate(0, 0),
      screen_coordinate(3 * 64, 2 * 64)
    );
    const screen_rect r2(
      screen_coordinate(1 * 64, 1 * 64),
      screen_coordinate(4 * 64, 3 * 64)
    );
    navigation_controls_layout layout_1(r1);
    navigation_controls_layout layout_2(r2);
    assert(layout_1 != layout_2);
  }
#endif // NDEBUG
}

bool operator==(const navigation_controls_layout& lhs, const navigation_controls_layout& rhs) noexcept
{
  return lhs.get_background() == rhs.get_background();
}

bool operator!=(const navigation_controls_layout& lhs, const navigation_controls_layout& rhs) noexcept
{
  return !(lhs == rhs);
}


