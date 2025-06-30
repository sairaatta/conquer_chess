#include "game_statistics_view_layout.h"

#include <cassert>
#include <cmath>

game_statistics_view_layout::game_statistics_view_layout(
  const screen_rect& r,
  const int margin_width
) : m_background{r}
{
  assert(margin_width > 0);
  const int panel_width{1200};
  const int panel_height{800};
  const int x1{64};
  const int x2{x1 + panel_width};

  const int y1{24};
  const int y2{y1 + panel_height};

  m_text = screen_rect(
    screen_coordinate(x1, y1),
    screen_coordinate(x2, y2)
  );
}

std::vector<screen_rect> get_panels(const game_statistics_view_layout& layout)
{
  return
  {
    layout.get_text()
  };
}

void test_game_statistics_view_layout()
{
  #ifndef NDEBUG
  // get_panels
  {
    const game_statistics_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  #endif
}
