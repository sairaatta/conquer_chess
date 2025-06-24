#include "lobby_view_layout.h"

#include <cassert>
#include <cmath>
#include "controls_bar.h"

lobby_view_layout::lobby_view_layout(
  const screen_rect& r,
  const int margin_width
) : m_background{r}
{
  const int w{get_width(r)};
  const int h{get_height(r)};

  // Central panel
  const int n_vertical_units{5};
  const int n_vertical_margins{n_vertical_units + 1}; // margins are above, below and between panels
  const int panel_height{
    static_cast<int>(
      static_cast<double>(
        h - (n_vertical_margins * margin_width))
        / static_cast<double>(n_vertical_units)
    )
  };
  const int panel_width{
    300
  };
  const int x1{margin_width};
  const int x2{(w / 2) - panel_width - (6 * margin_width)};
  const int x3{x2 + margin_width + margin_width};
  const int x4{x3 + panel_width};
  const int x5{x4 + margin_width + margin_width};
  const int x6{x5 + panel_width};
  const int x7{x6 + margin_width + margin_width};
  const int x8{w - margin_width};

  const int title_height{200};
  const int y1{margin_width};
  const int y2{y1 + title_height};
  const int y3{y2 + margin_width + margin_width + margin_width};
  const int y4{y3 + panel_height};
  const int y5{y4 + margin_width};
  const int y6{y5 + panel_height};
  const int y7{y6 + margin_width};
  const int y8{y7 + panel_height};

  m_title = screen_rect(
    screen_coordinate(x3, y1),
    screen_coordinate(x6, y2)
  );
  m_color[side::lhs] = screen_rect(
    screen_coordinate(x3, y3),
    screen_coordinate(x4, y4)
  );
  m_race[side::lhs] = screen_rect(
    screen_coordinate(x3, y5),
    screen_coordinate(x4, y6)
  );
  m_ready[side::lhs] = screen_rect(
    screen_coordinate(x3, y7),
    screen_coordinate(x4, y8)
  );
  m_color[side::rhs] = screen_rect(
    screen_coordinate(x5, y3),
    screen_coordinate(x6, y4)
  );
  m_race[side::rhs] = screen_rect(
    screen_coordinate(x5, y5),
    screen_coordinate(x6, y6)
  );
  m_ready[side::rhs] = screen_rect(
    screen_coordinate(x5, y7),
    screen_coordinate(x6, y8)
  );

  // Portraits
  const int portrait_height{y1 + 400};
  m_king_portrait[side::lhs] = screen_rect(
    screen_coordinate(x1, y1),
    screen_coordinate(x2, portrait_height)
  );
  m_king_portrait[side::rhs] = screen_rect(
    screen_coordinate(x7, y1),
    screen_coordinate(x8, portrait_height)
  );
}


const screen_rect& lobby_view_layout::get_color(const side player_side) const noexcept
{
  return m_color.at(player_side);
}

const screen_rect& lobby_view_layout::get_king_portrait(const side player_side) const noexcept
{
  return m_king_portrait.at(player_side);
}

std::vector<screen_rect> get_panels(const lobby_view_layout& layout)
{
  return
  {
    // layout.get_title(), // Needs to show the background
    layout.get_color(side::lhs),
    layout.get_race(side::lhs),
    layout.get_ready(side::lhs),
    layout.get_color(side::rhs),
    layout.get_race(side::rhs),
    layout.get_ready(side::rhs),
  };
}

const screen_rect& lobby_view_layout::get_race(const side player_side) const noexcept
{
  return m_race.at(player_side);
}

const screen_rect& lobby_view_layout::get_ready(const side player_side) const noexcept
{
  return m_ready.at(player_side);
}

const screen_rect& get_cursor_rect(
  const lobby_view_layout& layout,
  const lobby_view_item item,
  const side player_side
) noexcept
{
  switch (item)
  {
    case lobby_view_item::color: return layout.get_color(player_side);
    case lobby_view_item::race: return layout.get_race(player_side);
    default:
    case lobby_view_item::start:
      assert(item == lobby_view_item::start);
      return layout.get_ready(player_side);
  }
}

void test_lobby_view_layout()
{
  #ifndef NDEBUG
  // get_cursor_rect
  {
    const lobby_view_layout layout;
    const auto r1{get_cursor_rect(layout, lobby_view_item::color, side::lhs)};
    const auto r2{get_cursor_rect(layout, lobby_view_item::race, side::lhs)};
    const auto r3{get_cursor_rect(layout, lobby_view_item::start, side::lhs)};
    const auto r4{get_cursor_rect(layout, lobby_view_item::color, side::rhs)};
    const auto r5{get_cursor_rect(layout, lobby_view_item::race, side::rhs)};
    const auto r6{get_cursor_rect(layout, lobby_view_item::start, side::rhs)};
    assert(r1 != r2);
    assert(r1 != r3);
    assert(r1 != r4);
    assert(r1 != r5);
    assert(r1 != r6);
  }
  // get_color
  {
    const lobby_view_layout layout;
    assert(layout.get_color(side::lhs) != layout.get_color(side::rhs));
  }
  // get_king_portrait
  {
    const lobby_view_layout layout;
    assert(layout.get_king_portrait(side::lhs) != layout.get_king_portrait(side::rhs));
  }
  // get_panels
  {
    const lobby_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  // get_race
  {
    const lobby_view_layout layout;
    assert(layout.get_race(side::lhs) != layout.get_race(side::rhs));
  }
  // get_ready
  {
    const lobby_view_layout layout;
    assert(layout.get_ready(side::lhs) != layout.get_ready(side::rhs));
  }
  #endif
}
