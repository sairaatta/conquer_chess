#include "lobby_view_layout.h"

#include <cassert>
#include <cmath>

lobby_view_layout::lobby_view_layout(
  const screen_coordinate& window_size,
  const int margin_width
) : m_font_size{64},
    m_window_size{window_size}
{
  m_background = screen_rect(
    screen_coordinate(0, 0),
    screen_coordinate(m_window_size.get_x(), m_window_size.get_y())
  );

  // Central panel
  const int n_vertical_units{5};
  const int n_vertical_margins{n_vertical_units + 1}; // margins are above, below and between panels
  const int panel_height{
    static_cast<int>(
      static_cast<double>(
        window_size.get_y() - (n_vertical_margins * margin_width))
        / static_cast<double>(n_vertical_units)
    )
  };
  const int panel_width{
    300
  };
  const int x1{margin_width};
  const int x2{(window_size.get_x() / 2) - panel_width - (6 * margin_width)};
  const int x3{x2 + margin_width + margin_width};
  const int x4{x3 + panel_width};
  const int x5{x4 + margin_width + margin_width};
  const int x6{x5 + panel_width};
  const int x7{x6 + margin_width + margin_width};
  const int x8{window_size.get_x() - margin_width};

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
  m_lhs_color = screen_rect(
    screen_coordinate(x3, y3),
    screen_coordinate(x4, y4)
  );
  m_lhs_race = screen_rect(
    screen_coordinate(x3, y5),
    screen_coordinate(x4, y6)
  );
  m_lhs_start = screen_rect(
    screen_coordinate(x3, y7),
    screen_coordinate(x4, y8)
  );
  m_rhs_color = screen_rect(
    screen_coordinate(x5, y3),
    screen_coordinate(x6, y4)
  );
  m_rhs_race = screen_rect(
    screen_coordinate(x5, y5),
    screen_coordinate(x6, y6)
  );
  m_rhs_start = screen_rect(
    screen_coordinate(x5, y7),
    screen_coordinate(x6, y8)
  );

  // Side panel
  const int control_panel_width{250};
  const int control_panel_height{80};
  const int sx1{((x1 + x2) / 2) - (control_panel_width / 2)};
  const int sx2{sx1 + control_panel_width};

  const int sx7{((x7 + x8) / 2) - (control_panel_width / 2)};
  const int sx8{sx7 + control_panel_width};

  const int sy1{y1};
  const int sy99{window_size.get_y() - margin_width};
  const int sy98{sy99 - control_panel_height};
  const int sy97{sy98 - control_panel_height};
  const int sy96{sy97 - control_panel_height};
  const int sy95{sy96 - control_panel_height};
  const int sy94{sy95 - control_panel_height};
  m_lhs_king_portrait = screen_rect(
    screen_coordinate(x1, sy1),
    screen_coordinate(x2, sy94)
  );
  m_lhs_control_symbol = screen_rect(
    screen_coordinate(sx1, sy95),
    screen_coordinate(sx2, sy96)
  );
  m_lhs_control_up = screen_rect(
    screen_coordinate(sx1, sy96),
    screen_coordinate(sx2, sy97)
  );
  m_lhs_control_up_label = get_lhs_half(m_lhs_control_up);
  m_lhs_control_up_symbol = get_rhs_half(m_lhs_control_up);
  m_lhs_control_down = screen_rect(
    screen_coordinate(sx1, sy97),
    screen_coordinate(sx2, sy98)
  );
  m_lhs_control_down_label = get_lhs_half(m_lhs_control_down);
  m_lhs_control_down_symbol = get_rhs_half(m_lhs_control_down);
  m_lhs_control_select = screen_rect(
    screen_coordinate(sx1, sy98),
    screen_coordinate(sx2, sy99)
  );
  m_lhs_control_select_label = get_lhs_half(m_lhs_control_select);
  m_lhs_control_select_symbol = get_rhs_half(m_lhs_control_select);


  m_rhs_king_portrait = screen_rect(
    screen_coordinate(x7, sy1),
    screen_coordinate(x8, sy94)
  );
  m_rhs_control_symbol = screen_rect(
    screen_coordinate(sx7, sy95),
    screen_coordinate(sx8, sy96)
  );
  m_rhs_control_up = screen_rect(
    screen_coordinate(sx7, sy96),
    screen_coordinate(sx8, sy97)
  );
  m_rhs_control_up_label = get_lhs_half(m_rhs_control_up);
  m_rhs_control_up_symbol = get_rhs_half(m_rhs_control_up);
  m_rhs_control_down = screen_rect(
    screen_coordinate(sx7, sy97),
    screen_coordinate(sx8, sy98)
  );
  m_rhs_control_down_label = get_lhs_half(m_rhs_control_down);
  m_rhs_control_down_symbol = get_rhs_half(m_rhs_control_down);
  m_rhs_control_select = screen_rect(
    screen_coordinate(sx7, sy98),
    screen_coordinate(sx8, sy99)
  );
  m_rhs_control_select_label = get_lhs_half(m_rhs_control_select);
  m_rhs_control_select_symbol = get_rhs_half(m_rhs_control_select);

}


const screen_rect& lobby_view_layout::get_color(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_color;
  }
  assert(player_side == side::rhs);
  return m_rhs_color;
}

const screen_rect& lobby_view_layout::get_control_down(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_control_down;
  }
  assert(player_side == side::rhs);
  return m_rhs_control_down;
}

const screen_rect& lobby_view_layout::get_control_down_label(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_control_down_label;
  }
  assert(player_side == side::rhs);
  return m_rhs_control_down_label;
}

const screen_rect& lobby_view_layout::get_control_down_symbol(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_control_down_symbol;
  }
  assert(player_side == side::rhs);
  return m_rhs_control_down_symbol;
}

const screen_rect& lobby_view_layout::get_control_select(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_control_select;
  }
  assert(player_side == side::rhs);
  return m_rhs_control_select;
}

const screen_rect& lobby_view_layout::get_control_select_label(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_control_select_label;
  }
  assert(player_side == side::rhs);
  return m_rhs_control_select_label;
}

const screen_rect& lobby_view_layout::get_control_select_symbol(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_control_select_symbol;
  }
  assert(player_side == side::rhs);
  return m_rhs_control_select_symbol;
}

const screen_rect& lobby_view_layout::get_control_symbol(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_control_symbol;
  }
  assert(player_side == side::rhs);
  return m_rhs_control_symbol;
}

const screen_rect& lobby_view_layout::get_control_up(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_control_up;
  }
  assert(player_side == side::rhs);
  return m_rhs_control_up;
}

const screen_rect& lobby_view_layout::get_control_up_label(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_control_up_label;
  }
  assert(player_side == side::rhs);
  return m_rhs_control_up_label;
}

const screen_rect& lobby_view_layout::get_control_up_symbol(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_control_up_symbol;
  }
  assert(player_side == side::rhs);
  return m_rhs_control_up_symbol;
}

const screen_rect& lobby_view_layout::get_king_portrait(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_king_portrait;
  }
  assert(player_side == side::rhs);
  return m_rhs_king_portrait;
}

std::vector<screen_rect> get_panels(const lobby_view_layout& layout)
{
  return
  {
    // layout.get_title(), // Needs to show the background
    layout.get_color(side::lhs),
    layout.get_control_down(side::lhs),
    layout.get_control_select(side::lhs),
    layout.get_control_symbol(side::lhs),
    layout.get_control_up(side::lhs),
    layout.get_race(side::lhs),
    layout.get_start(side::lhs),
    layout.get_color(side::rhs),
    layout.get_control_down(side::rhs),
    layout.get_control_select(side::rhs),
    layout.get_control_symbol(side::rhs),
    layout.get_control_up(side::rhs),
    layout.get_race(side::rhs),
    layout.get_start(side::rhs),
  };
}

const screen_rect& lobby_view_layout::get_race(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_race;
  }
  assert(player_side == side::rhs);
  return m_rhs_race;
}

const screen_rect& lobby_view_layout::get_start(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_start;
  }
  assert(player_side == side::rhs);
  return m_rhs_start;
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
      return layout.get_start(player_side);
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
  // get_panels
  {
    const lobby_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  #endif
}
