#ifndef IN_GAME_CONTROLS_LAYOUT
#define IN_GAME_CONTROLS_LAYOUT

#include "navigation_controls_layout.h"
#include "screen_rect.h"
#include "action_number.h"

#include <iosfwd>
#include <map>

/// The layout for the controls in the game.
///
///
/// ```console
/// +--------------+
/// |     +---+    |
/// |     |Up |    |
/// | +---+---+---+|
/// | |Lft|Dwn|Rgt||
/// | +---+---+---+|
/// +---+----------+
/// |AK1|Text1     |
/// +---+----------+
/// |AK2|Text2     |
/// +---+----------+
/// |AK3|Text3     |
/// +---+----------+
/// |AK4|Text4     |
/// +---+----------+
/// ```
///
///
class in_game_controls_layout
{
public:
  in_game_controls_layout(const screen_rect& r = screen_rect(screen_coordinate(0, 0), screen_coordinate(64 * 3, 64 * 7)));

  /// The background
  const screen_rect& get_background() const noexcept { return m_background; }

  /// The symbol for the key of that action
  ///
  /// For example, the symbol for the key Q or LMB.
  ///
  /// The `AK1` in this image:
  ///
  /// ```console
  /// +---+----------+
  /// |AK1|Text1     |
  /// +---+----------+
  /// ```
  ///
  const screen_rect& get_action_key_symbol(const action_number& n) const noexcept;

  /// The text of what that action does
  ///
  /// For example 'select', 'attack', etc.
  ///
  /// The `Text1` in this image:
  ///
  /// ```console
  /// +---+----------+
  /// |AK1|Text1     |
  /// +---+----------+
  /// ```
  ///
  const screen_rect& get_action_key_text(const action_number& n) const noexcept;

  /// The layout for the navigational controls

  /// ```console
  /// +--------------+
  /// |     +---+    |
  /// |     |Up |    |
  /// | +---+---+---+|
  /// | |Lft|Dwn|Rgt||
  /// | +---+---+---+|
  /// +---+----------+
  /// ```
  const auto& get_navigation_controls() const noexcept { return m_navigation_controls; }

private:
  screen_rect m_background;
  navigation_controls_layout m_navigation_controls;

  std::map<action_number, screen_rect> m_action_key_symbol;
  std::map<action_number, screen_rect> m_action_key_text;
};

void test_in_game_controls_layout();

std::ostream& operator<<(std::ostream& os, const in_game_controls_layout& layout) noexcept;

#endif // IN_GAME_CONTROLS_LAYOUT

