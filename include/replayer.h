#ifndef REPLAYER_H
#define REPLAYER_H

#include "ccfwd.h"
#include "delta_t.h"
#include "in_game_time.h"
#include "game_controller.h"

#include "action_history.h"

#include <iosfwd>

/// A replay.
///
/// A replayer plays a game again by playing the action history of a game.
///
/// Use 'do_move' to go forward in time.
///
class replayer
{
public:
  explicit replayer(
    const action_history& r = action_history(),
    const game_controller& c = create_game_controller_with_user_settings(create_game_with_user_settings())
  );

  /// Do a move or do nothing, for 1 time unit
  void do_move(const delta_t& dt = delta_t(1.0));

  /// Get the history of actions, from a replay
  const auto& get_action_history() const noexcept { return m_action_history; }

  /// Get the current state of the game
  const auto& get_game() const noexcept { return m_game_controller.get_game(); }

  const auto& get_index() const noexcept { return m_index; };

private:

  /// The history of actions, from a replay
  action_history m_action_history;

  /// The current state of the game
  game_controller m_game_controller;

  /// The index of the next move to do in m_action_history
  int m_index;



};


const in_game_time& get_in_game_time(
  const replayer& r
) noexcept;

/// Get the number of moves in the replay
int get_n_moves(const replayer& r) noexcept;

game get_played_scholars_mate();

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const replayer& r,
  const square& coordinat
);

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const replayer& r,
  const std::string& square_str
);

/// Test this class and its free functions
void test_replayer();

bool operator==(const replayer& lhs, const replayer& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const replayer& r) noexcept;

#endif // REPLAYER_H
