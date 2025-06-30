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

  const int symbol_width{square_height};
  const int symbol_height{symbol_width};

  m_relative[game_statistic_type::time] = screen_rect(
    screen_coordinate(x1, y1),
    screen_coordinate(x2 - 1, y4) // -1 to have non-overlaping rectangles
  );
  m_symbol[game_statistic_type::time] = screen_rect(
    m_relative[game_statistic_type::time].get_br() - screen_coordinate(symbol_width, symbol_height),
    m_relative[game_statistic_type::time].get_br()
  );

  // Piece value
  m_relative[game_statistic_type::value] = screen_rect(
    screen_coordinate(x2, y1),
    screen_coordinate(x3 - 1, y2) // -1 to have non-overlaping rectangles
  );
  m_absolute[game_statistic_type::value][side::lhs] = screen_rect(
    screen_coordinate(x2, y2),
    screen_coordinate(x3 - 1, y3) // -1 to have non-overlaping rectangles
  );
  m_absolute[game_statistic_type::value][side::rhs] = screen_rect(
    screen_coordinate(x2, y3),
    screen_coordinate(x3 - 1, y4) // -1 to have non-overlaping rectangles
  );
  m_symbol[game_statistic_type::value] = screen_rect(
    m_absolute[game_statistic_type::value][side::rhs].get_br() - screen_coordinate(symbol_width, symbol_height),
    m_absolute[game_statistic_type::value][side::rhs].get_br()
  );

  // Activity
  m_relative[game_statistic_type::activity] = screen_rect(
    screen_coordinate(x3, y1),
    screen_coordinate(x4 - 1, y2) // -1 to have non-overlaping rectangles
  );
  m_absolute[game_statistic_type::activity][side::lhs] = screen_rect(
    screen_coordinate(x3, y2),
    screen_coordinate(x4 - 1, y3) // -1 to have non-overlaping rectangles
  );
  m_absolute[game_statistic_type::activity][side::rhs] = screen_rect(
    screen_coordinate(x3, y3),
    screen_coordinate(x4 - 1, y4) // -1 to have non-overlaping rectangles
  );
  m_symbol[game_statistic_type::activity] = screen_rect(
    m_absolute[game_statistic_type::activity][side::rhs].get_br() - screen_coordinate(symbol_width, symbol_height),
    m_absolute[game_statistic_type::activity][side::rhs].get_br()
  );

  // Protectedness
  m_relative[game_statistic_type::protectedness] = screen_rect(
    screen_coordinate(x4, y1),
    screen_coordinate(x5, y2)
  );
  m_absolute[game_statistic_type::protectedness][side::lhs] = screen_rect(
    screen_coordinate(x4, y2),
    screen_coordinate(x5, y3)
  );
  m_absolute[game_statistic_type::protectedness][side::rhs] = screen_rect(
    screen_coordinate(x4, y3),
    screen_coordinate(x5, y4)
  );
  m_symbol[game_statistic_type::protectedness] = screen_rect(
    m_absolute[game_statistic_type::protectedness][side::rhs].get_br() - screen_coordinate(symbol_width, symbol_height),
    m_absolute[game_statistic_type::protectedness][side::rhs].get_br()
  );
}

int get_height(const game_info_layout& layout) noexcept
{
  return get_height(layout.get_background());
}

int get_width(const game_info_layout& layout) noexcept
{
  return get_width(layout.get_background());
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

    assert(layout.get_background() != layout.get_relative(game_statistic_type::time));
    assert(layout.get_background() != layout.get_relative(game_statistic_type::value));
    assert(layout.get_background() != layout.get_relative(game_statistic_type::activity));
    assert(layout.get_background() != layout.get_relative(game_statistic_type::protectedness));

    assert(layout.get_absolute(game_statistic_type::value, side::lhs) != layout.get_absolute(game_statistic_type::value, side::rhs));
    assert(layout.get_absolute(game_statistic_type::activity, side::lhs) != layout.get_absolute(game_statistic_type::activity, side::rhs));
    assert(layout.get_absolute(game_statistic_type::protectedness, side::lhs) != layout.get_absolute(game_statistic_type::protectedness, side::rhs));

    // Use the full rectangle
    assert(layout.get_relative(game_statistic_type::time).get_tl().get_x() == layout.get_background().get_tl().get_x());
    assert(layout.get_relative(game_statistic_type::time).get_tl().get_y() == layout.get_background().get_tl().get_y());
    assert(layout.get_background().get_br().get_x() == layout.get_relative(game_statistic_type::protectedness).get_br().get_x());
    assert(layout.get_background().get_br().get_y() == layout.get_absolute(game_statistic_type::protectedness, side::rhs).get_br().get_y());

    // No overlapping pixels
    assert(layout.get_relative(game_statistic_type::time).get_br().get_x() + 1 == layout.get_relative(game_statistic_type::value).get_tl().get_x());
    assert(layout.get_relative(game_statistic_type::value).get_br().get_x() + 1 == layout.get_relative(game_statistic_type::activity).get_tl().get_x());
    assert(layout.get_relative(game_statistic_type::activity).get_br().get_x() + 1 == layout.get_relative(game_statistic_type::protectedness).get_tl().get_x());

  }
#endif

}
