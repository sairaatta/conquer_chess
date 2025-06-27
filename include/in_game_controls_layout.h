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
  /// @seealso
  /// - Use \link{get_action_key_symbol} to get
  ///   the symbol for the key for that action
  /// - Use \link{get_action_key_text} to get
  ///   the text of what that action does
  const screen_rect& get_action_key_row(const action_number& n) const noexcept;

  /// The symbol for the key of that action
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

  /// The full row
  std::map<action_number, screen_rect> m_action_key_rows;
  std::map<action_number, screen_rect> m_action_key_symbol;
  std::map<action_number, screen_rect> m_action_key_text;
};

void test_in_game_controls_layout();

std::ostream& operator<<(std::ostream& os, const in_game_controls_layout& layout) noexcept;

#endif // IN_GAME_CONTROLS_LAYOUT

