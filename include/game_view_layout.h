#ifndef GAME_VIEW_LAYOUT_H
#define GAME_VIEW_LAYOUT_H

#include "ccfwd.h"
#include "screen_rect.h"
#include "game_coordinate.h"
#include "screen_coordinate.h"
#include "layout.h"
#include "side.h"
#include "board_layout.h"
//#include "read_only.h"

#include <optional>
#include <iosfwd>
#include <map>
#include <vector>

/// The layout of the Game dialog.
///
/// The layout of the Game dialog (\link{game_view}).
///
/// ```text
///   x          x x     x x     x x          x
///   1          2 3     4 5     6 7          8
///
/// +-------------------------------------------+
/// |                                           |
/// | +----------+ +-------------+ +----------+ | y1
/// | | unit     | |             | | unit     | |
/// | |          | |             | |          | |
/// | |          | |             | |          | |
/// | | info     | |             | | info     | |
/// | +----------+ | board       | +----------+ | y2
/// | | controls | |             | | controls | |
/// | |          | |             | |          | |
/// | |          | |             | |          | |
/// | |          | +-------------+ |          | |
/// | |          |                 |          | |
/// | |          | +-------------+ |          | | y3
/// | |          | | game_info   | |          | |
/// | +----------+ +-------------+ +----------+ | y4
/// |                                           |
/// | +------------------+ +------------------+ | y5
/// | | log              | | log              | |
/// | +------------------+ +------------------+ | y6
/// |                                           |
/// | +------------------+ +------------------+ | y7
/// | | debug            | | debug            | |
/// | +------------------+ +------------------+ | y8
/// |                                           |
/// +-------------------------------------------+
///
///   ^          ^ ^            ^  ^          ^
///   |          | |            |  |          |
///   +----------+ +------------+  +----------+
///    panel_width  board_width     panel_width
///     300 pixels   remainder       300 pixels
/// ```
class game_view_layout
{
public:
  explicit game_view_layout(
    const screen_coordinate& window_size = get_default_screen_size(),
    const int margin_width = get_default_margin_width()
  );

  /// The entire screen are
  const auto& get_background() const noexcept { return m_background; }

  const auto& get_board() const { return m_board.value(); }
  const screen_rect& get_controls(const side player) const noexcept;

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
  const screen_rect& get_controls_key(const side player, const action_number& key) const noexcept;

  const screen_rect& get_debug(const side player) const noexcept;

  const auto& get_game_info() const noexcept { return m_game_info; }

  /// The area where the frame rate (in frames per seconds) is dsplayed
  const screen_rect& get_log(const side player) const noexcept;

  const screen_rect& get_navigation_controls(const side player) const noexcept;

  /// Information about a unit
  const screen_rect& get_unit_info(const side player) const noexcept;


private:

  screen_rect m_background;
  // screen_rect m_board;
  std::optional<board_layout> m_board;
  std::map<side, screen_rect> m_controls;
  std::map<action_number, std::map<side, screen_rect>> m_controls_key;
  std::map<side, screen_rect> m_debug;
  screen_rect m_game_info;
  std::map<side, screen_rect> m_log;
  std::map<side, screen_rect> m_navigation_controls;
  std::map<side, screen_rect> m_unit_info;
};

/// Convert an in-game coordinat to an on-screen coordinat
screen_coordinate convert_to_screen_coordinate(
  const game_coordinate& coordinat,
  const game_view_layout& layout
);

/// Convert an in-game coordinat to an on-screen coordinat
screen_rect convert_to_screen_rect(
  const game_rect& coordinat,
  const game_view_layout& layout
);

/// Convert an in-game square to an on-screen screen rectange
screen_rect convert_to_screen_rect(
  const square& s,
  const game_view_layout& layout
);

/// Convert an on-screen coordinat to an in-game coordinat
game_coordinate convert_to_game_coordinate(
  const screen_coordinate& coordinat,
  const game_view_layout& layout
);

/// Get the height of the board in pixels
int get_board_height(const game_view_layout& layout) noexcept;

/// Get the width of the board in pixels
int get_board_width(const game_view_layout& layout) noexcept;

/// Get the height of a square
double get_square_height(const game_view_layout& layout) noexcept;

/// Get the width of a square
double get_square_width(const game_view_layout& layout) noexcept;

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const game_view_layout& layout);

/// Test the game_view_layout class
void test_game_view_layout();

std::ostream& operator<<(std::ostream& os, const game_view_layout& layout) noexcept;

#endif // GAME_VIEW_LAYOUT_H
