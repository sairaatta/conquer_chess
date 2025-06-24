#include "about_view_layout.h"

#include <cassert>
#include <cmath>

about_view_layout::about_view_layout(
  const screen_rect& r,
  const int margin_width
) : m_background{r}
{
  const int panel_height{64};
  const int panel_width{800};
  const int x1{128};
  const int x2{x1 + margin_width};
  const int x3{x2 + panel_width};
  const int x4{x3 + margin_width};

  const int y1{128};
  const int y2{y1 + margin_width};
  const int y3{y2 + 200};
  const int y4{y3 + margin_width};
  const int y5{y4 + (1 * panel_height)};
  const int y6{y5 + margin_width};
  const int y7{y6 + (1 * panel_height)};
  const int y8{y7 + margin_width};
  const int y9{y8 + (1 * panel_height)};
  const int y10{y9 + margin_width};
  const int y11{y10 + (2 * panel_height)};
  const int y12{y11 + margin_width};

  m_title = screen_rect(
    screen_coordinate(x2, y2),
    screen_coordinate(x3, y3)
  );
  m_subtitle = screen_rect(
    screen_coordinate(x2, y3),
    screen_coordinate(x3, y5)
  );
  m_copyright = screen_rect(
    screen_coordinate(x2, y6),
    screen_coordinate(x3, y7)
  );
  m_url = screen_rect(
    screen_coordinate(x2, y8),
    screen_coordinate(x3, y9)
  );
  m_contributors = screen_rect(
    screen_coordinate(x2, y10),
    screen_coordinate(x3, y11)
  );
  m_panel_outline = screen_rect(
    screen_coordinate(x1, y1),
    screen_coordinate(x4, y12)
  );
}

std::vector<screen_rect> get_panels(const about_view_layout& layout)
{
  return
  {
    layout.get_title(),
    layout.get_subtitle(),
    layout.get_copyright(),
    layout.get_url(),
    layout.get_contributors()
  };
}

void test_about_view_layout()
{
  #ifndef NDEBUG
  // get_panels
  {
    const about_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  #endif
}
