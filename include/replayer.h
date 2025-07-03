#ifndef REPLAYER_H
#define REPLAYER_H

#include "ccfwd.h"
#include "delta_t.h"
#include "in_game_time.h"

#include "action_history.h"

#include <iosfwd>

/// Player that replays a match
class replayer
{
public:
  explicit replayer(const action_history& r = action_history());

  /// Do a move or do nothing, for 1 time unit
  void do_move(
    game_controller& c,
    game& g,
    const delta_t& dt = delta_t(1.0)
  );

  const auto& get_action_history() const noexcept { return m_action_history; }

  const auto& get_index() const noexcept { return m_index; };

private:

  /// The index of the next move to do in m_action_history
  int m_index;

  /// A replay
  action_history m_action_history;
};

game get_played_scholars_mate();

/// Get the number of moves in the replay
int get_n_moves(const replayer& r) noexcept;

/// Test this class and its free functions
void test_replayer();

bool operator==(const replayer& lhs, const replayer& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const replayer& r) noexcept;

#endif // REPLAYER_H
