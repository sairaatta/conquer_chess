#include "menu_view_layout.h"

#include <cassert>
#include <cmath>

menu_view_layout::menu_view_layout(
  const screen_coordinate& window_size,
  const int margin_width
) : m_window_size{window_size}
{
  const int panel_height{100};
  const int panel_width{400};
  const int x1{128 + margin_width};
  const int x2{x1 + margin_width};
  const int x3{x2 + panel_width};
  const int x4{x3 + margin_width};
  const int x5{x4 + margin_width};
  const int x6{window_size.get_x() - margin_width};
  assert(x4 < x5);
  assert(x5 < x6);

  const int y1{28 + margin_width};
  const int y2{y1 + margin_width};
  const int y3{y2 + panel_height + margin_width + panel_height};
  const int y4{y3 + 0};
  const int y5{y4 + panel_height};
  const int y6{y5 + margin_width};
  const int y7{y6 + panel_height};
  const int y8{y7 + margin_width};
  const int y9{y8 + panel_height};
  const int y10{y9 + margin_width};
  const int y11{y10 + panel_height};
  const int y12{y11 + margin_width};
  const int y13{y12 + panel_height};
  const int y14{y13 + margin_width};
  const int y15{y14 + panel_height};
  const int y16{y15 + margin_width};
  const int y17{y16 + margin_width + margin_width};
  const int y18{y17 + 64};

  m_menu_panel = screen_rect(
    screen_coordinate(x1, y1),
    screen_coordinate(x4, y16)
  );
  m_title = screen_rect(
    screen_coordinate(x2, y2),
    screen_coordinate(x3, y3)
  );
  m_subtitle = screen_rect(
    screen_coordinate(x2, y4),
    screen_coordinate(x3, y5)
  );
  m_background_image = screen_rect(
    screen_coordinate(0, 0),
    screen_coordinate(window_size.get_x(), window_size.get_y())
  );
  m_start = screen_rect(
    screen_coordinate(x2, y8),
    screen_coordinate(x3, y9)
  );
  m_options = screen_rect(
    screen_coordinate(x2, y10),
    screen_coordinate(x3, y11)
  );
  m_about = screen_rect(
    screen_coordinate(x2, y12),
    screen_coordinate(x3, y13)
  );
  m_quit = screen_rect(
    screen_coordinate(x2, y14),
    screen_coordinate(x3, y15)
  );
  m_controls = screen_rect(
    screen_coordinate(x2, y17),
    screen_coordinate(x3, y18)
  );
}

std::vector<screen_rect> get_panels(const menu_view_layout& layout)
{
  return
  {
    layout.get_title(),
    layout.get_subtitle(),
    layout.get_start(),
    layout.get_options(),
    layout.get_about(),
    layout.get_quit()
    // layout.get_controls() // Blends in with background
  };
}


const screen_rect& menu_view_layout::get_selectable_rect(const menu_view_item item) const noexcept
{
  switch (item)
  {
    case menu_view_item::start: return m_start;
    case menu_view_item::options: return m_options;
    case menu_view_item::about: return m_about;
    default:
    case menu_view_item::quit:
      assert(item == menu_view_item::quit);
      return m_quit;
  }
}

void test_menu_view_layout()
{
  #ifndef NDEBUG
  // get_panels
  {
    const menu_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  // get_selectable_rect
  {
    const menu_view_layout layout;
    assert(layout.get_selectable_rect(menu_view_item::start) == layout.get_start());
    assert(layout.get_selectable_rect(menu_view_item::options) == layout.get_options());
    assert(layout.get_selectable_rect(menu_view_item::about) == layout.get_about());
    assert(layout.get_selectable_rect(menu_view_item::quit) == layout.get_quit());
  }
  #endif
}
