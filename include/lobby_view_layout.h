#ifndef LOBBY_VIEW_LAYOUT_H
#define LOBBY_VIEW_LAYOUT_H

#include "side.h"
#include "lobby_view_item.h"
#include "read_only.h"
#include "screen_rect.h"
#include "layout.h"

#include <map>
#include <vector>

/// The layout of the Lobby dialog.
///
/// ```text
///   x          x x          x x          x x          x
///   1          2 3          4 5          6 7          8
///
/// +-----------------------------------------------------+
/// |                                                     |
/// | +----------+ +-----------------------+ +----------+ | y1
/// | |          | |                       | |          | |
/// | | lhs king | | title                 | | rhs king | |
/// | | portrait | |                       | | portrait | |
/// | |          | +-----------------------+ |          | | y2
/// | |          |                           |          | |
/// | |          | +----------+ +----------+ |          | | y3
/// | |          | |          | |          | |          | |
/// | |          | | color    | | color    | |          | |
/// | |          | |          | |          | |          | |
/// | |          | +----------+ +----------+ |          | | y4
/// | |          |                           |          | |
/// | +----------+ +----------+ +----------+ |          | | y5
/// |              |          | |          | |          | |
/// |              | race     | | race     | |          | |
/// |              |          | |          | |          | |
/// |              +----------+ +----------+ |          | | y6
/// |                                        |          | |
/// |              +----------+ +----------+ |          | | y7 <-+
/// |              |          | |          | |          | |      |
/// |              | ready    | | read     | |          | |      +- panel_height
/// |              |          | |          | |          | |      |
/// |              +----------+ +----------+ +----------+ | y8 <-+
/// |                                                     |
/// +-----------------------------------------------------+
///
///              ^          ^
///              |          |
///              +----------+
///               panel_width
/// ```
class lobby_view_layout
{
public:
  explicit lobby_view_layout(
    const screen_rect& r = get_default_screen_rect(),
    const int margin_width = get_default_margin_width()
  );

  const auto& get_background() const noexcept { return m_background; }
  const screen_rect& get_color(const side player_side) const noexcept;
  const screen_rect& get_king_portrait(const side player_side) const noexcept;
  const screen_rect& get_race(const side player_side) const noexcept;
  const screen_rect& get_ready(const side player_side) const noexcept;
  const auto& get_title() const noexcept { return m_title; }

private:



  screen_rect m_background;

  std::map<side, screen_rect> m_color;
  std::map<side, screen_rect> m_king_portrait;
  std::map<side, screen_rect> m_race;
  std::map<side, screen_rect> m_ready;
  screen_rect m_title;

  std::map<side, screen_rect> m_cursor;
};

/// Get the rectangle of the item
const screen_rect& get_cursor_rect(
  const lobby_view_layout& layout,
  const lobby_view_item item,
  const side player_side
) noexcept;

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const lobby_view_layout& layout);

/// Test the lobby_view_layout class
void test_lobby_view_layout();

#endif // LOBBY_VIEW_LAYOUT_H
