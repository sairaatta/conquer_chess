#include "replay_view_layout.h"

#include <cassert>
#include <cmath>

replay_view_layout::replay_view_layout(
  const screen_rect& r
) : m_background{r}
{
  const int board_width{800};
  const int board_height{board_width};
  const int statistics_width{board_width};
  const int widget_height{80};
  //const int statistics_height{statistics_width};
  const int x1{32};
  const int x2{x1 + board_width};
  const int x3{x2 + 32};
  const int x4{x3 + statistics_width};

  const int y1{24};
  const int y2{y1 + board_height};
  const int y3{y2 + 16};
  const int y4{y3 + widget_height};

  m_board = screen_rect(
    screen_coordinate(x1, y1),
    screen_coordinate(x2, y2)
  );

  m_statistics = screen_rect(
    screen_coordinate(x3, y1),
    screen_coordinate(x4, y2)
  );

  m_statistics_widget = game_statistics_widget_layout(
    screen_rect(
      screen_coordinate(x1, y3),
      screen_coordinate(x4, y4)
    )
  );
}

void test_replay_view_layout()
{
  #ifndef NDEBUG
  //
  {
    const replay_view_layout layout;
    assert(get_width(layout.get_background()) > 0);
    assert(get_width(layout.get_board()) > 0);
    assert(get_width(layout.get_statistics()) > 0);
    assert(get_width(layout.get_statistics_widget()) > 0);
  }
  #endif
}
