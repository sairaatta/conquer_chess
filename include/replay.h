#ifndef REPLAY_H
#define REPLAY_H

#include "ccfwd.h"
#include "chess_move.h"
#include "pgn_string.h"

#include <iosfwd>
#include <vector>

/// A replay of a regular chess match.
///
/// It converts a chess game in PGN notatation
/// to a collection of chess moves.
class replay
{
public:
  /// @param pgn_str a string of PGN notation in one line,
  /// e.g.
  explicit replay(const pgn_string& pgn_str = pgn_string());

  const auto& get_moves() const noexcept { return m_moves; }

private:

  std::vector<chess_move> m_moves;

};

/// Get the number of moves in the replay
int get_n_moves(const replay& r) noexcept;

replay get_replay_1() noexcept;

/// Test this class and its free functions
void test_replay();

bool operator==(const replay& lhs, const replay& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const replay& r) noexcept;

#endif // REPLAY_H
