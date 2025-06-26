#ifndef NAVIGATION_CONTROLS_LAYOUT
#define NAVIGATION_CONTROLS_LAYOUT

#include "screen_rect.h"

/// The navigation controls for a player.
///
/// Must be 3x64 pixels wide and 2x64 pixels high
/// For a keyboard:
///
/// ```console
///     +---+
///     |Up |
/// +---+---+---+
/// |Lft|Dwn|Rgt|
/// +---+---+---+
/// ```
///
/// For a mouse
///
/// ```console
/// [Picture of a mouse with all four arrows]
/// ```
class navigation_controls_layout
{
public:
  navigation_controls_layout(const screen_rect& r = screen_rect(screen_coordinate(0, 0), screen_coordinate(3 * 64, 2 * 64)));

  const screen_rect& get_background() const noexcept { return m_background; }
  const screen_rect& get_down() const noexcept { return m_down; }
  const screen_rect& get_left() const noexcept { return m_left; }
  const screen_rect& get_mouse() const noexcept { return m_mouse; }
  const screen_rect& get_right() const noexcept { return m_right; }
  const screen_rect& get_up() const noexcept { return m_up; }

private:
  screen_rect m_background;
  screen_rect m_down;
  screen_rect m_left;
  screen_rect m_mouse;
  screen_rect m_right;
  screen_rect m_up;
};

void test_navigation_controls_layout();

bool operator==(const navigation_controls_layout& lhs, const navigation_controls_layout& rhs) noexcept;
bool operator!=(const navigation_controls_layout& lhs, const navigation_controls_layout& rhs) noexcept;


#endif // NAVIGATION_CONTROLS_LAYOUT
