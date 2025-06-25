#ifndef GAME_INFO_LAYOUT_H
#define GAME_INFO_LAYOUT_H

#include "screen_rect.h"
#include "side.h"
#include "game_info_statistic.h"

#include <map>
/// The layout of the game info
///
/// ```text
/// x                       .x.                      .x.                      .x.                       x
/// 1                       .2.                      .3.                      .4.                      .5
///
/// +-----------------------++-----------------------++-----------------------++------------------------+ y1
/// |                       ||+--------------+------+||+--------+------------+||+----------------------+| .
/// |                       |||relative piece|value ||||relative|activity    ||||relative protectedness||
/// |                       ||+--------------+------+||+--------+------------+||+----------------------+| .
/// |                       ++-----------------------++-----------------------++------------------------+ y2
/// |                       ||+---------------+      ||+--------------+       ||+------------------+    | .
/// |   game  time          |||piece value lhs|      |||activity lhs  |       |||protectedness lhs |    |
/// |                       ||+---------------+      ||+--------------+       ||+------------------+    | .
/// |                       ++-----------------------++-----------------------++------------------------+ y3
/// |                       ||+-----------------+    ||+------------+         ||+--------------------+  | .
/// |                       |||piece value rhs  |    ||activity rhs |         ||protectedness rhs    |  |
/// |                       ||+-----------------+    ||+------------+         ||+--------------------+  | .
/// +-----------------------++-----------------------++-----------------------++------------------------+ y4
///
/// - 1: time symbol
/// - 2: piece value symbol
/// - 3: activity symbol
/// - 4: protectedness symbol
/// - .: a one-pixel difference
/// ```
///
class game_info_layout
{
public:

  game_info_layout(const screen_rect& square_rect = screen_rect(screen_coordinate(0, 0), screen_coordinate(8, 4)));

  const screen_rect& get_background() const noexcept { return m_background; }

  /// Get the symbol for a statistic
  const screen_rect& get_symbol(const game_info_statistic s) const { return m_symbol.at(s); }

  /// Get the rectangle where the relative statistics are shown.
  ///
  /// The rectangle for time is in here as well.
  const screen_rect& get_relative(const game_info_statistic s) const { return m_relative.at(s); }

  /// Get the rectangle where, per player, the absolute value of a statistic is shown.
  ///
  /// The rectangle for time is absent here.
  const screen_rect& get_absolute(const game_info_statistic s, const side player_side) const noexcept { return m_absolute.at(s).at(player_side); }

private:

  /// The full rectangle
  screen_rect m_background;

  /// Absolute value of a statistic per side
  std::map<game_info_statistic, std::map<side, screen_rect>> m_absolute;

  /// Absolute value of a statistic per side
  std::map<game_info_statistic, screen_rect> m_relative;

  /// Symbols for the statistics
  std::map<game_info_statistic, screen_rect> m_symbol;
};

int get_height(const game_info_layout& layout) noexcept;

int get_width(const game_info_layout& layout) noexcept;

void test_game_info_layout();

#endif // GAME_INFO_LAYOUT_H
