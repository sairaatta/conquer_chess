#ifndef GAME_INFO_LAYOUT_H
#define GAME_INFO_LAYOUT_H

#include "screen_rect.h"
#include "side.h"

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

  // Time
  const screen_rect& get_time() const noexcept { return m_time; }
  const screen_rect& get_time_symbol() const noexcept { return m_time_symbol; }

  // Piece value
  const screen_rect& get_relative_piece_value() const noexcept { return m_relative_piece_value; }
  const screen_rect& get_piece_value(const side player_side) const noexcept { return m_piece_value.at(player_side); }
  const screen_rect& get_piece_value_symbol() const noexcept { return m_piece_value_symbol; }

  // Activity
  const screen_rect& get_relative_f_active() const noexcept { return m_relative_f_active; }
  const screen_rect& get_f_active(const side player_side) const noexcept { return m_f_active.at(player_side); }
  const screen_rect& get_activity_symbol() const noexcept { return m_activity_symbol; }

  // Protectedness
  const screen_rect& get_relative_f_protected() const noexcept { return m_relative_f_protected; }
  const screen_rect& get_f_protected(const side player_side) const noexcept { return m_f_protected.at(player_side); }
  const screen_rect& get_protectedness_symbol() const noexcept { return m_protectedness_symbol; }

private:

  screen_rect m_background;

  // Time
  screen_rect m_time;
  screen_rect m_time_symbol;

  // Piece value
  screen_rect m_relative_piece_value;
  std::map<side, screen_rect> m_piece_value;
  screen_rect m_piece_value_symbol;

  // Activity
  screen_rect m_relative_f_active;
  std::map<side, screen_rect> m_f_active;
  screen_rect m_activity_symbol;

  // Protectedness
  screen_rect m_relative_f_protected;
  std::map<side, screen_rect> m_f_protected;
  screen_rect m_protectedness_symbol;
};

int get_height(const game_info_layout& layout) noexcept;

int get_width(const game_info_layout& layout) noexcept;

void test_game_info_layout();

#endif // GAME_INFO_LAYOUT_H
