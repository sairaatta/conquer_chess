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
  in_game_controls_layout(const screen_rect& r = screen_rect(screen_coordinate(0, 0), screen_coordinate(1, 1)));

  /// The background
  const screen_rect& get_background() const noexcept { return m_background; }

  /// Get the screen area for a row for an action key.
  ///
  /// The square that shows:
  ///   1. the action name, e.g. 'attack'
  ///      Use \link{get_controls_key_name} to get the screen_rect
  ///      for this section
  ///   2. the action icon, e.g. an arrow that points to a cross for attack.
  ///      Use \link{get_controls_key_icon} to get the screen_rect
  ///      for this section
  ///   3. the user input, e.g. 'Q' or LMB.
  ///      Use \link{get_controls_key_input} to get the screen_rect
  ///      for this section
  ///
  /// ```console
  /// +---+----------+
  /// |AK1|Text1     |
  /// +---+----------+
  /// ```
  ///
  const screen_rect& get_controls_key(const action_number& key) const noexcept;

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
  std::map<action_number, screen_rect> m_controls_key;
  std::map<action_number, screen_rect> m_keys;
};

void test_in_game_controls_layout();

std::ostream& operator<<(std::ostream& os, const in_game_controls_layout& layout) noexcept;

#endif // IN_GAME_CONTROLS_LAYOUT

