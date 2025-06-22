#include "game_info_layout.h"

#include <cassert>

game_info_layout::game_info_layout(const screen_rect& r)
  : m_background(r)
{
  const int square_width{get_width(r)};
  const int square_height{get_height(r)};

  const int x1{r.get_tl().get_x()};
  const int x2 = x1 + (0.25 * square_width);
  const int x3 = x2 + (0.25 * square_width);
  const int x4 = x3 + (0.25 * square_width);
  const int x5{r.get_br().get_x()};

  const int y1{r.get_tl().get_y()};
  const int y2 = y1 + (0.33 * square_height);
  const int y3 = y2 + (0.33 * square_height);
  const int y4{r.get_br().get_y()};

  m_time = screen_rect(
    screen_coordinate(x1, y1),
    screen_coordinate(x2, y4)
  );

  // Piece value
  m_relative_piece_value = screen_rect(
    screen_coordinate(x2, y1),
    screen_coordinate(x3, y2)
  );
  m_piece_value[side::lhs] = screen_rect(
    screen_coordinate(x2, y2),
    screen_coordinate(x3, y3)
  );
  m_piece_value[side::rhs] = screen_rect(
    screen_coordinate(x2, y3),
    screen_coordinate(x3, y4)
  );

  // Activity
  m_relative_f_active = screen_rect(
    screen_coordinate(x3, y1),
    screen_coordinate(x4, y2)
  );
  m_f_active[side::lhs] = screen_rect(
    screen_coordinate(x3, y2),
    screen_coordinate(x4, y3)
  );
  m_f_active[side::rhs] = screen_rect(
    screen_coordinate(x3, y3),
    screen_coordinate(x4, y4)
  );

  // Protectedness
  m_relative_f_protected = screen_rect(
    screen_coordinate(x4, y1),
    screen_coordinate(x5, y2)
  );
  m_f_protected[side::lhs] = screen_rect(
    screen_coordinate(x4, y2),
    screen_coordinate(x5, y3)
  );
  m_f_protected[side::rhs] = screen_rect(
    screen_coordinate(x4, y3),
    screen_coordinate(x5, y4)
  );
}

void test_game_info_layout()
{
#ifndef NDEBUG
  // Constructor
  {
    const screen_rect r(
      screen_coordinate(0, 0),
      screen_coordinate(40, 30)
    );
    const auto layout{game_info_layout(r)};
    assert(layout.get_background().get_tl().get_x() == 0);
    assert(layout.get_background().get_tl().get_y() == 0);
    assert(layout.get_background().get_br().get_x() == 40);
    assert(layout.get_background().get_br().get_y() == 30);

    assert(layout.get_background() != layout.get_time());
    assert(layout.get_background() != layout.get_relative_piece_value());
    assert(layout.get_background() != layout.get_relative_f_active());
    assert(layout.get_background() != layout.get_relative_f_protected());

    assert(layout.get_piece_value(side::lhs) != layout.get_piece_value(side::rhs));
    assert(layout.get_f_active(side::lhs) != layout.get_f_active(side::rhs));
    assert(layout.get_f_protected(side::lhs) != layout.get_f_protected(side::rhs));

  }
#endif

}
