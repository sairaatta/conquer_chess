#ifndef GAME_STATISTICS_IN_TIME_H
#define GAME_STATISTICS_IN_TIME_H

#include "ccfwd.h"
#include "game_statistics.h"

class game_statistics_in_time
{
public:
  game_statistics_in_time();

  void add(const game& g);
  void add(const game_statistics& s);

  const auto& get() const noexcept { return m_statistics; }

private:
  std::vector<game_statistics> m_statistics;
};


/// Create a `game_statistics_in_time` used for testing
game_statistics_in_time create_test_game_statistics_in_time();

/// Play a random game, used for profiling
game_statistics_in_time play_random_game_to_get_statistics_in_time(const int n_turns = 2);


void test_game_statistics_in_time();

#endif // GAME_STATISTICS_IN_TIME_H
