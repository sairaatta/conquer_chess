#ifndef GAME_STATISTICS_IN_TIME_H
#define GAME_STATISTICS_IN_TIME_H

#include "ccfwd.h"
#include "game_statistics.h"

class game_statistics_in_time
{
public:
  game_statistics_in_time();

  void add(const game& g);

  const auto& get() const noexcept { return m_statistics; }

private:
  std::vector<game_statistics> m_statistics;
};

void test_game_statistics_in_time();

#endif // GAME_STATISTICS_IN_TIME_H
